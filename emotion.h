#pragma once
#ifndef _EMOTION_H
#define _EMOTION_H
#include "use.h"
#include "model.h"
#include "fixtime.h"
using namespace Tree;
vector<double> ClsP(NClass, 0.0), vt(NWord, 0.0);
vector<vector<double>> WordP(NClass, vt);

template<int ndoc, int nword>
tuple<vector<vector<int>>, vector<int>, vector<vector<int>>, vector<int>>
input()//for test
{
	ifstream inpart("D:/STUDY/STUDY_NOW/python/part");
	ifstream incls("D:/STUDY/STUDY_NOW/python/cls");
	ifstream intestpart("D:/STUDY/STUDY_NOW/python/testpart");
	ifstream intestcls("D:/STUDY/STUDY_NOW/python/testcls");
	string tmp;
	vector<int> vt(nword, 0), retcls(ndoc, 0), rettestout;
	vector<vector<int>> retmat(ndoc, vt), rettestmat;
	cout << "read train begin" << endl;
	/*
	for (auto i : Range(ndoc))
	if (!(inpart.eof() || incls.eof()))
	{
	for (auto j : Range(nword))
	{
	inpart >> tmp;
	retmat[i][j] = StoN(tmp);
	}
	incls >> tmp;
	auto t(StoN(tmp));
	if (t == -1)
	retcls[i] = 0;
	else
	retcls[i] = 1;
	QuickPrint<15>(i);
	}
	inpart.close(); incls.close();
	//*/
	ClrLn("read train end\nread test begin");
	int cnt(0);
	while (!(intestpart.eof() || intestcls.eof()))
	{
		QuickPrint<10>(cnt++);
		for (auto i : Range(nword))
		{
			intestpart >> tmp;
			vt[i] = StoN(tmp);
		}
		rettestmat.push_back(vt);
		intestcls >> tmp;
		rettestout.push_back(StoN(tmp));
	}
	ClrLn("read test end");
	return make_tuple(retmat, retcls, rettestmat, rettestout);
};

template<int NDoc, int NWord, int NClass >//class transfered to 0,1,2,...,(NClass-1)
tuple<vector<vector<double>>, vector<double>>
NBTrain(vector<vector<int>>& Mat, vector<int>& Cls)
{
	vector<int> ClsCnt(NClass, 0), vti(NWord, 1);
	vector<double> ClsP(NClass, 0.0), vtd(NWord, 0.0);
	vector<vector<int>> VecS(NClass, vti);
	vector<vector<double>> Ret(NClass, vtd);
	vector<int> WordS(NClass, 0);

	cout << "train" << endl;
	for (auto i : Cls)
		ClsCnt[i] += 1;

	for (auto i : Range(NClass))
	{
		ClsP[i] = ddiv(ClsCnt[i], NDoc);
		ClsCnt[i] = 2;//!!!!!careful
	}

	for (auto i : Range(NDoc))
	{
		auto index(Cls[i]);
		VecAdd<NWord>(VecS[index], Mat[i]);
		ClsCnt[index] += accumulate(Mat[i].cbegin(), Mat[i].cend(), 0);
	}

	for (auto i : Range(NClass))
		for (auto j : Range(NWord))
			Ret[i][j] = log(ddiv(VecS[i][j], ClsCnt[i]));
	return make_tuple(Ret, ClsP);
};

template<int NWord, int NClass >
void NBWrite(vector<vector<double>> WordP, vector<double> ClsP)
{
	ofstream out("D:/code/pdata/pNB.pdt");
	for (auto i : ClsP)
		out << i << ' ';
	out << endl;
	for (auto i : WordP)
	{
		for (auto j : i)
			out << j << ' ';
		out << endl;
	}
};

