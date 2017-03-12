# -*- coding: utf-8 -*-
import sys
import math
reload(sys)
sys.setdefaultencoding( "utf-8" )
import logging
import numpy as np
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)

title_content = []
title_name = []
title_id  = []
hashname_id = {}

# 得到 paragraph vector
def get_vector():
    temp_vector = []
    vector_file = open('sentence_vectors.txt', 'r')
    for lines in vector_file.readlines():
        line = lines.strip().split(' ')
        tv = []
        for i in range(1,len(line)):
            tv.append(float(line[i]))
        temp_vector.append(tv)
    vector_file.close()
    name_file = open('paragraph_name.txt', 'r')
    k = 0
    for lines in name_file.readlines():
        line = lines.strip().split('\t')
        title_name.append(line[0])
        title_id.append(line[1])
        hashname_id[line[0]] = k
        k += 1
    name_file.close()
    return np.mat(temp_vector)

def cluster1(methor = 'DBSCAN'):#dbscan
    out = open('d:/code/emotion/dbscan.txt','w')
    from sklearn.cluster import DBSCAN
    p_vector = get_vector()
    p_vector = p_vector[0:10000]
    if methor == 'DBSCAN':
        print 'dbscan start'
        db = DBSCAN(eps=0.7, min_samples=4).fit(p_vector)
        print 'dbscan over'
        labels = db.labels_
        print labels
        cluster_num = max(labels) + 1
        label_name = [[] for i in range(cluster_num)]
        for i in range(len(labels)):
            if labels[i] >= 0:
                label_name[labels[i]].append(title_name[i])
    for i in range(len(labels)):
        out.write(str(i)+' '+str(labels[i])+'\n')
    print cluster_num
    for i in range(len(label_name)):
        print i
        for j in label_name[i]:
            print j

def cluster2():#mini-batch-kmeans
    out = open('d:/code/emotion/mbkm.txt','w')
    from sklearn.cluster import KMeans, MiniBatchKMeans
    p_vector = get_vector()
    print 'mb-kmeans start'
    mbkm = MiniBatchKMeans(n_clusters=4, init='k-means++')
    mbkm.fit(p_vector)
    labels = mbkm.labels_
    print len(labels),' ',len(p_vector)
    for i in range(len(labels)):
        out.write(str(i)+' '+str(labels[i])+'\n')
    print 'mb-kmeans over'

def cluster3():#mean-shsift
    out = open('d:/code/emotion/meanshift.txt','w')
    from sklearn.cluster import MeanShift, estimate_bandwidth
    p_vector = get_vector()
    print 'mean-shift start'
    bandwidth = estimate_bandwidth(p_vector,quantile=0.93,n_samples=25000)
    print 'bandwidth ',bandwidth
    ms = MeanShift(bandwidth=bandwidth, bin_seeding=True)
    ms.fit(p_vector)
    labels = ms.labels_
    labels_unique = np.unique(labels)
    n_clusters_ = len(labels_unique)
    print len(labels),' ',len(p_vector)
    for i in range(len(labels)):
        out.write(str(i)+' '+str(labels[i])+'\n')
    print 'mean-shift over. N_Cluster: ',n_clusters_

def cluster4():#Agglomerative
    out = open('d:/code/emotion/agglomerative.txt','w')
    from sklearn.cluster import AgglomerativeClustering
    p_vector = get_vector()
    p_vector=p_vector[0:5000]
    print 'Agglomerative start'
    aglm=AgglomerativeClustering(n_clusters=6)
    aglm.fit_predict(p_vector)
    labels = aglm.labels_
    cluster_num = max(labels) + 1
    print len(labels),' ',len(p_vector)
    for i in range(len(labels)):
        out.write(str(i)+' '+str(labels[i])+'\n')
    print 'Agglomerative over'
    label_name = [[] for i in range(cluster_num)]
    for i in range(len(labels)):
        if labels[i] >= 0:
            label_name[labels[i]].append(title_name[i])
    for i in range(len(label_name)):
        for j in label_name[i]:
            print i,' ',j

def cluster5():
    pass

def error(method,n,y1,y2):
    out=open('paint.csv','w')
    logerr=0
    experr=0
    partexperr=0
    partlogerr=0
    for i in range(n):
        experr+=((math.exp(y1[i])-math.exp(y2[i][0]))**2)
        logerr+=((y1[i]-y2[i][0])**2)
        partlogerr+=abs((y1[i]-y2[i][0])/y2[i][0])
        partexperr+=abs((math.exp(y1[i])-math.exp(y2[i][0]))/math.exp(y2[i][0]))
        out.write(str(y1[i]))
        out.write(',')
        out.write(str(y2[i][0]))
        out.write('\n')
    print '------------------------------------------------------------'
    print '--------->',method,' EXP RMSE: ',math.sqrt(experr/n)
    print '--------->',method,' LOG RMSE: ',math.sqrt(logerr/n)
    print '--------->',method,' EXP MAPE: ',math.sqrt(partexperr/n)*100,'%'
    print '--------->',method,' LOG MAPE: ',math.sqrt(partlogerr/n)*100,'%'
    print '------------------------------------------------------------\n'

def Regression():
    from sklearn import linear_model
    from sklearn.linear_model import LinearRegression
    from sklearn.svm import SVR,NuSVR

    vx=[]
    vy=[]
    mask=[
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        #0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1
    ]

    #inx=open('D:/code/pdata/REG/structemo/regX.pdt')
    inx=open('regX.pdt')
    for line in inx:
        cnt=0
        lv = line.strip().split(' ')
        tv=[]
        for i in range(len(lv)):
            if mask[cnt]==1:
                tv.append(float(lv[i]))
            cnt+=1
        tv.append(float(1))
        vx.append(tv)
    inx.close()

    #iny=open('D:/code/pdata/REG/structemo/regY.pdt')
    iny=open('regY.pdt')
    for line in iny:
        cnt=0
        lv = line.strip().split(' ')
        tv=[]
        for i in range(len(lv)):
            tv.append(float(lv[i]))
        vy.append(tv)
    iny.close()

    NTest=int(len(vx)*0.3)
    print 'ALL: ',len(vx),'TRAIN: ',len(vx)-NTest,'TEST: ',NTest
    trainx=vx[:-NTest]
    trainy=vy[:-NTest]
    testx=vx[-NTest:]
    testy=vy[-NTest:]

    #reg=SVR(C=7,epsilon=1.89,gamma='auto',kernel='rbf')
    reg=SVR(C=3,epsilon=0.0000009,gamma='auto',kernel='rbf')
    y = reg.fit(trainx, trainy).predict(testx)
    error('SVR rbf',NTest,y,testy)

    reg=LinearRegression()
    y=reg.fit(trainx, trainy).predict(testx)
    #error('Liner ',NTest,y,testy)
if __name__ == '__main__':
    #cluster1()
    #cluster2()
    #cluster3()
    #cluster4()
    Regression()
    #trans()
