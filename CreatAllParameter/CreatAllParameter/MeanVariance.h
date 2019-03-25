#pragma once
#include "number/commonfunction.h"
struct MeanVar
{
	friend class CMeanVariance;
	tyStockData mean;
	tyStockData var;
private:
	tyStockData Fn;//迭代计算方差的中间变化
};

class CMeanVariance
{
public:
	CMeanVariance();
	~CMeanVariance();

	bool GetNextMeanVar(tyStockData newData, MeanVar& meanVar);
	int nNumber;
};

