import sys
import os
import math
import numpy as np
import xlrd                           # xlrd 这玩意不能读 xlsx 啊
import xlwt
import re
from openpyxl import Workbook
from openpyxl import load_workbook
from openpyxl.compat import range
import openpyxl.cell
from numpy import *
import time
import timeit
from tqdm import tqdm, trange
from time import sleep


def CreateDataSet(wordlist, corpus_path):
    MATRIX = []  # 11855个训练样本, 9000个feature + 1个 label

    catelist = os.listdir(corpus_path)  # 获取seg_path下的所有子目录，也就是分类信息
    for category in catelist:  # 遍历每一个种类文件夹
        cate_path = corpus_path + category + "/"  # 拼出分类子目录的路径
        filelist = os.listdir(cate_path)  # 获取class_path下的所有文件
        for file in filelist:
            FILE = open(cate_path + '/' + file, 'r').read()
            WORDS = set()
            word = ''
            for char in FILE:
                if char != '\t':  # 工作原理: 一个字符一个字符地读, 将不是制表符的字符丢进 word 字符串当中
                    word += char  # 连接字符
                else:  # 读到了制表符, 就从 word 字符串当中读取一个单词, 进行词频统计!
                    WORDS.add(word)
                    word = ''  # 清空字符串,等待下一个词的输入
            Feature_Vector = []  # 文本的向量
            for key in wordlist:
                if key in WORDS:
                    Feature_Vector.append('YES')
                else:
                    Feature_Vector.append('NO')
            Feature_Vector.append(category)
            MATRIX.append(Feature_Vector)     # 用 List 嵌套 List 来实现矩阵

    return MATRIX


def calcShannonEnt(dataSet):
    numEntris = len(dataSet)          # labelcounts字典键为类别，值为该类别样本数量
    labelcounts = {}
    for featVec in dataSet:           # 得到dataset中每个样本的最后一个元素，即类别
        currentlabel = featVec[-1]
        if currentlabel not in labelcounts.keys():      # 当前样本类别labelcounts中没有，添加
            labelcounts[currentlabel] = 0              # 有则当前样本所属类别数量加一
        labelcounts[currentlabel] += 1

    shannonEnt = 0.0    # 计算香农熵
    for key in labelcounts:
        prob = float(labelcounts[key]/numEntris)
        shannonEnt -= prob * math.log(prob, 2)  # numpy 的 log 好像第二个参数是 out array 要用 math 的 log 才会是 base
    return shannonEnt


def spiltDataSet(dataSet, axis, value):  #  划分数据集（数据集，划分特征索引，特征值）
    #  python中函数参数按引用传递，因此函数中构建参数的复制
    #  防止传入的参数原值被修改
    retDataSet = []
    for featVec in dataSet:
        if featVec[axis] == value:      # 去掉当前这个特征（因为划分中已用过）
            reducedFeatVec = featVec[:axis]
            reducedFeatVec.extend(featVec[axis+1:])
            retDataSet.append(reducedFeatVec)
    return retDataSet


def chooseBestFeatureToSplit(dataset):  # 选择最好的划分特征（数据集）
    numFeatures = len(dataset[0])-1      # 特征数量
    bestEntropy = calcShannonEnt(dataset)      # 原始数据集信息熵
    bestInfoGain = 0.0      # 最优的信息增益
    bestFeature = -1   # 最优的特征索引

    for i in range(numFeatures):
        featList = [example[i] for example in dataset]  # 取第i个特征
        uniqueVals = set(featList)   # set构建集合，将列表中重复元素合并
        newEntropy = 0.0
        for value in uniqueVals:
            subDataSet = spiltDataSet(dataset, i, value)  # 按照所取当前特征的不同值划分数据集
            prob = len(subDataSet)/float(len(dataset))              # 计算当前划分的累计香农熵
            newEntropy += prob*calcShannonEnt(subDataSet)
        infoGain = bestEntropy-newEntropy          # 得到当前特征划分的信息增益

        if infoGain > bestInfoGain:         # 选出最大的信息增益特征
            bestInfoGain = infoGain
            bestFeature = i
    return bestFeature


def dict2list(dic:dict):  # 将字典转化为列表
    keys = dic.keys()
    vals = dic.values()
    lst = [(key, val) for key, val in zip(keys, vals)]
    return lst


# 若特征用完后仍存在同一分支下有不同类别的样本
# 则此时采用投票方式决定该分支隶属类别
# 即该分支下哪个类别最多，该分支就属哪个类别
def majorityCnt(classList):
    classCount = {}
    for vote in classList:
        if vote not in classCount.keys():
            classCount[vote] = 0
        classCount[vote] += 1
    # 字典排序（字典的迭代器，按照第1个域排序也就是值而不是键，True是降序）
    sortedClassCount = sorted(dict2list(classCount), key = operator.itemgetter(1), reverse=True)
    # 返回类别
    return sortedClassCount[0][0]