template<int NWord, int NClass >
void NBRead()
{
	ifstream in("D:/code/pdata/pNB.pdt");
	string tmp;
	cout << "NBRead begin" << endl;
	for (auto i : Range(NClass))
	{
		in >> tmp;
		ClsP[i] = StoN<double>(tmp);
	}
	for (auto i : Range(NClass))
		for (auto j : Range(NWord))
		{
			in >> tmp;
			WordP[i][j] = StoN<double>(tmp);
		}
	cout << "NBRead end" << endl;
};

template<int nword = NWord, int nclass = NClass>//maybe deprecated
inline int NBClassify(vector<int>& testvec)
{
	int MaxValue, MaxIndex;
	vector<double> tmp(nclass, 0.0);
	//cout << "classify" << endl;
	MaxValue = (-2 << 30);
	for (auto i : Range(NClass))
	{
		tmp[i] = VecMul<NWord, int, double>(testvec, WordP[i]) + log(ClsP[i]);
		if (tmp[i]>MaxValue)
		{
			MaxValue = tmp[i];
			MaxIndex = i;
		}
	}
	//cout<<MaxIndex<<' ';
	return MaxIndex;
};

template<int NDoc, int NWord, int NClass >//maybe deprecated
vector<int> NBClassify(vector<vector<double>>& TrainWordP, vector<double>& TrainClsP, vector<vector<int>>& TestMat)
{
	int NTest(TestMat.size()), MaxValue, MaxIndex;
	vector<double> tmp(NClass, 0.0);
	vector<int> ret(NTest, -1);
	cout << "classify" << endl;
	for (auto i : Range(NTest))
	{
		VecZero<NWord>(tmp);
		MaxValue = (-2 << 30);
		for (auto j : Range(NClass))
		{
			tmp[j] = VecMul<NWord, int, double>(TestMat[i], TrainWordP[j]) + log(TrainClsP[j]);
			cout << tmp[j] << endl;
			if (tmp[j]>MaxValue)
			{
				MaxValue = tmp[j];
				MaxIndex = j;
			}

		}
		ret[i] = MaxIndex;
		//cout<<MaxIndex<<endl;
	}
	cout << "classify over" << endl;
	return ret;
};
void CalcError(vector<int>& est, vector<int>& act)
{
	int estl(est.size());
	int actl(act.size());
	if (actl == estl)
	{
		int crtcnt(0);
		for (auto i : Range(actl))
			if ((act[i] == 1 && est[i] == 1) || (act[i] == -1 && est[i] == 0))
				crtcnt++;
		cout << crtcnt << '/' << actl << " = " << (static_cast<double>(crtcnt) / static_cast<double>(actl));
	}
};


void PreAnalyse(string tid=SpecTid)
{
	ifstream incls("d:/code/pdata/pCmtCls.pdt");
	map<string,int> tmp;
	string outname("p");
	outname+=tid;
	outname+=".pdt";
	ofstream out(outname);
	auto c(CTN[tid].Cmts);
	out<<tid<<endl;
	if (incls.is_open())
	{
		string emp(""), cid(""), last(""), tmp("");/*last cid*/
		int cnt(0);
		vector<int> tmpvec;
		cout << "PreAnalyse Start" << endl;
		while (!incls.eof())
		{
			incls >> tmp;
			auto l(tmp.length());
			if (l == 19)
			{
				last = cid;
				cid = tmp;
				if (last != emp)
				{
					if(c.find(last)!=c.end())
					{
						auto l(tmpvec.size());
						out<<last<<' '
							<<c[last]->Time<<' '
							<<l<<' ';
						for(auto i:Range(l))
							out<<tmpvec[i]<<' ';
						out<<endl;
					}
					tmpvec.clear();
					cnt++;
					QuickPrint<2000>(cnt);
				}
			}
			else
				tmpvec.push_back(StoN(tmp));
		}
		ClrLn("PreAnalyse Over");
	}

};

