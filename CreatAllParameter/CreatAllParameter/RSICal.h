#pragma once
#ifndef RSICAL_H
#define RSICAL_H

#include "number/NumberBase.h"

class CRSICal
{
public:
	CRSICal();
	~CRSICal();
	void GetNextRSI(const SinCyclePriceData& OneDayData,RSI& _FrontRSI);

	void Inition();
	SinCyclePriceData REFPrice;
	StockDataType _FrontMax1, _FrontMax2, _FrontMax3;
	StockDataType _FrontABS1, _FrontABS2, _FrontABS3;
	unsigned int N1, N2, N3;
};

#endif
