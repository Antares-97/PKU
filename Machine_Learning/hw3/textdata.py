import os
import re
import sys
import codecs
import numpy as np
import string


def read_words(file_name):
    with open(file_name, mode="r", encoding="utf8", errors='ignore') as file:
        words = file.read()
    dictionary = []
    word = ''
    for char in words:
        # print(word)
        if char != '\t':
            word += char
        else:
            dictionary.append(word)
            # print(word)
            word = ''

    return dictionary


def del_stop_words(feature_words, stop_words):

    newwordsfile = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw3/Text_Cluster/new_words.txt"
    # stop_words = read_words(stop_word_file)
    # feature_words = read_words(feature_word_file)
    new_words = []
    for r in feature_words:
        if r not in stop_words:
            new_words.append(r)
            # print(r)
    return new_words


def get_all_vector(file_path, stop_words_set, feature_word_set):
    names = [ file_path +'/'+ f for f in os.listdir(file_path) ]
    posts = []      # posts里头是没有分割成一个个单词的原句
    for name in names:
        temp_post = read_words(name)
        posts.append(temp_post)
    docs = []
    # word_set = set()
    for post in posts:
        doc = del_stop_words(post, stop_words_set)
        docs.append(doc)
        # word_set = set(doc)
        #print len(doc),len(word_set)

    # word_set = list(word_set)
    docs_vsm = []

    #for word in word_set[:30]:
        #print word.encode("utf-8"),
    # feature_word_set = read_words(feature_word_path)

    for doc in docs:
        temp_vector = []
        dict = {}
        for word in feature_word_set:
            dict.setdefault(word, 0)

        for word in doc:    # 对一篇文档中的所有词进行遍历
            if word in feature_word_set:    # 如果该词出现于特征词集合中
                dict[word] += 1

        for word in feature_word_set:
            temp_vector.append(dict[word] * 1.0)
        #print temp_vector[-30:-1]
        docs_vsm.append(temp_vector)

    docs_matrix = np.array(docs_vsm)
    return docs_matrix