void Analyse(int chkp=(2<<12),string tid=SpecTid)
{
/*
	NBRead<12581, 2>();
	for(auto i:Range(NClass))
	{
		cout<<"CLASS "<<i<<endl;
		vector<double> tmp(WordP[i]);
		sort(tmp.begin(),tmp.end(),[](double a,double b){return a>b;});
		for(auto j:Range(TopDem))
		{
			//cout<<(find(WordP[i].begin(),WordP[i].end(),tmp[j])-WordP[i].begin());
			cout<<(find(WordP[i].rbegin(),WordP[i].rend(),tmp[j])-WordP[i].begin())<<": "<<tmp[j]<<endl;
		}
		cout<<endl<<endl;
	}
//*/
/*
CLASS 0
10751: -3.32574
4753: -3.48514
9283: -3.54888
3287: -3.6147
2347: -3.84011
11847: -3.88292
4405: -3.8926
10553: -3.8941
8095: -3.97689
4322: -4.05918
10351: -4.06006
9378: -4.16248

CLASS 1
10751: -3.17637
4753: -3.20864
9283: -3.53779
11315: -3.78473
4322: -3.9499
10351: -3.9499
311: -4.33364
3287: -4.41416
7059: -4.45295
8791: -4.57013
6404: -4.58199
7859: -4.67327
*/
	//man set
	vector<int> diff({
		527,8095,11847,4405, 2347,10553, 11888,1162,9378, 7059,11179,2167,//class 0
		9333,6607,6379, 10598,8791,7859, 311,2590,5340, 8037,3731,7059//class 1
	});
	vector<int> dorder({
		4,18,1, 0,2,3, 9,20,5, 6,13,19,
		21,16,14, 23,10,11, 7,12,15, 17,22,8
	});
	vector<int> dordidx({
		3,2,4, 5,0,8, 9,18,23, 6,16,17,
		19,10,14, 20,13,21, 1,11,7, 12,22,15
	});

	vector<int> common({4322,6404,3287,4753,10751,9283,11315,10351});
	vector<int> corder({5,7,3,1,0,2,4,6});
	vector<int> cordidx({4,3,5,2,6,0,7,1});

	auto cbeg(common.begin());
	auto cend(common.end());
	auto clen(common.size());


	auto dbeg(diff.begin());
	auto dend(diff.end());
	auto dlen(diff.size());

	vector<int> vt(dlen,0);
	vector<vector<int>> record(clen,vt);
	string inname("p");
	inname+=tid;
	string outname(inname);
	inname+=".pdt";
	ifstream in(inname);

	outname+="out";
	outname+=ItoS(chkp);
	outname+=".csv";
	ofstream out(outname);
	if(in.is_open())
	{
		string tmp;
		in>>tmp;//tid
		in>>tmp;//ttime
		(*tt)(tmp);
		int ttime((*tt)());
		while(!in.eof())
		{
			int cidx(-1),didx(-1);
            vector<int> dmark(dlen,0);
            int dmarkcnt(0);
            vector<int> cmark(clen,0);
            int cmarkcnt(0);
			in>>tmp;//cid
			in>>tmp;//ctime
			(*ct)(tmp);
			int ctime((*ct)());
			in>>tmp;//n_1_Feature
			if(ctime<=ttime+chkp)
			{
				for(auto i:Range(StoN(tmp)))
				{
					in>>tmp;
					auto cpos(find(cbeg,cend,StoN(tmp))-cbeg);
					if(cpos!=clen&&cmark[cpos]==0)
					{
						cmark[cpos]=1;
						cmarkcnt++;
						continue;
					}
					auto dpos(find(dbeg,dend,StoN(tmp))-dbeg);
					if(dpos!=dlen&&dmark[dpos]==0)
					{
						dmark[dpos]=1;
						dmarkcnt++;
					}
				}
				if(dmarkcnt>0&&cmarkcnt>0)
				{
					for(auto i:Range(clen))
						if(cmark[cordidx[i]]==1)
						{
							cidx=cordidx[i];
							break;
						}
					for(auto i:Range(dlen))
						if(dmark[dordidx[i]]==1)
						{
							didx=dordidx[i];
							break;
						}
					record[cidx][didx]+=1;
				}
			}
			else
				for(auto i:Range(StoN(tmp)))
					in>>tmp;
		}
	}
	cout<<tid<<' '<<chkp<<endl;
	/*
	for(auto i:Range(clen))
	{
		for(auto j:Range(dlen))
			if(record[i][j]!=0)
				out<<log(static_cast<double>(record[i][j]))<<',';
			else
				out<<0<<',';
		out<<endl;
	}
	*/
	for(auto i:Range(clen))
	{
		for(auto j:Range(dlen))
			if(record[i][j]!=0)
				out<<log(static_cast<double>(record[i][j]))<<',';
			else
				out<<0<<',';
		out<<endl;
	}
};
/*
0 num: 72234  TID: 1220103936  Time: 2015-10-06-01:59
1 num: 52265  TID: 1257588071  Time: 2015-11-29-07:35//
2 num: 51974  TID: 1232422402  Time: 2015-10-24-13:50
3 num: 44212  TID: 1231279705  Time: 2015-10-22-20:03
4 num: 40157  TID: 1298519354  Time: 2016-01-21-03:45
5 num: 36708  TID: 1270794830  Time: 2015-12-16-10:48//
6 num: 35342  TID: 1233462817  Time: 2015-10-26-14:41
7 num: 31663  TID: 1257647568  Time: 2015-11-29-10:00
8 num: 31644  TID: 1215804054  Time: 2015-09-28-04:39
9 num: 27133  TID: 1274158682  Time: 2015-12-19-23:57
*/
template<int front=10>
void ChooseTile()
{
	vector<int> l;
	map<int,string> tmp;
	for(auto p:CTN)
	{
		auto s(p.second.Cmts.size());
		l.push_back(s);
		tmp[s]=p.first;
	}
	sort(l.begin(),l.end(),[](int a,int b){return a>b;});
	for(auto i:Range(front))
		cout<<i<<" num: "<<l[i]
			<<"  TID: "<<tmp[l[i]]
			<<"  Time: "<<CTN[tmp[l[i]]].Time
			<<endl;
};

