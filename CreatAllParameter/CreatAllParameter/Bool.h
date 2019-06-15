#pragma once

#ifndef BOOL_H
#define BOOL_H

#include "number/NumberBase.h"
#include "MeanVariance.h"

class CBool
{
public:
	CBool();
	~CBool();
	void GetNextBool(const SinCyclePriceData& OneDayData,BOOLIndex& FrontBool);

	bool Inition();

	inline StockDataType GetRePrice1(const SinCyclePriceData& OneDayData);
	inline StockDataType GetRePrice2(const SinCyclePriceData& OneDayData);

	unsigned int N;
	unsigned int P;
	MeanVar MidMeanVar;
	CMeanVariance MidCalTool;
	MeanVar StdMeanVar;
	CMeanVariance StdCalTool;

	list<SinCyclePriceData> priceList;
};

#endif