import sys
import os
import sklearn
import pickle
import _pickle
import pickletools
from sklearn.datasets.base import Bunch


def savefile(savepath, content):
    with open(savepath, "wb") as fp:
        fp.write(content)


def _readfile(path):
    with open(path, "rb") as fp:
        content = fp.read()
    return content

def corpus2Bunch(wordbag_path, seg_path):
    catelist = os.listdir(seg_path)  # 获取seg_path下的所有子目录，也就是分类信息
    #创建一个Bunch实例
    bunch = Bunch(target_name=[], label=[], filenames=[], contents=[])
    bunch.target_name.extend(catelist)

    for category in catelist:       #对每一个种类文件夹
        class_path = seg_path + category + "/"  # 拼出分类子目录的路径
        file_list = os.listdir(class_path)  # 获取class_path下的所有文件
        for file in file_list:  # 遍历类别目录下文件
            fullname = class_path + file  # 拼出文件名全路径
            bunch.label.append(category)
            bunch.filenames.append(fullname)
            bunch.contents.append(_readfile(fullname))

    with open(wordbag_path, "wb") as file_obj:
        pickle.dump(bunch, file_obj)


if __name__ == "__main__":
    wordbag_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Train/train_set.dat"
    seg_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/TrainSet/"
    corpus2Bunch(wordbag_path, seg_path)

    wordbag_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test/test_set.dat"
    seg_path = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw1/new_weibo_13638/Test_Set/"
    corpus2Bunch(wordbag_path, seg_path)