template<int span=15,int nclass=NClass>//3 minutes
void EmoMove(string tid)
{
	string name("d:/code/pdata/EmoMove/");
	name+=tid;
	name+='P';
	name+=ItoS(CTN[tid].Cmts.size());
	ofstream out(name+"EmMov.csv");
	(*tt)(CTN[tid].Time);
	int ttime((*tt)());
	int tlen(1000);
	int tmps[2]={0,0};
	int len(tlen);
	vector<int> vt(tlen,0);
	vector<vector<int>> table(nclass,vt);
	for(auto p:CTN[tid].Cmts)
	{
		auto c(p.second);
		auto t(c->Class);
		(*ct)(c->Time);
		int ctime((*ct)());
		int spantime(ctime-ttime);
		if(spantime>=0&&spantime<span*tlen&&(t==0||t==1))
		{
			table[t][spantime/span]+=1;
			tmps[t]+=1;
		}
	}
	for(auto i(tlen-1);i>0;i--)
		if(table[0][i]!=0||table[1][i]!=0)
		{
			len=i+1;
			break;
		}
	double rate(static_cast<double>(tmps[0])/static_cast<double>(tmps[1]));
	for(auto i:Range(len))
	{
		out<<i<<',';
		out<<table[0][i]<<',';
		out<<static_cast<double>(table[1][i])*rate<<',';
		out<<endl;
	}
	cout<<"Paint "<<tid<<" EmoMove Over"<<endl;
}

