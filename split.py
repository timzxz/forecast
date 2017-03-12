
#encoding=utf-8
import sys
import math
import jieba
import codecs
from numpy import mat
#sys.setdefaultencoding( "utf-8" )
CallCnt=0
wordlist=[]
wordset={}
NWord=12581
NClass=2#0,1
WordP0=[]
WordP1=[]
ClsP=[0.0]*NClass
CmtCls=open("D:/code/pdata/pCmtCls.pdt",'w')
def BltWrdLst():
    #buile wordlist
    trainfile=codecs.open("d:/code/EmotionCorpus/libinbinxin0.txt")
    dict={}
    cnt=0
    for line in trainfile:
        cnt+=1
        tmp=line.split('----->')
        list=jieba.cut(tmp[0])
        for i in list:
            dict[i]=1
    print "train doc num: ",cnt
    nword=len(dict)
    print "train word num(in original dict): ",nword
    for i in dict:
        wordlist.append(i)
def bltWrdSet():
    for i,word in enumerate(wordlist):
        wordset[word]=i
    print "train word num(in word dict): ",len(wordset)
def ReadP():
    pfile=open("D:/code/pdata/pNB.pdt")
    cnt=0
    for line in pfile:
        tmp=line.split(' ')
        if cnt==0:
            ClsP[0]=float(tmp[0])
            print cnt,ClsP[0]
            ClsP[1]=float(tmp[1])
            print cnt,ClsP[1]
            cnt+=1
            continue
        elif cnt==1:
            for i in range(NWord):
                WordP0.append(float(tmp[i]))
            cnt+=1
            continue
        elif cnt==2:
            for i in range(NWord):
                WordP1.append(float(tmp[i]))
            cnt+=1
            continue
    print len(WordP0),len(WordP1)

def realpart():
    cmt=codecs.open("D:\code\json\commentitem.json",encoding='utf-8')
    cmtout=codecs.open("cmtout",'w',encoding='utf-8')
    cnt=0
    pstart=pend=0
    buf=""
    print "real part:\n"
    for line in cmt:
        pstart=line.find("{\"_id\":\"")
        if pstart!=-1:
            cid=line[pstart+8:pstart+27]
            pstart=line.find("\"comments\":\"")
            #cmtout.write(cid)
            #print cid
            pend=line.find("\",\"user\"")
            if pstart!=-1 and pend!=-1:#in the same line
                buf=line[pstart+12:pend]
                #print buf
                #CalcCls(cid,buf)
                #CalcCls(cid,buf)
                CmtCls.write(cid)
                CmtCls.write(" ")

                list=jieba.cut(buf)
                for i in list:
                    if i in wordset:
                        CmtCls.write(str(wordset[i]))
                        CmtCls.write(" ")
                CmtCls.write("\n")

                #print cid,'\n',buf
                buf=""
                cnt+=1
        if cnt%1000==0:
            print cnt,
    print cnt
def samplepart():
    #train
    trainfile=codecs.open("d:/code/EmotionCorpus/libinbinxin0.txt")
    dict={}
    cnt=0
    for line in trainfile:
        cnt+=1
        tmp=line.split('----->')
        list=jieba.cut(tmp[0])
        for i in list:
            dict[i]=1
    print cnt
    nword=len(dict)
    print nword
    wordlist=[]
    for i in dict:
        wordlist.append(i)

    #test
    testfile=codecs.open("d:/code/EmotionCorpus/libinbinxin_test_mengniu0.txt")
    testpart=codecs.open("testpart",'w',encoding='utf-8')
    testcls=codecs.open("testcls",'w',encoding='utf-8')
    cnt=0
    for line in testfile:
        tmplist=[0]*nword
        tmp=line.split('----->')
        list=jieba.cut(tmp[0])
        for i in list:
            for j in range(nword):
                if i==wordlist[j]:
                    tmplist[j]=1
                    break
        for i in tmplist:
            testpart.write(str(i))
            testpart.write(' ')
        testpart.write('\n')
        testcls.write(str(tmp[1]))
        cnt+=1
        print cnt,
if __name__ == '__main__':
    #samplepart()
    #BltWrdLst()
    #bltWrdSet()
    #ReadP()
    #realpart()
    list=jieba.cut('结巴可以对一段中文进行分词，有三种模式，可以适应不同的需求。')
    for i in list:
        print i,
    trainfile=codecs.open("d:/code/EmotionCorpus/libinbinxin0.txt")
    part=codecs.open("part",'w',encoding='utf-8')
    cls=codecs.open("cls",'w',encoding='utf-8')
    cnt=0
    for line in trainfile:
        tmplist=[0]*nword
        tmp=line.split('----->')
        list=jieba.cut(tmp[0])
        for i in list:
            for j in range(nword):
                if i==wordlist[j]:
                    tmplist[j]=1
                    break
        for i in tmplist:
            part.write(str(i))
            part.write(' ')
        part.write('\n')
        cls.write(str(tmp[1]))
        cnt+=1
        print cnt,
#'''
'''
def realpart():
    cmt=codecs.open("D:\code\json\commentitem.json",encoding='utf-8')
    cmtout=codecs.open("cmtout",'w',encoding='utf-8')
    state=0
    cnt=0
    pstart=pend=pstartline=pendline=0
    buf=""
    print "real part:\n"
    for line in cmt:
        if state==2:
            if line.find("{\"_id\":\""):
                state=0
        if state==0:
            pstart=line.find("\"comments\":\"")
            cid=line[pstart-56:pstart-37]
            #cmtout.write(cid)
            #print cid
            pend=line.find("\",\"user\"")
            if pstart!=-1 and pend!=-1:#in the same line
                buf=line[pstart+12:pend]
                #print buf

                #CalcCls(cid,buf)
                #CalcCls(cid,buf)
                CmtCls.write(cid)
                CmtCls.write(" ")
                list=jieba.cut(buf)
                for i in list:
                    if i in wordlist:
                        CmtCls.write(str(wordlist.index(i,0,NWord)))
                        CmtCls.write(" ")
                CmtCls.write("\n")

                buf=""
                cnt+=1
                state=0
            elif pstart!=-1 and pend==-1:#not in the same line
                buf=line[pstart+12:]
                state=1
        if state==1:
            pend=line.find("\",\"user\"")
            if p!=-1:#current line is the end
                buf+=line[pend:]
                #CalcCls(cid,buf)
                #cmtout.write(buf)
                CmtCls.write(cid)
                CmtCls.write(" ")
                list=jieba.cut(buf)
                for i in list:
                    if i in wordlist:
                        CmtCls.write(str(wordlist.index(i,0,NWord)))
                        CmtCls.write(" ")
                CmtCls.write("\n")

                print buf
                buf=""
                cnt+=1
                state=2
            else:#current line is not the end
                buf+=line
                state=1
        if cnt%1000==0:
            print cnt,
#'''
'''
def CalcCls(cid,cmt):#NBClassify
    global CallCnt
    CallCnt+=1
    #calc vector
    tmplist=[0]*NWord
    list=jieba.cut(cmt)
    for i in list:
        for j in range(NWord):
            if i==wordlist[j]:
                tmplist[j]=1
                break
    #calc VecMul
    p0=mat(tmplist)*(mat(WordP0).T)+math.log(ClsP[0])
    p1=mat(tmplist)*(mat(WordP1).T)+math.log(ClsP[1])
    if p0>p1:
        CmtCls.write(cid)
        CmtCls.write(" 0\n")
    else:
        CmtCls.write(cid)
        CmtCls.write(" 1\n")
    if CallCnt%500==0:
        print CallCnt,
#'''
