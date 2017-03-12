#pragma once
#ifndef _FEATURE_H
#define _FEATURE_H // DEFINE SOME FEATRUES
#include "use.h"
#include "model.h"
#include "fixtime.h"
using namespace Tree;

inline bool IsVld(shared_ptr<CMT> c)
{
	if (c != nullptr)
		if (c->Level != 1 || c->Degree != 0)
			return true;
	return false;
};

auto DgrVld = [&]()mutable->void//NODE DEGREEE (SON NUM) | NODE IS REPLY OR HAS REPLY
{
	cout << "CALC NODE DEGREE | VALID NODE BEGIN." << endl;
	for (auto& n : CTN)//n : pair<tid,news>
	{
		auto tmp(0);
		n.second.Degree = n.second.Son.size();
		for (auto& c : n.second.Cmts)//c : pair<pid,cmt*>
			if (c.second != nullptr)
			{
				(c.second)->Degree = ((c.second)->Son).size();
				if (((c.second)->Degree) != 0 || ((c.second)->Parent) != nullptr)
					tmp++;
			}
		n.second.NValid = tmp;
	}
	cout << "CALC NODE DEGREE | VALID NODE END." << endl;
};

auto Deepth = [&]()mutable->void //WEIGHT
{
	int ncnt(0), lcnt(0), MaxLevel(0);
	//Vld : Valid : has reply or is reply
	int AllNodeNum(0), AllLevelSum(0), VldNodeNum(0), VldLevelSum(0);
	double AllAvgLevel(0), VldAvgLevel(0);
	deque<shared_ptr<CMT>> buf;
	ofstream out(PLevel);
	cout << "CALC NODE DEEPTH BEGIN." << endl;
	for (auto& p : CTN)//p : pair<tid,news>
	{
		ncnt++;
		QuickPrint<50>(ncnt);

		MaxLevel = AllNodeNum = AllLevelSum = VldNodeNum = VldLevelSum = 0;
		VldAvgLevel = AllAvgLevel = 0;
		buf.clear();
		lcnt = 1;

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
			if (IsVld(v))
				VldLevelSum += lcnt;
			AllLevelSum += lcnt;
			buf.pop_front();
		}
		AllNodeNum = p.second.NCmts;
		AllAvgLevel = ddiv(AllLevelSum, AllNodeNum);
		VldNodeNum = p.second.NValid;
		VldAvgLevel = ddiv(VldLevelSum, VldNodeNum);
		MaxLevel = lcnt;
		//out << p.first << ' ' << VldNodeNum << ' ' << VldLevelSum << ' ' << VldAvgLevel << ' '
		//	<< AllNodeNum << ' ' << AllLevelSum << ' ' << AllAvgLevel << ' ' << MaxLevel << endl;
		if (VldNodeNum != 0)
			out << p.first << ',' << log(VldNodeNum) << ',' << log(VldLevelSum) << ',' << VldAvgLevel << ','
			<< log(AllNodeNum) << ',' << log(AllLevelSum) << ',' << AllAvgLevel << ',' << MaxLevel << endl;
	}
	ClrLn();
	cout << "CALC NODE DEEPTH END." << endl;
};

auto SpecLvl = [=](int l)mutable->void//specific max level = 4
{
	string tmp, heat, o;
	int maxlvl;
	ifstream in("level.prn");
	o += "maxlvlk";
	o += ItoS(l);
	o += ".csv";
	cout << o << endl;
	ofstream out(o);
	cout << "Max Level = " << l << " Begin." << endl;
	while (!in.eof())
	{
		in >> tmp; in >> tmp; in >> tmp; in >> tmp; in >> tmp;
		heat = tmp;
		in >> tmp; in >> tmp; in >> tmp;
		maxlvl = StoN(tmp);
		in >> tmp;
		if (maxlvl == l)
			out << heat << ',' << l << endl;
	}
	in.close();
	cout << "Max Level = " << l << " END." << endl;
};

