#pragma once
#ifndef _MODEL_H
#define _MODEL_H // CONSTRUCT TOPOLOGICAL MODEL
#include "use.h"
namespace Tree
{
    const int K=30;
	const int MODE = 5;
	const int DEM = 4;
	const int CHKM = 150;//check minutes
	struct CMT
	{
		string CID;
		string Time;
		int Degree = 0;
		int Level = 0;
		bool IsReply;
		vector<shared_ptr<CMT>> Son;
		shared_ptr<CMT> Parent;
		CMT(string cmtid, string cmttime)
		{
			this->CID = cmtid;
			(this->Time) = (cmttime);
			this->IsReply = false;
		};
		CMT(string cmtid, string cmttime, shared_ptr<CMT> pp)//pp: parent pointer
		{
			this->CID = cmtid;
			this->IsReply = true;
			(this->Time) = (cmttime);
			this->Parent = pp;
		};
	};
	struct NEWS
	{
		bool Used = false;
		string Time;
		double Heat = 0;
		int Class = -1;//1~K (0~K-1)
		int NCmts = 0;//all reply
		int NValid = 0;//is reply or has reply
		int Degree = 0;//son
		vector<shared_ptr<CMT>> Son;//DIRECT comments record (pointer)
		map<string, shared_ptr<CMT>> Cmts;//ALL comments record (pionter)
		int Immature[DEM];
		double Feature[DEM];
		void operator+= (shared_ptr<CMT> add)
		{
			if (!(add->IsReply))
			{
				(this->Cmts)[add->CID] = add;
				(this->Son).push_back(add);
			}
			if (add->IsReply)
			{
				(this->Cmts)[add->CID] = add;
				((add->Parent)->Son).push_back(add);
			}
			this->NCmts++;
		};
	};
	int StoI(string s)
	{
		int ret;
		stringstream ss;
		ss << s;
		ss >> ret;
		return ret;
	};
	map<int, int> M2D = { { 0,0 },{ 1,31 },{ 2,59 },{ 3,90 },{ 4,120 },{ 5,151 },{ 6,181 },
	{ 7,212 },{ 8,243 },{ 9,273 },{ 10,304 },{ 11,334 },{ 12,365 } };
	int m[12] = { 31,28,31,30,31,10,31,31,30,31,30,31 };
	struct Time_t
	{
		int Year, Month, Day, Hour, Min;
		Time_t() { Year = Month = Day = Hour = Min = 0; }
		Time_t(string t)
		{
			this->Year = StoI(t.substr(0, 4));
			this->Month = StoI(t.substr(5, 2));
			this->Day = StoI(t.substr(8, 2));
			this->Hour = StoI(t.substr(11, 2));
			this->Min = StoI(t.substr(14, 2));
		}
		int up(int min)
		{
			return min / 60 + (!((min / 60 * 60) == min));
		}
		int operator() ()//time to minutes
		{
			return (((Year - 1) * 365 + M2D[Month - 1] + Day - 1) * 24 + Hour) * 60 + Min;
		};
		int operator() (int h1, int h2)//convert to legel time
		{
			if (Hour < h2)
			{
				Hour = 8; Min = 0;
				return (*this)();
			}
			if (Hour >= h1)
			{
				if (m[Month - 1] == Day)
				{
					if (Month == 12)
					{
						Year += 1; Month = Day = 1; Hour = 8; Min = 0;
					}
					else
					{
						Month += 1; Day = 1; Hour = 8; Min = 0;
					}
				}
				else
				{
					Day += 1; Hour = 8; Min = 0;
				}
				return (*this)();
			}
			if (Hour<h1&&Hour >= (h1 -  up(CHKM)))
			{
				if (m[Month - 1] == Day)
				{
					if (Month == 12)
					{
						Year += 1; Month = Day = 1; Hour = 8 - (h1 - Hour);
					}
					else
					{
						Month += 1; Day = 1; Hour = 8 - (h1 - Hour);
					}
				}
				else
				{
					Day += 1; Hour = 8 - (h1 - Hour);
				}
				return (*this)();
			}
			return (*this)();
		}
	};
	map<string, NEWS> CTN;//container! ctn.first=news ID, ctn.second=news INFO
}
#endif // _MODEL_H
