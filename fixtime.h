#pragma once
#ifndef _FIXTIME_H
#define _FIXTIME_H
#include "use.h"
#include "model.h"
using namespace Tree;
vector<double> RealTime(MinsPerDay / Gap, 0);
int M2D[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
int m[12] = { 31,28,31,30,31,10,31,31,30,31,30,31 };

struct Time_t
{
	int Year, Month, Day, Hour, Min;
	Time_t() { Year = Month = Day = Hour = Min = 0; }
	inline int operator() ()//time to minutes
	{
		return (((Year - 1) * 365 + M2D[Month - 1] + Day - 1) * 24 + Hour) * 60 + Min;
	};
	inline void operator() (string t)
	{
		if (t.size() == 16)
			this->Year = StoN(t.substr(0, 4)),
			this->Month = StoN(t.substr(5, 2)),
			this->Day = StoN(t.substr(8, 2)),
			this->Hour = StoN(t.substr(11, 2)),
			this->Min = StoN(t.substr(14, 2));
	};
	inline int operator() (int chk)
	{
		double allm(static_cast<double>(Hour * 60 + Min));
		int now(allm / Gap);
		double left(static_cast<double>(chk));
		left -= (allm - static_cast<double>(Gap*now))*ddiv(RealTime[now], Gap);
		now = Next(now);
		while (left>RealTime[now])
		{
			left -= RealTime[now];
			now = Next(now);
		}
		double realm(static_cast<double>(now*Gap) + left*ddiv(RealTime[now], Gap));
		return (*this)() + static_cast<int>(realm>allm ? realm - allm : static_cast<double>(MinsPerDay) + realm - allm);
	};
};
auto ct(make_shared<Time_t>());
auto tt(make_shared<Time_t>());

inline bool IsTime(int ttime, string ctime)
{
	(*ct)(ctime);
	return (*ct)()<ttime;
}
void TimeFreq()
{
	vector<int> TCnt(MinsPerDay / Gap, 0);
	ofstream out(PTimeCoef);
	auto cnt(0);
	cout << "CALC TIME FREQUENCY BEGIN." << endl;
	for (auto p : CTN)
	{
		for (auto pp : p.second.Cmts)
		{
			QuickPrint(cnt);
			cnt++;
			string t((pp.second)->Time);
			int Hour = StoN(t.substr(11, 2));
			int Min = StoN(t.substr(14, 2));
			TCnt[(Hour * 60 + Min) / Gap] += 1;
		}
	}
	ClrLn();
	cout << "CALC TIME FREQUENCY BEGIN." << endl;
	int sum(accumulate(TCnt.cbegin(), TCnt.cend(), 0));
	double avg(ddiv(sum, MinsPerDay / Gap));
	for (auto i : Range(MinsPerDay / Gap))
		out << TCnt[i] / avg << endl;
}

void ReadTimeFreq()
{
	ifstream in(PTimeCoef);
	ofstream out("fixtime.pdt");
	string tmp;
	for (auto i : Range(MinsPerDay / Gap))
	{
		in >> tmp;
		RealTime[i] = func(StoN<double>(tmp));
		//cout<<RealTime[i]<<' ';
	}
	double s(accumulate(RealTime.cbegin(), RealTime.cend(), 0.0));
	double FixMinsPerDay=s*static_cast<double>(Gap);
	cout<<"FixMinsPerDay : "<<FixMinsPerDay<<endl;
	s = static_cast<double>(FixMinsPerDay) / s;
	for (auto& p : RealTime)
	{
		///*
		p *= s;
		out << p << endl;
		//*/
		//p=Gap;
		//cout<<p<<' ';
	}
	//cout<<accumulate(RealTime.cbegin(),RealTime.cend(),0.0)<<endl;
}

#endif // !_FIXTIME_H
/*
template<int chkp>//deprecated
inline bool IsTime(string ttime, string ctime)
{
(*ct)(ctime); (*tt)(ttime);
return (((*ct)() - (*tt)(22, 8)) <= chkp);
};
template<int chkp>
inline bool IsTime(int ttime, string ctime)
{
(*ct)(ctime);
return (((*ct)() - ttime) <= chkp);
};
inline bool IsTime(int chkp, int ttime, string ctime)
{
(*ct)(ctime);
return (((*ct)() - ttime) <= chkp);
};

inline int operator() (int h1, int h2)//convert to legel time, deprecated
{
if (Hour < h2)
{
Hour = 8, Min = 0;
return (*this)();
}
if (Hour >= h1)
{
if (m[Month - 1] == Day)
{
if (Month == 12)
Year += 1, Month = Day = 1, Hour = 8, Min = 0;
else
Month += 1, Day = 1, Hour = 8, Min = 0;
}
else
Day += 1, Hour = 8, Min = 0;
return (*this)();
}
if (Hour<h1&&Hour >= (h1 - up(CHKM)))
{
if (m[Month - 1] == Day)
{
if (Month == 12)
Year += 1, Month = Day = 1, Hour = 8 - (h1 - Hour);
else
Month += 1, Day = 1, Hour = 8 - (h1 - Hour);
}
else
Day += 1, Hour = 8 - (h1 - Hour);
return (*this)();
}
return (*this)();
}
inline int up(int min)
{
return min / 60 + (!((min / 60 * 60) == min));
}
*/