string TitleTime("2015-10-06-01:59");
inline bool IsPid(string p)
{
	if (p.length() == 19)
	{
		for (auto i : Range(19))
			if (!(isdigit(p[i])))
				return false;
		return true;
	}
	return false;
};
void MakeShort(string tid = "1220103936")//qingdao
{
	ifstream inPN("D:/code/emotion/paragraph_name.txt");
	ofstream outPT("d:/code/emotion/PidTime.txt");
	string tmp, last;
	auto cnt(0);
	while (!(inPN.eof()))
	{
		inPN >> tmp;
		if (IsPid(tmp) && (last != tmp))
		{
			cnt++;
			auto cp(CTN[tid].Cmts[tmp]);
			if (cp != nullptr)
				outPT << tmp << ' ' << cp->Time << endl;
			else
				outPT << tmp << ' ' << TitleTime << endl;
		}
		last = tmp;
	}
	cout << endl << cnt << endl;
};
template <int k = 7>
void Count(string CmtClass = "d:/code/emotion/mbkm.txt", const int PNum = 72152)//comment classification
{
	const int ITV = 12;//interval=12min
	ifstream inPT("d:/code/emotion/PidTime.txt");
	ifstream inC(CmtClass);
	ofstream out("d:/code/emotion/outcome.csv");
	string pid, ptime, tmp;
	int pc, cnt(PNum), last(0);
	int all[500][k];
	(*tt)(TitleTime);
	for (auto i : Range(500))
		for (auto j : Range(k))
			all[i][j] = 0;
	while (cnt--)
	{
		inPT >> pid;
		inPT >> ptime;
		inC >> tmp;
		inC >> tmp;
		pc = StoN(tmp);
		(*ct)(ptime);
		auto intt((*tt)() - (*ct)());
		if (intt > last)
			last = intt;
		//cout << intt / ITV <<' '<<pc<< endl;
		all[intt / ITV][pc]++;
	}
	for (auto i : Range(last / ITV))
	{
		out << i*ITV << ',';
		for (auto j : Range(k - 1))
			out << all[i][j] << ',';
		out << all[i][k - 1] << endl;
	}
};
#endif // _EMOTION_H
/*
template<int NDoc, int NWord, int NClass = 3>//-1,0,1
tuple<vector<vector<double>>, vector<double>>
NBTrain(vector<vector<int>> Mat, vector<int> Cls)
{
vector<int> ClsCnt(NClass, 0), vti(NWord, 1);
vector<double> ClsP(NClass, 0.0), vtd(NWord, 0.0);
vector<vector<int>> VecS(NClass, vti);
vector<vector<double>> Ret(NClass, vtd);
vector<int> WordS(NClass, 0);

cout<<"train"<<endl;
for (auto i : Cls)
ClsCnt[i + 1] += 1;
for(auto i:ClsCnt)
cout<<i<<endl;
cout<<endl;

for (auto i : Range(NClass))
ClsP[i] = ddiv(ClsCnt[i], NDoc),
ClsCnt[i]=NClass;
for(auto i:ClsP)
cout<<i<<endl;
cout<<endl;

for (auto i : Range(NDoc))
{
auto index(Cls[i] + 1);
VecAdd<NWord>(VecS[index], Mat[i]);
ClsCnt[index] += accumulate(Mat[i].cbegin(), Mat[i].cend(), 0);
}
for(auto i:ClsCnt)
cout<<i<<endl;
cout<<endl;

for (auto i : Range(NClass))
for (auto j : Range(NWord))
Ret[i][j] = log(ddiv(VecS[i][j], ClsCnt[i]));
return make_tuple(Ret, ClsP);
};
*/
/*
template<int NWord, int NClass >
tuple<vector<vector<double>>,vector<double>>
NBRead()
{
ifstream in("D:/code/pdata/pNB.pdt");
string tmp;
vector<double> RetClsP(NClass,0.0),vt(NWord,0.0);
vector<vector<double>> RetWordP(NClass,vt);
cout<<"NBRead begin"<<endl;
for(auto i:Range(NClass))
{
in>>tmp;
RetClsP[i]=StoN<double>(tmp);
}
for(auto i:Range(NClass))
for(auto j:Range(NWord))
{
in>>tmp;
RetWordP[i][j]=StoN<double>(tmp);
}
cout<<"NBRead end"<<endl;
return make_tuple(RetWordP,RetClsP);
};
*/
