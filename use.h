#pragma once
#ifndef _USE_H
#define _USE_H
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include <memory>
#include <cmath>
#include <stdlib.h>
#include <cctype>
using std::string;
using std::vector;
using std::map;
using std::deque;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::bind;
using std::function;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::accumulate;
using std::tuple;
using std::make_tuple;
using std::get;
using std::sort;
const int K = 384;
const int MODE = 5;
const int DEM = 4;
const int CHKM = 120;//check minutes
const int MaxNumLength = 8;
const int NWord = 12581;
const int NClass = 2;
const int Gap = 4;
const int MinsPerDay = 1440;//MinsPerDay is an integer multiple of Gap!!!
const int FixMinsPerDay = 960;

//PATH
string PTitle("D:/code/pdata/ptitle.pdt");
string PComment("D:/code/pdata/pcomment.pdt");
string PTimeCoef("D:/code/pdata/ptimecoef.pdt");
string PFeature("D:/code/pdata/feature.pdt");
string PLevel("D:/code/pdata/level.pdt");
string PRegX("D:/code/pdata/regX.pdt");
string PRegY("D:/code/pdata/regY.pdt");
string SpecTid("1257588071");
//string SpecTid("1270794830");
//for range
class RangeIter
{
public:
	int value;
	explicit RangeIter(int val) : value(val) {}
	bool operator!= (const RangeIter& other) const
	{
		return (this->value) != (other.value);
	}
	int operator* () const { return this->value; }
	const RangeIter& operator++ ()
	{
		value += 1;
		return *this;
	}
};
class Range
{
public:
	int BeginValue;
	int EndValue;
	Range(int b, int e) : BeginValue(b), EndValue(e) {}
	Range(int e) : BeginValue(0), EndValue(e) {}
	RangeIter begin() const { return RangeIter(BeginValue); }
	RangeIter end() const { return RangeIter(EndValue); }
};

inline void SlowPrint(int n)
{
	for (auto n : Range(MaxNumLength))
		cout << "\b \b";
	cout << n;
};
template <int M = 10000>
inline void QuickPrint(int n)
{
	if (!(n % M))
	{
		if (n != M)
			for (auto i : Range(MaxNumLength))
				cout << "\b \b";
		cout << n;
	}
};
inline void ClrLn()
{
	for (auto i : Range(MaxNumLength))
		cout << "\b \b";
};
template <typename T>
inline void ClrLn(T s)
{
	ClrLn();
	cout << s << endl;
};

template <typename T = int>
inline T StoN(string s)
{
	T ret;
	stringstream ss;
	ss << s;
	ss >> ret;
	return ret;
};

inline string ItoS(int i)
{
	string ret;
	stringstream ss;
	ss << i;
	ss >> ret;
	return ret;
};

template <typename T1, typename T2>//DOUBLE DIVISION
inline double ddiv(T1 a1, T2 a2)
{
	return (static_cast<double>(a1)) / (static_cast<double>(a2));
};

template<int sd = 0, int ed = DEM, typename T = vector<double>>//D DEMENSION DISTANCE 0~(d-1)
inline double Dst(T a, T b)//calc dst
{
	auto tmps(0.0);
	for (auto i : Range(sd, ed))
		tmps += (a[i] - b[i])*(a[i] - b[i]);
	return sqrt(tmps);
};

template<int k = K, int d = DEM, typename T = vector<double>>
inline void Cpy(T src[], T dst[])
{
	for (auto i : Range(k))
		for (auto j : Range(d))
			dst[i][j] = src[i][j];
};

template <int k = K, typename T = vector<double>>
inline int FindMin(T s)
{
	int id(0);
	double m = s[0];
	for (auto i : Range(1, k))
		if (s[i]<m)
		{
			m = s[i];
			id = i;
		}
	return id;
};

template <typename Te, typename Ta>
inline Te error(Te est, Ta act)
{
	return (est - static_cast<Te>(act))*(est - static_cast<Te>(act));// *(est - act);// / act;
};

template <int M = 24>
int Next(int n)
{
	return n == (M * 60 / Gap - 1) ? 0 : n + 1;
};

inline double func(double n)
{
	return
		///*2494
		n<0.92 ?
		pow(n, 3.7)
		:
		pow((n+0.08),0.02)-0.08
		;
		//*/
		/*2495
		n<0.92 ?
		pow(n, 3.6)
		:
		pow((n+0.08),0.025)-0.08
		;
		//*/
		/*2506
		n<0.92 ?
		pow(n, 3.8)
		:
		pow((n+0.08),0.03)-0.08
		;
		//*/
	//pow(n,3) exp(pow(n,2)) 2300
	/*
	n<1 ?
	exp(exp(exp(n)))
	:
	pow(n, 2)
	;
	//*/
};

template<int d, typename T = int>
inline void VecAdd(vector<T>& dst, vector<T> src)
{
	for (auto i : Range(d))
		dst[i] += src[i];
};

template<int d, typename T1, typename T2>
inline double VecMul(vector<T1> a, vector<T2> b)
{
	double s(0.0);
	for (auto i : Range(d))
		s += static_cast<double>(a[i])*static_cast<double>(b[i]);
	return s;
};

template<int d, typename T>
inline void VecZero(vector<T>& a)
{
	for (auto i : Range(d))
		a[i] = 0.0;
}
#endif // _USE_H
