#pragma once
#include "number/commonfunction.h"
struct MeanVar
{
	friend class CMeanVariance;
	tyStockData mean;
	tyStockData var;
private:
	tyStockData Fn;//�������㷽����м�仯
};

class CMeanVariance
{
public:
	CMeanVariance();
	~CMeanVariance();

	bool GetNextMeanVar(tyStockData newData, MeanVar& meanVar);
	int nNumber;
};

