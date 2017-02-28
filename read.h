#pragma once
#ifndef _READ_H
#define _READ_H
#include "use.h"
#include "model.h"
using namespace Tree;
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
void ReadComment(string file)
{
	int ncnt(0);
	string tmp, cid, pid, tid, time;
	ifstream in(file, ios::binary);
	shared_ptr<CMT> prtp;//parent pointer
	if (in.is_open())
	{
		cout << "********** OPEN [" << file << "] START **********" << endl;
		while (!in.eof())
		{
			ncnt++;
			if (!(ncnt % 10000))
			{
				auto t(ncnt);
				if (t != 10000)
				{
					while (t)
					{
						t /= 10;
						cout << "\b \b";
					}
					cout << "\b \b";
				}
				cout << ncnt;
			}
			in >> tmp; cid = tmp;//CID
			in >> tmp; tid = tmp;//TID
			in >> tmp; pid = tmp;//PID
			in >> tmp;//TIME
			if (pid == "null")//NOT reply
			{
				if (CTN[tid].Used == true)
					CTN[tid] += (make_shared<CMT>(cid, tmp));
			}
			else//IS reply
			{
				if (CTN[tid].Used == true)
				{
					prtp = (CTN[tid].Cmts)[pid];//THERE MAYBE BAD CMT TO INC THE [CMTS] SIZE
					if (prtp != nullptr)
						CTN[tid] += (make_shared<CMT>(cid, tmp, prtp));
					else
						(CTN[tid].Cmts).erase(pid);
				}
			}
		}
	}
	in.close();
	for (auto i(0); i < 8; i++)
		cout << "\b \b";
	cout << "********** READ COMMENT OVER. COMMENTS COUNT : " << ncnt << endl;
};
void Read ()//READ AND CONSTRUCT
{
	auto ReadNews = bind(ReadTitle, string("D:/code/pdata/ptitle.pdt"));
	auto ReadCmts = bind(ReadComment, string("D:/code/pdata/pcomment.pdt"));
	map<string, function<void(void)>> Read = { { "News",ReadNews },{ "Comments",ReadCmts } };
	cout << "READ-PRE AND CONSTRUCT BEGIN." << endl;
	Read["News"]();
	Read["Comments"]();
	cout << "READ-PRE AND CONSTRUCT END." << endl;
};
#endif // _READ_H
