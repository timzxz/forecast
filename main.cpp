#include "use.h"
#include "model.h"
#include "feature.h"
#include "read.h"
#include "pr.h"
#include "emotion.h"
#include "regression.h"
using namespace Tree;
int main()
{
    /*
    Read();
    //EmoMove("1283848235");
    //EmoMove("1255757599");
    EmoMove("1224390594");
    /*
    EmoMove("1211760487");
    EmoMove("1211821518");
    EmoMove("1245317404");
    EmoMove("1245366143");
    EmoMove("1244437242");
    EmoMove("1242955388");
    EmoMove("1241918896");
    EmoMove("1223264520");
    EmoMove("1257563376");
    EmoMove("1267026002");

    EmoMove("1220103936");
    EmoMove("1257588071");
    EmoMove("1232422402");
    EmoMove("1231279705");
    EmoMove("1298519354");
    EmoMove("1270794830");
    EmoMove("1233462817");
    EmoMove("1257647568");
    EmoMove("1215804054");
    EmoMove("1274158682");
    */
	/*
    //Read();
    //cout<<CTN["1270794830"].Time<<endl;
    //cout<<CTN["1257588071"].Time<<endl;
    //2015-12-16-10:48
    //2015-11-29-07:35
    //ChooseTile();
	//PreAnalyse();

	vector<int> t({75,90,135,2<<12});
	for(auto i:t)
        Analyse(i);

    Analyse(105);
	//*/
	///*
	//CalcCls();
	Read();
	TimeFreq();
	//ReadTimeFreq();

	//DgrVld();
	//Deepth();
	//History();
	//*/
	/*
	//Read();TimeFreq();
	ReadTimeFreq();
	//*/
	//Feature();ReadFeature();fit<5>();KMeans(); //deprecated
	//SpecLvl(6);
	//Count<7>("d:/code/emotion/agglomerative.txt",10000);
	//CalPy();
}
/*
	[]()
	{
		vector<vector<int>>
			mat({
				{ 0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,1 },
				{ 0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,1,1,
				0,0,0,0,0,0,1,0,
				0,0,0,0,1,0,0,0 },
				{ 0,0,0,0,0,0,0,0,
				1,0,0,0,0,0,0,0,
				0,0,0,0,0,1,0,0,
				1,0,1,0,0,0,0,0 },
				{ 0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0 },
				{ 0,0,0,1,0,0,0,0,
				0,1,0,0,0,0,0,0,
				0,1,0,1,0,0,0,0,
				0,0,1,0,0,0,0,0 },
				{ 0,0,0,0,0,0,0,0,
				0,1,0,0,1,0,0,1,
				0,0,0,0,1,1,0,0,
				0,1,0,0,0,1,1,1 },
		}),
		test({
			{ 0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0 }
		});
		vector<int> cls({ 0,1,0,1,0,1 });
		vector<vector<double>> wordp;
		vector<double> clsp;
		auto train(NBTrain<6, 32, 2>(mat, cls));
		wordp = get<0>(train);
		cout << "main " << endl;
		for (auto i : wordp)
		{
			for (auto j : i)
				cout << j << ' ';
			cout << endl;
		}
		clsp = get<1>(train);
		for (auto i : clsp)
			cout << i << ' ';
		cout << endl;
		auto ans(NBClassify<6, 32, 2>(wordp, clsp, test));
		for (auto i : ans)
			cout << i << endl;
	};

	[]()
	{
	auto tmp(input<15844, 12581>());
	//auto train(NBTrain<15844, 12581, 2>(get<0>(tmp), get<1>(tmp)));
	//NBWrite<15844,12581>(get<0>(train),get<1>(train));
	auto rf(NBRead<12581,2>());
	auto test(NBClassify<15844, 12581, 2>(get<0>(rf), get<1>(rf), get<2>(tmp)));
	CalcError(test, get<3>(tmp));
	};
*/