template <int chkp1 = 100, int chkp2 = 90, int chkp3 = 80, int chkp4 = 70, int chkp5 = 60, int nchkp = 5, int nclass = NClass>
void History(string px = PRegX, string py = PRegY)
{
	const int chkp[nchkp] = { chkp1,chkp2,chkp3,chkp4,chkp5 };
	int ncnt(0), ttime[nchkp], MaxLevel[nchkp],
		AllNodeNum[nclass][nchkp], AllLevelSum[nclass][nchkp],
		VldNodeNum[nclass][nchkp], VldLevelSum[nclass][nchkp];
	ofstream outx(px), outy(py), outxy("d:/code/pdata/RegXY");
	cout << "CALC REG_X, REG_Y BEGIN." << endl;
	for (auto& p : CTN)//p : pair<tid,news>
	{
		ncnt++;
		SlowPrint(ncnt);
		(*tt)(p.second.Time);
		for (auto i : Range(nclass))
			for (auto j : Range(nchkp))
			{
				AllNodeNum[i][j] = AllLevelSum[i][j] =
					VldNodeNum[i][j] = VldLevelSum[i][j] =
					MaxLevel[j] = 0;
				//ttime[j] = (*tt)(chkp[j]);
				ttime[j] = (*tt)()+chkp[j];
			}
		for (auto pp : p.second.Cmts)
		{
			auto v(pp.second);
			auto c(v->Class);
			for (auto i : Range(nchkp))
			{
				if (IsTime(ttime[i], v->Time))
				{
					if (c == 0 || c == 1)
					{
						AllNodeNum[c][i] += 1;
						auto l(v->Level);
						AllLevelSum[c][i] += l;
						if (l>MaxLevel[i])
							MaxLevel[i] = l;
						if (IsVld(v))
						{
							VldLevelSum[c][i] += l;
							VldNodeNum[c][i] += 1;
						}
					}
				}
				else
					break;
			}
		}
		if (VldNodeNum[0][nchkp - 1] != 0 &&
			VldNodeNum[1][nchkp - 1] != 0 &&
			MaxLevel[nchkp - 1] != 0)
		{
			///*
			outy << log(static_cast<double>(p.second.NCmts)) << endl;
			for (auto i : Range(nchkp))
			{
				outx
					<< log(static_cast<double>(AllNodeNum[0][i]+AllNodeNum[1][i])) << ' '
					<< log(static_cast<double>(AllLevelSum[0][i]+AllLevelSum[1][i])) << ' '
					<< ddiv(AllLevelSum[0][i]+AllLevelSum[1][i], AllNodeNum[0][i]+AllNodeNum[1][i]) << ' '
					<< log(static_cast<double>(VldNodeNum[0][i]+VldNodeNum[1][i])) << ' '
					<< log(static_cast<double>(VldLevelSum[0][i]+VldLevelSum[1][i])) << ' '
					<< ddiv(VldLevelSum[0][i]+VldLevelSum[1][i], VldNodeNum[0][i]+VldNodeNum[1][i]) << ' ';
				outx << log(static_cast<double>(MaxLevel[i])) << ' ';
			}
			outx << '1' << endl;
			//*/
			/*
			outy << log(static_cast<double>(p.second.NCmts)) << endl;
			for (auto i : Range(nchkp))
			{
				for (auto j : Range(nclass))
					outx
					<< log(static_cast<double>(AllNodeNum[j][i])) << ' '
					<< log(static_cast<double>(AllLevelSum[j][i])) << ' '
					<< ddiv(AllLevelSum[j][i], AllNodeNum[j][i]) << ' '
					<< log(static_cast<double>(VldNodeNum[j][i])) << ' '
					<< log(static_cast<double>(VldLevelSum[j][i])) << ' '
					<< ddiv(VldLevelSum[j][i], VldNodeNum[j][i]) << ' ';

				outx << ddiv(AllNodeNum[0][i],
					AllNodeNum[0][i]+AllNodeNum[1][i]) << ' ';
				outx << log(static_cast<double>(MaxLevel[i])) << ' ';
			}
			outx << '1' << endl;
			//*/
			/*
			outxy << log(static_cast<double>(p.second.NCmts)) << ' ';
			int t(1);
			for (auto i : Range(nchkp))
			{
				for (auto j : Range(nclass))
				{
					outxy << t << ':' << log(static_cast<double>(AllNodeNum[j][i])) << ' '; t++;
					outxy << t << ':' << log(static_cast<double>(AllLevelSum[j][i])) << ' '; t++;
					outxy << t << ':' << ddiv(AllLevelSum[j][i], AllNodeNum[j][i]) << ' '; t++;
					outxy << t << ':' << log(static_cast<double>(VldNodeNum[j][i])) << ' '; t++;
					outxy << t << ':' << log(static_cast<double>(VldLevelSum[j][i])) << ' '; t++;
					outxy << t << ':' << ddiv(VldLevelSum[j][i], VldNodeNum[j][i]) << ' '; t++;
				}
				outxy << t << ':' << log(static_cast<double>(MaxLevel[i])) << ' '; t++;
			}
			outxy << t << ":1" << endl;
			*/
			//*/
			/*
			outy << p.second.NCmts << endl;
			for (auto i : Range(nchkp))
			outx
			<< AllNodeNum[i] << ' '
			<< AllLevelSum[i] << ' '
			<< ddiv(AllLevelSum[i], AllNodeNum[i]) << ' '
			<< VldNodeNum[i] << ' '
			<< VldLevelSum[i] << ' '
			<< ddiv(VldLevelSum[i], VldNodeNum[i]) << ' '
			<< MaxLevel[i] << ' '
			;
			outx<<"1 ";
			outx << endl;

			outxy<<log(static_cast<double>(p.second.NCmts))<<' ';
			int t(1);
			for (auto i : Range(nchkp))
			{
			outxy<< t<<':'<<AllNodeNum[i] << ' ';t++;
			outxy<< t<<':'<<AllLevelSum[i]<<' ';t++;
			outxy<< t<<':'<<ddiv(AllLevelSum[i], AllNodeNum[i]) << ' ';t++;
			outxy<< t<<':'<<VldNodeNum[i]<< ' ';t++;
			outxy<< t<<':'<<VldLevelSum[i] << ' ';t++;
			outxy<< t<<':'<<ddiv(VldLevelSum[i], VldNodeNum[i]) << ' ';t++;
			outxy<< t<<':'<<MaxLevel[i] << ' ';t++;
			}
			outxy<<t<<":1"<<endl;
			//*/
		}
	}
	ClrLn("CALC REG_X, REG_Y END.");
};

