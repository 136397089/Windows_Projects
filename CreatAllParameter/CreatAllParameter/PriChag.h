#pragma once
#ifndef PRICECHANGE_H
#define PRICECHANGE_H

#include "Number\NumberBase.h"


class CGeneraIindicators
{
public:
	CGeneraIindicators();
	~CGeneraIindicators();

	bool GetNextGI(const SinCyclePriceData& TodayDayData, GeneralIndicators& changeRate);

	bool Inition();
private:
	SinCyclePriceData FrontData;
	int EnableCount;
	int IniEnableCount;
};

#endif
