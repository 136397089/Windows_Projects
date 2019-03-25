#include "stdafx.h"
#include "MeanVariance.h"


CMeanVariance::CMeanVariance()
{
	nNumber = 0;
}


CMeanVariance::~CMeanVariance()
{
}

bool CMeanVariance::GetNextMeanVar(tyStockData newData, MeanVar& reasultMV)
{
	nNumber++;
	MeanVar lastData = reasultMV;
	reasultMV.mean = lastData.mean + (newData - lastData.mean) / nNumber;

	reasultMV.Fn = reasultMV.Fn + (newData - lastData.mean)*(newData - reasultMV.mean);
	reasultMV.var = reasultMV.Fn / nNumber;

	return true;
}
