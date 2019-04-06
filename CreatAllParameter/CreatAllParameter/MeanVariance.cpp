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

bool CMeanVariance::GetChangeRateMeanVar(const VStockData& datalist, MeanVar& meanVar)
{
	for (unsigned int i = 0; i < datalist.size();i++)
	{
		GetNextMeanVar(datalist[i], meanVar);
	}
	return true;
}

bool CMeanVariance::GetChangeRateMeanVar(const VStockData::iterator beginIte, const VStockData::iterator endIte, MeanVar& meanVar)
{
	for (VStockData::iterator ite = beginIte; ite != endIte; ite++)
	{
		GetNextMeanVar(*ite, meanVar);
	}
	return true;
}

bool CMeanVariance::GetChangeRateMeanVar(const StockDataTable& datalist, MeanVar& meanVar)
{
	for (unsigned int i = 1; i < datalist._vTimeDay.size(); i++)
	{
		GetNextMeanVar((datalist._vClose[i] - datalist._vClose[i - 1]) * 100/ datalist._vClose[i - 1], meanVar);
		GetNextMeanVar((datalist._vHigh[i] - datalist._vClose[i - 1]) * 100 / datalist._vClose[i - 1], meanVar);
		GetNextMeanVar((datalist._vLow[i] - datalist._vClose[i - 1]) * 100 / datalist._vClose[i - 1], meanVar);
	}
	return true;
}
