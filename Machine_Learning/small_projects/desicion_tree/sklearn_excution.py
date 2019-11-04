import os
import pickle
from sklearn.datasets.base import Bunch
from sklearn import preprocessing
from sklearn import tree
from sklearn import metrics


def _readbunchobj(path):
    with open(path, "rb") as fileobj:
        bunch = pickle.load(fileobj)
    return bunch


trainpath = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Train/tfidfspace.dat"
train_set = _readbunchobj(trainpath)

testpath = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test/testspace.dat"
test_set = _readbunchobj(testpath)

clf = tree.DecisionTreeClassifier(criterion="entropy").fit(train_set.tdm, train_set.label)

test_prediction = clf.predict(test_set.tdm)

n = 0
correct = 0
for flabel, file_name, expct_cate in zip(test_set.label, test_set.filenames, test_prediction):
    if flabel == expct_cate:
        correct += 1
    n += 1
    print("Actual:", flabel, "---- Predict:", expct_cate)

print('Accuracy:', correct/n)