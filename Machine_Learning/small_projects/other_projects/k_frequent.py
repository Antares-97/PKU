def dict2list(dic:dict):
    keys = dic.keys()
    vals = dic.values()
    lst = [(key, val) for key, val in zip(keys, vals)]
    return lst


def Kfrequent(self, LIST, k:int):
    dic = {}
    for num in LIST:
        dic[num] = dic.get(num, 0) + 1

    listDic = sorted(dict2list(dic), key=lambda d: d[1], reverse=True)

    res = []
    for i in range(k):
        res += listDic[i][0],
    return res