auto AvgRl = [&]()mutable->void {};//NET AVG ROUTE LENGTH
auto Btwn = [&]()mutable->void {};//NET BETWEENESS
#endif // FEATURE_HPP


								  /*
								  auto Feature = [&]()mutable->void //NO WEIGHT, DEPRECATED
								  {
								  int ncnt(0), hhcnt2(0), hhcnt1(0), hhcnt3(0), ttime;//hhcnt: history hcnt
								  ofstream out(PFeature);
								  cout << "CALC ALL FEATURE BEGIN." << endl;
								  for (auto& p : CTN)//p : pair<tid,news>
								  {
								  ncnt++;
								  SlowPrint(ncnt);
								  hhcnt1 = hhcnt2 = hhcnt3 = 0;
								  (*tt)(p.second.Time);
								  ttime = (*tt)(22, 8);
								  for (auto pp : p.second.Cmts)
								  {
								  auto v(pp.second);
								  if (IsTime<90>(ttime, v->Time))
								  {
								  hhcnt3 += 1;
								  if (IsTime<75>(ttime, v->Time))
								  {
								  hhcnt2 += 1;
								  if (IsTime<60>(ttime, v->Time))
								  hhcnt1 += 1;
								  }
								  }
								  }
								  if (hhcnt1 != 0)
								  out << p.first << ' ' << p.second.NCmts << ' ' << hhcnt1 << ' ' << hhcnt2 << ' ' << hhcnt3 << endl;
								  //out << p.first << '\t' << hhcnt1 << '\t' << hhcnt2 << '\t' << hhcnt3 <<'\t'<< p.second.NCmts << '\t' << endl;
								  }
								  ClrLn();
								  cout << "CALC ALL FEATURE END." << endl;
								  };
								  auto ReadFeature = [&]()mutable->void
								  {
								  int cnt(0);
								  string tmp, id;
								  ifstream in(PFeature);
								  cout << "READ FEATURE BEGIN." << endl;
								  if (in.is_open())
								  {
								  while (!in.eof())
								  {
								  QuickPrint<100>(cnt);
								  cnt++;
								  in >> tmp;
								  id = tmp;
								  for (auto i : Range(DEM))
								  {
								  in >> tmp;
								  FCTN[id].push_back(StoN(tmp));
								  }
								  }
								  ClrLn("READ FEATURE END.");
								  in.close();
								  }
								  };
								  */
