#!/usr/bin/env Python
#coding=utf-8

import os
import re
import sys


def process(path):
    catelist = os.listdir(path)  # 获取seg_path下的所有子目录，也就是分类信息
    for category in catelist:   # 遍历每一个种类文件夹
        dict = {}  # 为每一个种类文件夹都创建一个空字典
        class_path = path + category + "/"  # 拼出分类子目录的路径
        file_list = os.listdir(class_path)  # 获取class_path下的所有文件
        for file in file_list:  # 遍历类别目录下文件
            fullname = class_path + file
            with open(fullname, mode='r', encoding='utf-8') as inFile:
                word = ''  # 空字符串以便于连接字符
                for char in inFile.read():
                    if char != '\t': # 工作原理: 一个字符一个字符地读, 将不是制表符的字符丢进 word 字符串当中
                        word += char  # 连接字符
                    else:   # 读到了制表符, 就从 word 字符串当中读取一个单词, 进行词频统计!
                        if word in dict:
                            dict[word] += 1
                            word = ''  # 将word置为空，否则，word值无限增大
                        else:
                            dict.setdefault(word, 1)
                            word = ''

        wordfreq = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/" + category + ".txt"
        with open(wordfreq, mode='w', encoding='utf-8') as outFile:
            for word, freq in dict.items():
                s = '{0}\t{1}\n'.format(word, freq)
                outFile.write(s)


if __name__ == "__main__":
    path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Train_Set/"
    process(path)
