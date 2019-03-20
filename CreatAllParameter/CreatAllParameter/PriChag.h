#pragma once
#ifndef PRICECHANGE_H
#define PRICECHANGE_H

#include "Number\NumberBase.h"


class CChagRate
{
public:
	CChagRate();
	~CChagRate();

	bool GetNextChangeRate(const tyStockData& OneDayData, float& changeRate);

	bool Inition();
private:
	float frontValue;
};

#endif
