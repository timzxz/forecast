#pragma once
#ifndef _FEATURE_H
#define _FEATURE_H // DEFINE SOME FEATRUES
#include "use.h"
#include "model.h"
using namespace Tree;
template <typename T1, typename T2>
inline double ddiv(T1 a1, T2 a2)
{
	return (static_cast<double>(a1)) / a2;
};//DOUBLE DIVISION
template <typename T>
inline int LevelValue(T l)
{
	return static_cast<int>(l);
};//NODE VALUE IN A SPECIFIC LEVEL
template <int chkp = CHKM>//chkp is CHKM
inline bool IsTime(string ttime, string ctime)
{
	auto t(make_shared<Time_t>(ttime));
	auto c(make_shared<Time_t>(ctime));
	return (((*c)() - (*t)(22, 8)) <= chkp);
}
auto DgrVld = [&]()mutable->void//NODE DEGREEE (SON NUM) | NODE IS REPLY OR HAS REPLY
{
	cout << "CALC NODE DEGREE | VALID NODE BEGIN." << endl;
	for (auto& n : CTN)//n : pair<tid,news>
	{
		auto tmp(0);
		n.second.Degree = n.second.Son.size();
		for (auto& c : n.second.Cmts)//c : pair<pid,cmt*>
		{
			if (c.second != nullptr)
			{
				(c.second)->Degree = ((c.second)->Son).size();
				if (((c.second)->Degree) != 0 || ((c.second)->Parent) != nullptr)
					tmp++;
			}
		}
		n.second.NValid = tmp;
		/*
		if(n.second.ncmts!=0)
		cout<<n.second.ncmts<<" "<<n.second.nvalid<<" "<<ddiv(n.second.nvalid,n.second.ncmts)<<endl;
		cout<<n.second.ncmts<<' '<<n.second.cmts.size()<<endl;
		//*/
	}
	cout << "CALC NODE DEGREE | VALID NODE END." << endl;
};
auto Deepth = [&]()mutable->void//NODE DEEPTH (LEVEL VALUE) AND HEAT
{
	int ncnt(0),lcnt(0), hcnt(0), hhcnt2(0), hhcnt1(0), hhcnt3(0);//hhcnt: history hcnt
	deque<shared_ptr<CMT>> buf;
	//ofstream out("feature.txt");
	cout << "CALC NODE DEEPTH BEGIN." << endl;
	for (auto& p : CTN)//p : pair<tid,news>
	{
		//ncnt++;
		//auto t(ncnt);
		//if (t != 1)
		//{
		//	while (t)
		//	{
		//		t /= 10;
		//		cout << "\b \b";
		//	}
		//	cout << "\b \b";
		//}
		//cout << ncnt;
		buf.clear();
		lcnt = 1;
		hcnt = hhcnt2 = hhcnt1=hhcnt3=0;//
		for (auto& pp : p.second.Son)//pp : cmt*
		{
			pp->Level = lcnt;
			buf.push_back(pp);
		}
		while (!buf.empty())
		{
			auto v = buf.front();
			if (v->Level>lcnt)
				lcnt++;
			if (v->Level == lcnt)
				for (auto vp : v->Son)
				{
					vp->Level = lcnt + 1;
					buf.push_back(vp);
				}
			hcnt += LevelValue(lcnt);
			if (IsTime<60>(p.second.Time, v->Time))
			{
				hhcnt3 += LevelValue(lcnt);
				//if (IsTime<125>(p.second.Time, v->Time))
				//{
				//	hhcnt2 += LevelValue(lcnt);
				//	if (IsTime<100>(p.second.Time, v->Time))
				//		hhcnt1 += LevelValue(lcnt);
				//}
			}
			//if (IsTime(p.second.Time, v->Time))
			//	hhcnt2 += LevelValue(lcnt);
			buf.pop_front();
		}
		p.second.Heat = hcnt;
		p.second.Immature[2] = hhcnt3;
		p.second.Immature[0] = p.second.NCmts;//hcnt;
		//p.second.Immature[1] = hhcnt1;
		//p.second.Immature[2] = hhcnt2;
		//p.second.Immature[3] = hhcnt3;
		//out << p.second.Immature[0] << ' ' << p.second.Immature[2] << endl; //<< ' ' << p.second.Immature[2] << ' ' << p.second.Immature[3] << endl;
	}
	//for (auto i(0); i < 4; i++)
	//	cout << "\b \b";
	cout << "CALC NODE DEEPTH END." << endl;
};
auto AvgRl = [&]()mutable->void {};//NET AVG ROUTE LENGTH
auto Btwn = [&]()mutable->void {};//NET BETWEENESS
#endif // FEATURE_HPP
