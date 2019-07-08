#include "stdafx.h"
#include "PriChag.h"


CGeneraIindicators::CGeneraIindicators():
EnableCount(0),
IniEnableCount(100)
{
}


CGeneraIindicators::~CGeneraIindicators()
{
}

bool CGeneraIindicators::GetNextGI(const SinCyclePriceData& TodayDayData, GeneralIndicators& changeRate)
{
	if (FrontData._Open <=0.001)
	{
		changeRate.VolChangerate = 1.0f;
		changeRate.PriceChangeRate = 1.0f;
		changeRate.Dataable = 0;
		EnableCount = IniEnableCount;
		FrontData = TodayDayData;
		return true;
	}

	if (FrontData._Volume >= 0.01)
		changeRate.VolChangerate = TodayDayData._Volume / FrontData._Volume - 1;
	else
		changeRate.VolChangerate = 0.0f;

	if (FrontData._Volume >= 0.01)
		changeRate.PriceChangeRate = TodayDayData._Close / FrontData._Close - 1;
	else
		changeRate.PriceChangeRate = 0.0f;

	if (changeRate.PriceChangeRate < -0.101 || changeRate.PriceChangeRate > 0.101)
		EnableCount = IniEnableCount;
	if (EnableCount > 0)
	{
		changeRate.Dataable = 0;
		EnableCount--;
	}
	else
		changeRate.Dataable = 1;

	FrontData = TodayDayData;
	return true;
}

bool CGeneraIindicators::Inition()
{
	SinCyclePriceData emptyData;
	FrontData = emptyData;
	EnableCount = 0;
	return true;
}
