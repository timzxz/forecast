#pragma once
#ifndef _FIT_H
#define _FIT_H
#include "use.h"
#include "model.h"
using namespace Tree;
double error(double est, double act)
{
	return (est - act)*(est - act);// / act;
}
template<int M = MODE>// 20%test 80%train
void fit()
{
	int ecnt(0), cnt(0), thh(0), th(0);
	double shh(0), sh(0), rate(0), err(0), fit(0), real(0);//use history=3 hour
	ofstream out("fit.csv");
	cout << "FIT BEGIN." << endl;

	cnt = ecnt = thh = th = 0; shh = sh = 0;
	for (auto p : CTN)//train
	{
		cnt++;
		if (cnt%M)
		{
			th = p.second.Immature[0];
			thh = p.second.Immature[2];
			if (th != 0 && thh != 0)
			{
				ecnt++;
				shh += log(static_cast<double>(thh));
				sh += log(static_cast<double>(th));
			}
		}
		else
			continue;
	}
	rate = sh / shh;//for estimate
	cout << "********** ESTIMATE RATE: " << rate << " (FROM " << ecnt << ")" << endl;

	cnt = ecnt = 0; err = 0;
	for (auto p : CTN)//test
	{
		cnt++;
		if (!(cnt%M))
		{
			th = p.second.Immature[0];
			thh = p.second.Immature[2];
			if (th != 0 && thh != 0)
			{
				ecnt++;
				fit = log(static_cast<double>(thh))*rate;
				real = log(static_cast<double>(th));
				out << p.first << ',' << fit << ',' << real << endl;
				err += error(fit, real);
			}
		}
		else
			continue;
	}
	cout << "********** FIT AVG ERROR: " << (err / ecnt) << " (FROM " << ecnt << ")" << endl;
	cout << "FIT END." << endl;
}
#endif // _FIT_H
