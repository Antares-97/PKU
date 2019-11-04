import textdata
import TF_IDF
import os
import numpy as np
import math
from numpy import *
from numpy.random.mtrand import power
from matplotlib import pyplot as plt

# coding = 'gbk'


def distEclud(vecA, vecB):
    vecC = vecA - vecB
    vecC = multiply(vecC, vecC)
    row = shape(vecC)[0]
    col = shape(vecC)[1]
    result = 0
    for i in range(row):
        for j in range(col):
            result += vecC[i, j]

    return sqrt(result)


def randCent(dataSet, k):
    n = shape(dataSet)[1]
    centroids = mat(zeros((k, n)))  # k*n
    for j in range(n):
        minJ = min(dataSet[:, j])  #
        rangeJ = float(max(dataSet[:, j]) - minJ)
        centroids[:, j] = minJ + rangeJ * random.rand(k, 1)
    return centroids  #


def kMeans(dataSet, k, distMeas=distEclud, createCent=randCent):
    m = shape(dataSet)[0]  #
    clusterAssment = mat(zeros((m, 2)))  #
    centroids = createCent(dataSet, k)  #
    clusterChanged = True  #
    while clusterChanged:
        clusterChanged = False
        for i in range(m):  #
            minDist = inf  #
            minIndex = -1
            for j in range(k):  #
                distJI = distMeas(centroids[j, :], dataSet[i, :])  #
                if distJI < minDist:  #
                    minDist = distJI
                    minIndex = j
            if clusterAssment[i, 0] != minIndex:
                clusterChanged = True
            clusterAssment[i, :] = minIndex, minDist ** 2
            # print centroids
        for cent in range(k):
            ptsInClust = dataSet[nonzero(clusterAssment[:, 0].A == cent)[0]]
            centroids[cent, :] = mean(ptsInClust, axis=0)
    return centroids, clusterAssment


def show(dataSet, k, centroids, clusterAssment):
    numSamples, dim = dataSet.shape  #
    mark = ['or', 'ob', 'og', 'ok', '^r', '+r', 'sr', 'dr', '<r', 'pr']
    for i in range(numSamples):  #
        markIndex = int(clusterAssment[i, 0])
        plt.plot(dataSet[i, 0], dataSet[i, 1], mark[markIndex])

    mark = ['Dr', 'Db', 'Dg', 'Dk', '^b', '+b', 'sb', 'db', '<b', 'pb']
    for i in range(k):2
        plt.plot(centroids[i, 0], centroids[i, 1], mark[i], markersize=12)
    plt.show()


if __name__ == "__main__":
    feature_word = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw3/Text_Cluster/feature_words.txt"
    stop_word = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw3/Text_Cluster/stop_words.txt"
    text_file_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw3/Text_Cluster/new_weibo_13638"
    stop_word_list = textdata.read_words(stop_word)
    feature_word_list = textdata.read_words(feature_word)
    docs_matrix = textdata.get_all_vector(text_file_path, stop_word_list, feature_word_list)
    dataMat = mat(TF_IDF.TF_IDF(docs_matrix))
    myCentroids, clusterAssing = kMeans(dataMat, 9)
    show(dataMat, 9, myCentroids, clusterAssing)
