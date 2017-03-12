#encoding=utf-8
import sys
import math
import jieba
import codecs
from numpy import mat
def getqingdao():
    cmt=codecs.open("D:\code\json\commentitem.json",encoding='utf-8')
    out=codecs.open("qingdao",'w',encoding='utf-8')
    cnt=0
    pstart=pend=0
    buf=""
    print "real part:\n"
    for line in cmt:
        pstart=line.find("{\"_id\":\"")
        if pstart!=-1:
            cid=line[pstart+8:pstart+27]
            pstart=line.find("title_id")
            if pstart!=-1:
                tid=line[pstart+11:pstart+21]
                #print tid
                if tid=='1220103936':
                    pstart=line.find("\"comments\":\"")
                    pend=line.find("\",\"user\"")
                    if pstart!=-1 and pend!=-1:#in the same line
                        buf=line[pstart+12:pend]
                        #print cid,'\n',buf
                        out.write(buf)
                        out.write('\n')
                        buf=""
            cnt+=1
        if cnt%10000==0:
            print cnt,
    print cnt
def have(cmt,word):
    if cmt.find(word)!=-1:
        return true
    return False
def paint():
    cnt1=cnt2=cnt3=0
    last1=last2=last3=0
    cmt=codecs.open("qingdao",encoding='utf-8')
    keyword=codecs.open("keyword.txt",encoding='utf-8')
    paint=open('qingdaomove.csv','w')
    key1=key2=key3=[]
    cnt=0
    for line in keyword:
        if cnt==0:
            key1=line.split(' ')
            cnt+=1
            continue
        if cnt==1:
            key2=line.split(' ')
            cnt+=1
            continue
        if cnt==2:
            key3=line.split(' ')
            cnt+=1
            continue
    print key1,key2,key3
    cnt=0
    for line in cmt:
        cnt+=1
        if cnt%750==0:
            paint.write(str(cnt1-last1))
            paint.write(',')
            paint.write(str(cnt2-last2))
            paint.write(',')
            paint.write(str(cnt3-last3))
            paint.write('\n')
            last1=cnt1
            last2=cnt2
            last3=cnt3
        for i in key1:
            if line.find(i)!=-1:
                cnt1+=1
                break
        for i in key2:
            if line.find(i)!=-1:
                cnt2+=1
                break
        for i in key3:
            if line.find(i)!=-1:
                cnt3+=1
                break
    print cnt1,cnt2,cnt3
if __name__ == '__main__':
    #realpart()
    paint()
