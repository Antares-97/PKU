import numpy as np
import collections
from scipy import stats

def count_table(classes, cluster_labels, Clusters_N):
    Classes_N = len(classes)
    table = np.zeros((Classes_N, Clusters_N), dtype=int)
    last = 0
    for i in range(Classes_N):
        class_count = collections.Counter(cluster_labels[last:last+classes[i]])
        last += classes[i]
        table[i] = np.array([class_count[j] for j in range(Clusters_N)])
    return table


def total_entropy(count_table):
    n_j = np.sum(count_table, axis=0)
    entropy_j = np.array([stats.entropy(count_table[:, j]) for j in range(count_table.shape[1])])
    return sum(n_j * entropy_j) / sum(n_j)
