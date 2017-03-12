#pragma once
#ifndef _REGRESSION_H
#define _REGRESSION_H
#include "use.h"
#include "model.h"
using namespace Tree;

template<int M = MODE>// 20%test 80%train
void fit()//y=ax+b
{
	int cnt(0);
	const int point = 1;//60 min
	double a, b, x, y, s_xy(0), s_x(0), s_y(0), s_xx(0), s_err(0), est, real;
	map<string, double[2]> fitctn;//container for fit [0]:log ncmts [1]:log 105min hhcnt
	ofstream out("fit.txt");
	cout << "FIT BEGIN." << endl;

	for (auto p : FCTN)
	{
		auto ps(p.second);
		if (ps[0] != 0 && ps[point] != 0)
		{
			fitctn[p.first][0] = log(static_cast<double>(ps[0]));//NCMTS
			fitctn[p.first][1] = log(static_cast<double>(ps[point]));
		}
	}
	for (auto p : fitctn)
	{
		auto ps(p.second);
		x = ps[point]; y = ps[0];
		s_xy += x*y;
		s_x += x;
		s_y += y;
		s_xx += x*x;
		cnt++;
	}

	a = (s_xy - s_x * s_y / cnt) / (s_xx - s_x * s_x / cnt);
	b = s_y / cnt - a * s_x / cnt;

	for (auto p : fitctn)
	{
		auto ps(p.second);
		est = exp((a*(ps[point])) + b);
		real = (FCTN[p.first])[0];
		s_err += error(est, real);
		out << p.first << ',' << static_cast<int>(est) << ',' << real << endl;
	}

	cout << "********** FIT AVG ERROR: " << sqrt(s_err / cnt) << " (FROM " << cnt << ")" << endl;
	cout << "FIT END." << endl;
};
template<int d = 5>
inline double VecMul(vector<double> a, vector<double> b)
{
	double s(0);
	for (auto i : Range(d))
		s += a[i] * b[i];
	return s;
}
template <int d = 3, int cut = 900, int all = 3212>
void CalPy()
{
	ifstream inx("D:/code/pdata/regX.pdt");
	ifstream iny("D:/code/pdata/regY.pdt");
	vector<double> vx, esty, acty;
	vector<double> w({ 0.66208427,-0.45038038,0.0 });
	//({6.40894738,-5.79793928,3.18899,-0.01077439,0.0});
	string tmp;
	int cnt(0);
	while (!(inx.eof()))
	{
		cnt++;
		if (cnt <= (all - cut))
			continue;
		else
		{
			for (auto i : Range(d))
			{
				inx >> tmp;
				vx.push_back(StoN<double>(tmp));
			}
			esty.push_back(VecMul<d>(vx, w));
			vx.clear();
		}
	}
	cnt = 0;
	while (!(iny.eof()))
	{
		cnt++;
		if (cnt <= (all - cut))
			continue;
		else
		{
			iny >> tmp;
			acty.push_back(StoN<double>(tmp));
		}
	}

	double rmse(0);
	for (auto i : Range(cut))
		rmse += error(exp(esty[i]), exp(acty[i]));
	rmse /= cut;
	cout << sqrt(rmse) << endl;
}
#endif // _REGRESSION_H
