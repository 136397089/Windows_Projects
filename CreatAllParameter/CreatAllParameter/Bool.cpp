#include "stdafx.h"
#include "Bool.h"
#include "math.h"

CBool::CBool()
{
	Inition();
}


CBool::~CBool()
{
}

bool CBool::Inition()
{
	N = 26;
	P = 2;

	MidMeanVar.clear();
	StdMeanVar.clear();

	priceList.clear();

	return true;
}

void CBool::GetNextBool(const SinCyclePriceData& OneDayData, BOOLIndex& FrontBool)
{
	priceList.push_back(OneDayData);
	if (priceList.size() > N)
	{
		SinCyclePriceData FristPrice = priceList.front();
		priceList.pop_front();
		MidCalTool.GetMeanVarRemoveData(GetRePrice1(FristPrice), MidMeanVar);
		StdCalTool.GetMeanVarRemoveData(GetRePrice2(FristPrice), StdMeanVar);
	}
	MidCalTool.GetNextMeanVar(GetRePrice1(OneDayData), MidMeanVar);
	StdCalTool.GetNextMeanVar(GetRePrice2(OneDayData), StdMeanVar);

	FrontBool.MidPrice = MidMeanVar.mean;
	FrontBool.PriceSTD = sqrt(StdMeanVar.var);
	FrontBool.Uper = FrontBool.MidPrice + P*FrontBool.PriceSTD;
	FrontBool.Downer = FrontBool.MidPrice - P*FrontBool.PriceSTD;

	return;
}

StockDataType CBool::GetRePrice1(const SinCyclePriceData& OneDayData)
{
	return (OneDayData._Close + OneDayData._High + OneDayData._Low) / 3;
}

StockDataType CBool::GetRePrice2(const SinCyclePriceData& OneDayData)
{
	return (OneDayData._Close + OneDayData._High + OneDayData._Low) / 3;
}
