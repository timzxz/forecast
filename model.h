#pragma once
#ifndef _MODEL_H
#define _MODEL_H // CONSTRUCT TOPOLOGICAL MODEL
#include "use.h"
namespace Tree
{
	struct CMT
	{
		string CID;
		string Time;
		int Degree = 0;
		int Level = 0;
		int Class = -1;
		bool IsReply;
		vector<shared_ptr<CMT>> Son;
		shared_ptr<CMT> Parent;
		CMT(string cmtid, string cmttime, int cls)
		{
			this->CID = cmtid;
			(this->Time) = (cmttime);
			this->IsReply = false;
			this->Class = cls;
		};
		CMT(string cmtid, string cmttime, shared_ptr<CMT> pp, int cls)//pp: parent pointer
		{
			this->CID = cmtid;
			this->IsReply = true;
			(this->Time) = (cmttime);
			this->Parent = pp;
			this->Class = cls;
		};
	};

	struct NEWS
	{
		bool Used = false;
		string Time;
		int NCmts = 0;//all reply
		int NValid = 0;//is reply or has reply
		int Degree = 0;//son num
		vector<shared_ptr<CMT>> Son;//DIRECT comments record (pointer)
		map<string, shared_ptr<CMT>> Cmts;//ALL comments record (pionter)
		void operator<< (shared_ptr<CMT> add)
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

	map<string, NEWS> CTN;//container! ctn.first=news ID, ctn.second=news INFO
	map<string, vector<int>> FCTN;//feature container! ctn.first=news ID, ctn.second=news feature (FOR FIT | KMEANS)

}

#endif // _MODEL_H
