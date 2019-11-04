import text_data
from sklearn.feature_extraction import text
from sklearn.decomposition import TruncatedSVD
import calc_score
import numpy as np
from sklearn import metrics
from scipy import spatial
from matplotlib import pyplot as plt
import My_Cluster


# 为了防止全零行计算cosine affinity 出错补上一个标志
def mark_allzeros(matrix):
    mark = np.prod(matrix == 0, axis=1)
    return np.concatenate((matrix, mark.reshape((len(mark), 1))), axis=1)


IDF = True
MIN_Freq = 100
Clusters_N = 9
Components_N = 200

data = text_data.load_data(type="str")
countVectorizer = text.TfidfVectorizer(input='content', min_df=MIN_Freq, use_idf=IDF)  # 最少总共出现过min_df次
term_freq = countVectorizer.fit_transform(data)
print('词典总词数:', len(countVectorizer.vocabulary_))
# print(countVectorizer.vocabulary_)

svd = TruncatedSVD(n_components=Components_N)
svd_result = svd.fit_transform(term_freq)
print("Explained variance of the SVD step: {}%".format(int(svd.explained_variance_ratio_.sum() * 100)))

marked_svd = mark_allzeros(svd_result)
Dist = spatial.distance.pdist(marked_svd, metric="cosine")
my_cluster = My_Cluster.My_Cluster(Dist)
my_pred = My_Cluster.My_Predict(np.array(my_cluster, dtype=np.int32)[:, :2], n_clusters=9)
print(my_cluster)
print(my_pred)

for labels_pred in [my_pred]:
    count_table = calc_score.count_table(text_data.init_num_by_cls, labels_pred, Clusters_N)
    print(count_table)
    total_entropy = calc_score.total_entropy(count_table)
    print("Total Entropy:", total_entropy)
    print("homogeneity_score", metrics.homogeneity_completeness_v_measure(text_data.labels_true(), labels_pred))

plt.show()
