import os
import pickle
import sklearn
from sklearn.datasets.base import Bunch
from sklearn.feature_extraction.text import TfidfVectorizer


def _readfile(path):
    with open(path, "rb") as fp:
        content = fp.read()
    return content


def _readbunchobj(path):
    with open(path, "rb") as file_obj:
        bunch = pickle.load(file_obj)
    return bunch

def _writebunchobj(path, bunchobj):
    with open(path, "wb") as fileobj:
        pickle.dump(bunchobj, fileobj)


def vector_space(stopword_path, bunch_path, space_path, train_tfidf_path):
    stpwrdlist = _readfile(stopword_path).splitlines()
    bunch = _readbunchobj(bunch_path)
    TFidfspace = Bunch(target_name=bunch.target_name, label=bunch.label, filenames=bunch.filenames, tdm=[], vocabulary={})

    trainbunch = _readbunchobj(train_tfidf_path)
    TFidfspace.vocabulary = trainbunch.vocabulary

    vectorizer = TfidfVectorizer(stop_words=stpwrdlist, sublinear_tf=True, max_df=0.5, vocabulary=trainbunch.vocabulary)
    TFidfspace.tdm = vectorizer.fit_transform(bunch.contents)
    _writebunchobj(space_path, TFidfspace)


if __name__ == '__main__':
    stopword_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/stopwords.txt" #停用词表的路径
    bunch_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test/test_set.dat"  #导入训练集Bunch的路径
    space_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test/testspace.dat"  # 词向量空间保存路径
    train_tfidf_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Train/tfidfspace.dat"
    vector_space(stopword_path, bunch_path, space_path, train_tfidf_path)

