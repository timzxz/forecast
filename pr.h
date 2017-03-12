#pragma once
#ifndef _PR_H
#define _PR_H // PATTERN RECOGNIZE
#include "use.h"
#include "model.h"
using namespace Tree;
template<int d = DEM, typename T = vector<double>>
inline bool eql(T a, T b)
{
	for (auto i : Range(d))
		if (fabs(a[i] - b[i])>0.0001)
			return false;
	return true;
};

template<int k = K, int d = DEM, typename T = vector<double>>
inline bool IsOver(T a[], T b[])//if iter over
{
	bool flag[k];
	for (auto i : Range(k))
		flag[i] = false;
	for (auto i : Range(k))//a[i]
		for (auto j : Range(k))//b[j]
			if (!flag[j])
				if (eql(a[i], b[j]))
					flag[j] = true;
	for (auto i : Range(k))
		if (!flag[i])
			return false;
	return true;
};

template <int k = K, int d = DEM, int m = MODE>
void KMeans()//K-MEANS
{
	double tmpdst[k];
	vector<double> klast[k], knew[k], tmps[k], tv({ 0,0,0,0 });
	map<string, vector<double>> lfctn;//log feature CTN
	map<string, int> cls;//CLASS CTN
	int clsacc[k], tmpcls, forcnt(0);
	int train(0), testcnt(0), traincnt(0);
	//calc log(feature)

	train = static_cast<int>(0.7*static_cast<double>(FCTN.size()));
	traincnt = 0;
	for (auto p : FCTN)
	{
		traincnt++;
		if (traincnt <= train)
		{
			bool hz = false;//has zero
			for (auto pp : p.second)
				if (pp == 0)
				{
					hz = true;
					break;
				}
			if (!hz)
				for (auto pp : p.second)
				{
					lfctn[p.first].push_back(log(static_cast<double>(pp)));
					cls[p.first] = -1;
				}
		}
		else
			break;
	}
	cout << FCTN.size() << ' ' << lfctn.size() << endl;
	//clear
	for (auto i : Range(k))
	{
		tmpdst[i] = 0;
		clsacc[i] = 0;
	}

	//init
	auto tmp(0);
	for (auto p : lfctn)
		if (tmp < k)
		{
			knew[tmp] = p.second;
			klast[tmp] = tv;
			tmps[tmp] = tv;
			tmp++;
		}
		else
			break;
	/*
	for(auto p:lfctn)
	{
	cout<<p.first<<" : ";
	for(auto pp:p.second)
	cout<<pp<<' ';
	cout<<endl;
	}
	*/
	while (!IsOver(klast, knew))
	{
		forcnt++;
		SlowPrint(forcnt);
		//clear
		for (auto i : Range(k))
		{
			tmpdst[i] = 0;
			clsacc[i] = 0;
			tmps[i] = tv;
		}

		Cpy(knew, klast);
		for (auto p : lfctn)
		{
			for (auto i : Range(k))
				tmpdst[i] = Dst(p.second, klast[i]);
			tmpcls = FindMin(tmpdst);
			cls[p.first] = tmpcls;
			clsacc[tmpcls] += 1;
			for (auto i : Range(d))
				(tmps[tmpcls])[i] += (p.second)[i];

		}
		for (auto i : Range(k))
			for (auto j : Range(d))
				if (clsacc[i] != 0)
					knew[i][j] = (tmps[i][j]) / (clsacc[i]);
	}
	//test
	ofstream out("kmeans.csv");
	traincnt = 0;
	double est(0), err(0), act(0);
	vector<double> lf;
	for (auto p : FCTN)
	{
		traincnt++;
		if (traincnt>train)
		{
			testcnt++;
			lf.clear();
			for (auto i : p.second)
				lf.push_back(log(static_cast<double>(i)));
			for (auto i : Range(k))
				tmpdst[i] = Dst<1>(lf, knew[i]);
			tmpcls = FindMin(tmpdst);
			//cout<<tmpcls<<' ';
			est = exp((knew[tmpcls][0]));
			act = static_cast<double>((p.second)[0]);
			err += error(est, act);
			out << p.first << ',' << (est) << ',' << (act) << endl;
		}
		else
			continue;
	}
	ClrLn();
	cout << "\nK:" << k << ". ITER NUM: " << forcnt
		<< ".\nAVG ERROR: " << sqrt(err / testcnt)
		<< ". (FROM " << testcnt << ")" << endl;
	/*
	ofstream out("kmeans.csv");
	double est(0), err(0);
	double act(0);
	for (auto p : lfctn)
	{
	for (auto i : Range(k))
	tmpdst[i] = Dst<1>(p.second, knew[i]);
	tmpcls = FindMin(tmpdst);
	//cout<<tmpcls<<' ';
	est = (knew[tmpcls][0]);
	act = log(static_cast<double>((FCTN[p.first])[0]));
	err += error(est, act);
	out << p.first << ',' << (est) << ',' << (act) << endl;
	}
	ClrLn();
	cout << "\nK:" << k << ". ITER NUM: " << forcnt << ".\nAVG ERROR: " << sqrt(err / (lfctn.size())) << ". (FROM " << lfctn.size() << ")\n";
	*/
};

#endif // PR_HPP
