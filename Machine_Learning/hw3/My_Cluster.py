import numpy as np
from tqdm import tqdm


def matrix_to_array(n, i, j):
    return np.int32(n*(n-1)/2 - (n-i)*(n-i-1)/2 + j - i - 1)


def index_matrix(d, i):
    b = 1 - 2 * d
    x = int(np.floor((-b - np.sqrt(b**2 - 8*i))/2))
    y = int(i + x*(b + x + 2)/2 + 1)
    return x, y


def My_Cluster(Dist, linkage="average"):
    n_init = int(np.ceil(np.sqrt(len(Dist)*2)))
    n_clusters = np.ones((n_init,), dtype=np.int32)
    n = len(n_clusters)  # 聚类个数n
    record = []
    for i_th in tqdm(range(n-2)):       # 迭代
        max_indice = Dist.argmin()
        i, j = index_matrix(n, max_indice)
        n_i, n_j = n_clusters[i], n_clusters[j]
        n_clusters[i] = n_tot = n_i+n_j
        n_clusters = np.delete(n_clusters, j)
        record.append([i, j, Dist[max_indice], n_tot])
        Dist_old = np.copy(Dist)
        Dist = np.delete(Dist, matrix_to_array(n, j, np.arange(n - 1, j, -1)))
        Dist = np.delete(Dist, matrix_to_array(n, np.arange(j - 1, -1, -1), j))
        k = np.arange(i)
        Dist[matrix_to_array(n - 1, k, i)] = (n_i * Dist_old[matrix_to_array(n, k, i)] + n_j * Dist_old[matrix_to_array(n, k, j)]) / n_tot
        k = np.arange(i+1, j)
        Dist[matrix_to_array(n - 1, i, k)] = (n_i * Dist_old[matrix_to_array(n, i, k)] + n_j * Dist_old[matrix_to_array(n, k, j)]) / n_tot
        k = np.arange(j+1, n)
        Dist[matrix_to_array(n - 1, i, k - 1)] = (n_i * Dist_old[matrix_to_array(n, i, k)] + n_j * Dist_old[matrix_to_array(n, j, k)]) / n_tot
        n = len(n_clusters)
    # n=2 时
    record.append([0, 1, Dist[0], n_clusters[0]+n_clusters[1]])
    return record


def My_Predict(record, n_clusters, re="predict"):
    n = record.shape[0]+1
    clusters = [[i] for i in range(n)]
    for i_th in range(n-n_clusters):
        clusters[record[i_th, 0]].extend(clusters[record[i_th, 1]])
        del clusters[record[i_th, 1]]
    if re == "clusters": return clusters
    else:
        pred = np.zeros(n, dtype=int)
        for i, clt in enumerate(clusters):
            pred[clt] = np.array([i]*len(clt))
        if re == "predict": return pred
    raise ValueError("re can only be ['clusters','predict','both']")
