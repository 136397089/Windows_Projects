#include "stdafx.h"
#include "Bool.h"
#include "math.h"

CBool::CBool():
N(26),
P(2)
{
	Inition();
}

CBool::~CBool()
{
}

bool CBool::Inition()
{
	MidMeanVar.clear();
	HLMeanVar.clear();
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
		HLCalTool.GetMeanVarRemoveData(FristPrice._High - FristPrice._Low, HLMeanVar);
	}
	MidCalTool.GetNextMeanVar(GetRePrice1(OneDayData), MidMeanVar);
	MidCalTool.GetNextMeanVar(OneDayData._High - OneDayData._Low, HLMeanVar);


	StockDataType HLStd = sqrt(HLMeanVar.var);
	FrontBool.MidPrice = MidMeanVar.mean;
	FrontBool.PriceSTD = sqrt(MidMeanVar.var);
	FrontBool.Uper = FrontBool.MidPrice + P*FrontBool.PriceSTD;
	FrontBool.Downer = FrontBool.MidPrice - P*FrontBool.PriceSTD;
	FrontBool.cdp = (OneDayData._High + OneDayData._Low + 2 * OneDayData._Close) / 4;
	FrontBool.cdpAUp = FrontBool.cdp + 2 * HLStd;
	FrontBool.cdpNUp = FrontBool.cdp + HLStd;
	FrontBool.cdpNDown = FrontBool.cdp - HLStd;
	FrontBool.cdpADown = FrontBool.cdp - 2 * HLStd;

	return;
}

StockDataType CBool::GetRePrice1(const SinCyclePriceData& OneDayData)
{
	return (OneDayData._Close + OneDayData._High + OneDayData._Low) / 3;
}

