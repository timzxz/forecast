#include "use.h"
#include "model.h"
#include "feature.h"
#include "read.h"
#include "fit.h"
//#include "pr.h"
using namespace Tree;
int main()
{
	Read();
	//DgrVld();
	Deepth();
	fit<5>();
	//KMeans();
	CTN.clear();

}
