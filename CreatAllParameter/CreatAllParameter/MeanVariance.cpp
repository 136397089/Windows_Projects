#include "stdafx.h"
#include "MeanVariance.h"


CMeanVariance::CMeanVariance()
{
	//nNumber = 0;
}


CMeanVariance::~CMeanVariance()
{
}

bool CMeanVariance::GetNextMeanVar(
	StockDataType newData,
	MeanVar& reasultMV)const
{
	reasultMV.n++;
	MeanVar lastData = reasultMV;
	reasultMV.mean = lastData.mean + (newData - lastData.mean) / reasultMV.n;

	reasultMV.Fn = reasultMV.Fn + (newData - lastData.mean)*(newData - reasultMV.mean);
	reasultMV.var = reasultMV.Fn / reasultMV.n;

	return true;
}


bool CMeanVariance::GetMeanVarRemoveData(
	StockDataType removeData, 
	MeanVar& meanVar) const
{
	MeanVar fromData;
	fromData.n = meanVar.n - 1;
	fromData.mean = (meanVar.n * meanVar.mean - removeData) / fromData.n;
	fromData.Fn = meanVar.Fn - (removeData - fromData.mean)*(removeData - meanVar.mean);
	fromData.var = fromData.Fn / fromData.n;
	meanVar = fromData;
	return true;
}

bool CMeanVariance::MeanVarDebugFunction(
	const StockDataTable& datalist,
	MeanVar& meanVar)const
{
	for (unsigned int i = 0; i < datalist._vHigh.size();i++)
	{
		GetNextMeanVar(datalist._vHigh[i], meanVar);
		if (i >= 10)
		{
			GetMeanVarRemoveData(datalist._vHigh[i], meanVar);
		}
	}
	return true;
}



