#pragma once
#ifndef _READ_H
#define _READ_H
#include "use.h"
#include "model.h"
#include "emotion.h"
using namespace Tree;
map<string, int> CLS;
void ReadTitle(string file)
{
	int ncnt(0);
	string tmp, tid;
	ifstream in(file, ios::binary);
	if (in.is_open())
	{
		cout << "********** OPEN [" << file << "] START **********" << endl;
		while (!in.eof())
		{
			in >> tmp; tid = tmp;
			ncnt++;
			CTN[tid].Used = true;
			in >> tmp;
			(CTN[tid].Time) = (tmp);
		}
		in.close();
		cout << "********** READ NEWS OVER. NEWS COUNT : " << ncnt << endl;
	}
};
void CalcCls()
{
	ifstream incls("d:/code/pdata/pCmtCls.pdt");
	ofstream outcls("d:/code/pdata/pClsOut.pdt");
	NBRead<12581, 2>();
	if (incls.is_open())
	{
		string emp(""), cid(""), last(""), tmp("");/*last cid*/
		int cnt(0);
		vector<int> tmpvec(NWord, 0);
		cout << "Calc Cmt Class Start" << endl;
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
					outcls << last << ' ' << NBClassify(tmpvec);
					VecZero<NWord>(tmpvec);
					cnt++;
					QuickPrint<2000>(cnt);
				}
			}
			else
				tmpvec[StoN(tmp)] = 1;
		}
		ClrLn("Calc Cmt Class Over");
	}
};
void ReadCls()
{
	ifstream in("d:/code/pdata/pClsOut.pdt");
	string tmp(""), cid("");
	int cnt(0);
	cout << "Read Class Start." << endl;
	if (in.is_open())
		while (!in.eof())
		{
			QuickPrint<10000>(cnt++);
			in >> tmp;
			cid = tmp;
			in >> tmp;
			CLS[cid] = StoN(tmp);
		}
	ClrLn("Read Class Over.");
};
void ReadComment(string file)
{
	int ncnt(0);
	string tmp, cid, pid, tid, time;
	ifstream in(file, ios::binary);
	shared_ptr<CMT> prtp;//parent pointer
	ReadCls();
	if (in.is_open())
	{
		cout << "********** OPEN [" << file << "] START **********" << endl;
		while (!in.eof())
		{
			ncnt++;

			if (!(ncnt % 100000))
				if (ncnt != 100000)
					cout << '(' << ncnt << ')'
					<< " CLS: " << CLS.size()
					<< " CTN: " << CTN.size() << endl;

			in >> tmp; cid = tmp;//CID
			in >> tmp; tid = tmp;//TID
			in >> tmp; pid = tmp;//PID
			in >> tmp;//TIME
			if (pid == "null")//NOT reply
			{
				if (CTN[tid].Used == true && (CLS[cid] == 1 || CLS[cid] == 0))
					CTN[tid] << (make_shared<CMT>(cid, tmp, CLS[cid]));
			}
			else//IS reply
				if (CTN[tid].Used == true && (CLS[cid] == 1 || CLS[cid] == 0))
				{
					prtp = (CTN[tid].Cmts)[pid];//THERE MAYBE BAD CMT TO INC THE [CMTS] SIZE
					if (prtp != nullptr)
						CTN[tid] << (make_shared<CMT>(cid, tmp, prtp, CLS[cid]));
					else//SO THEN ERASE IT
						(CTN[tid].Cmts).erase(pid);
				}
			CLS.erase(cid);
		}
	}
	in.close();
	ClrLn();
	cout << "********** READ COMMENT OVER. COMMENTS COUNT : " << ncnt << endl;
};
void Read()//READ AND CONSTRUCT
{
	auto ReadNews = bind(ReadTitle, PTitle);
	auto ReadCmts = bind(ReadComment, PComment);
	map<string, function<void(void)>> Read = { { "News",ReadNews },{ "Comments",ReadCmts } };
	cout << "READ-PRE AND CONSTRUCT BEGIN." << endl;
	Read["News"]();
	Read["Comments"]();
	cout << "READ-PRE AND CONSTRUCT END." << endl;
};
#endif // _READ_H
/*
void ReadComment(string file)
{
int ncnt(0);
string tmp, cid, pid, tid, time;
ifstream in(file, ios::binary);
ifstream incls("d:/code/pdata/pCmtCls.pdt");
shared_ptr<CMT> prtp;//parent pointer
if (in.is_open())
{
cout << "********** OPEN [" << file << "] START **********" << endl;
while (!in.eof())
{
ncnt++;
QuickPrint(ncnt);

in >> tmp; cid = tmp;//CID
in >> tmp; tid = tmp;//TID
in >> tmp; pid = tmp;//PID
in >> tmp;//TIME
if (pid == "null")//NOT reply
{
if (CTN[tid].Used == true)
CTN[tid] << (make_shared<CMT>(cid, tmp));
}
else//IS reply
{
if (CTN[tid].Used == true)
{
prtp = (CTN[tid].Cmts)[pid];//THERE MAYBE BAD CMT TO INC THE [CMTS] SIZE
if (prtp != nullptr)
CTN[tid] << (make_shared<CMT>(cid, tmp, prtp));
else//SO THEN ERASE IT
(CTN[tid].Cmts).erase(pid);
}
}
}
}
in.close();
ClrLn();
cout << "********** READ COMMENT OVER. COMMENTS COUNT : " << ncnt << endl;
};
*/