# 递归构建决策树
def creatertree(dataset, labels):
    classList = [example[-1] for example in dataset]     # 取类别
    # 如果classList中索引为0的类别数量和classList元素数量相等
    # 即分支下都属同一类，停止递归
    if classList.count(classList[0]) == len(classList):
        return classList[0]
    # 划分类别的特征已用完，停止递归，返回投票结果
    if len(dataset[0]) == 1:
        return majorityCnt(classList)
    # 选择最具区分度特征
    bestFeat = chooseBestFeatureToSplit(dataset)
    bestFeatLabel = labels[bestFeat]
    # 树用嵌套的字典表示
    myTree = {bestFeatLabel:{}}
    del(labels[bestFeat])
    featValues = [example[bestFeat] for example in dataset]
    uniqueVals = set(featValues)
    for value in uniqueVals:
        subLabels = labels[:]
        # 递归构建决策树
        myTree[bestFeatLabel][value] = creatertree(spiltDataSet(dataset, bestFeat, value), subLabels)
    return myTree


# 分类函数（决策树，标签，待分类样本）
def classify(inputTree, featLabels, testVec):
    firstSides = list(inputTree.keys())
    # 找到输入的第一个元素
    firstStr = firstSides[0]
    # 这里表明了python3和python2版本的差别，上述两行代码在2.7中为：firstStr = inputTree.key()[0]
    secondDict = inputTree[firstStr]
    # 找到在label中firstStr的下标
    featIndex = featLabels.index(firstStr)
    # for i in secondDict.keys():
        # print(i)

    classLabel = 'Education'

    for key in secondDict.keys():
        if testVec[featIndex] == key:
            if type(secondDict[key]) == dict:   # 判断一个变量是否为dict，直接type就好
                classLabel = classify(secondDict[key], featLabels, testVec)
            else:
                classLabel = secondDict[key]
    # 比较测试数据中的值和树上的值，最后得到节点
    return classLabel


def treetest(mytree, wordlist, test_path):
    n = 0
    correct = 0
    catelist = os.listdir(test_path)  # 获取seg_path下的所有子目录，也就是分类信息
    for category in catelist:  # 遍历每一个种类文件夹
        cate_path = corpus_path + category + "/"  # 拼出分类子目录的路径
        filelist = os.listdir(cate_path)  # 获取class_path下的所有文件
        for file in filelist:
            FILE = open(cate_path + '/' + file, 'r').read()
            WORDS = set()
            word = ''
            for char in FILE:
                if char != '\t':  # 工作原理: 一个字符一个字符地读, 将不是制表符的字符丢进 word 字符串当中
                    word += char  # 连接字符
                else:  # 读到了制表符, 就从 word 字符串当中读取一个单词, 进行词频统计!
                    WORDS.add(word)
                    word = ''  # 清空字符串,等待下一个词的输入
            Feature_Vector = []  # 文本的向量
            for key in wordlist:
                if key in WORDS:
                    Feature_Vector.append('YES')
                else:
                    Feature_Vector.append('NO')
            feature_label = category

            predict = classify(mytree, feature_label, Feature_Vector)
            if predict == feature_label:
                correct += 1
            n += 1
            print("Actual:", flabel, "---- Predict:", expct_cate)

    print('Accuracy:', correct / n)


if __name__ == "__main__":
    wordlist_path = '/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/WORDS.xlsx'
    corpus_path = '/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Train_Set/'
    test_path = '/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test_Set/'

    cpu_start = time.clock()
    time_start = time.time()

    wordlist = set()
    file = xlrd.open_workbook(wordlist_path)
    sheet = file.sheet_by_index(0)
    Features = []
    for rownum in range(1, 927):
        row = sheet.row_values(rownum)
        word = row[0]
        wordlist.add(word)
        Features.append(word)

    DataSets = CreateDataSet(wordlist, corpus_path)
    Decision_Tree = creatertree(DataSets, Features)
    treetest(Decision_Tree, wordlist, test_path)

    cpu_end = time.clock()
    time_end = time.time()

    print('cpu:', cpu_end - cpu_start)
    print('time: ', time_end - time_start)  # 以秒为单位
    print(Decision_Tree)
    trainsets = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/1800.txt"
    with open(trainsets, mode='w', encoding='utf-8') as outFile:
        outFile.write(Decision_Tree)
