#pragma once
#ifndef _PR_H
#define _PR_H // PATTERN RECOGNIZE
#include "use.h"
#include "model.h"
using namespace Tree;
template<int d = DEM>
double Dst(double a[], double b[])//calc dst
{
    auto tmps(0.0);
	for (auto i(0); i<d; i++)
        tmps += (a[i] - b[i])*(a[i] - b[i]);
	return sqrt(tmps);
};
template<int k = K, int d = DEM>
bool IsOver(double a[][d], double b[][d])//if iter over
{
	for (auto i(0); i < k; i++)
		for (auto j(0); j < d; j++)
			if (fabs(b[i][j] - a[i][j]) >= 0.01)
				return false;
	return true;
};
template<int d = DEM>
bool NoZero(int t[])//if immature[] has 0
{
	for (auto i(0); i<d; i++)
		if (t[i] == 0)
			return false;
	return true;
};
template<typename T,int d = DEM>
void Cpy(T src[], T dst[])//cpy from double[] to double[]
{
	for (auto i(0); i<d; i++)
		dst[i] = src[i];
};
template<typename T = double, int k = K, int d = DEM>
void Cpy(T src[][d], T dst[][d])//cpy from double[][] to double[][]
{
	for (auto i(0); i<k; i++)
		for (auto j(0); j<d; j++)
			dst[i][j] = src[i][j];
};
template <typename T=double, int d = DEM>
int FindMin(T s[])
{
	int id(0);
	T m(s[0]);
	for (auto i(1); i < d; i++)
		if (m < s[i])
		{
			m = s[i];
			id = i;
		}
	return id;
};
template<int K = 30, int D = DEM, int M = MODE>
void KMeans()//K-MEANS
{
	double klast[K][D], knew[K][D], tmps[K][D], tmpdst[K];//class means
	vector<vector<int>> FCTN;
	int clsnum[K],tmpcls,forcnt(0);
	ifstream in("feature.txt");
	[&]()mutable->void//init
	{
		if (in.is_open())
		{
			string ts; int ti; vector<int> tv;
			while (!in.eof())
			{
				for (auto i(0); i < D; i++)
				{
					in >> ts; ti = StoI(ts); tv.insert(ti);
				}
				FCTN.insert(tv);
				tv.clear();
				cout << FCTN.size() << endl;
			}
			int cnt(0);
			for (auto i(0); i < K; i++)
			{
				clsnum[i] = 0;
				for (auto j(0); j<D; j++)
					knew[i][j] = knew[i][j] = tmps[i][j] = 0;
			}
			for (auto p : CTN)
			{
				for (auto i(0); i<DEM; i++)
					if (NoZero(p.second.Immature))
						p.second.Feature[i] = log(static_cast<double>(p.second.Immature[i]));
				if (cnt >= K)
					continue;
				Cpy<double>(p.second.Feature, knew[cnt]);
				cnt++;
			}
		}
		else
			return;
	}();
	while (!IsOver<K>(klast, knew))
	{
		cout << forcnt++ << ' ';
		Cpy<double, K, D>(knew, klast);
		[&]()mutable->void
		{
			tmpcls = -1;
			for (auto i(0); i < K; i++)
			{
				tmpdst[i] = clsnum[i] = 0;
				for (auto j(0); j < D; j++)
					tmps[i][j] = 0;
			}
		}();
		for (auto& p : CTN)
		{
			if (NoZero(p.second.Immature))
			{
				for (auto i(0); i<K; i++)
					tmpdst[i] = Dst(p.second.Feature, klast[i]);
				tmpcls = FindMin(tmpdst);
				p.second.Class = tmpcls;
				clsnum[tmpcls] += 1;
				for (auto i(0); i < D; i++)
					tmps[tmpcls][i] += p.second.Feature[i];
			}
		}
		for (auto i(0); i < K; i++)
			if (clsnum[i] != 0)
				for (auto j(0); j < D; j++)
					knew[i][j] = tmps[i][j] / clsnum[i];
	}
	for (auto p : clsnum)
		cout << p << endl;
};
#endif // PR_HPP
