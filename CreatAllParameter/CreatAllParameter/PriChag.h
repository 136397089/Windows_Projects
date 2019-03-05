#pragma once
#ifndef PRICECHANGE_H
#define PRICECHANGE_H

#include "Number\IndexNumber.h"


class CChagRate
{
public:
	CChagRate();
	~CChagRate();

	bool GetNextChangeRate(const tySData& OneDayData, float& changeRate);

	bool Inition();
private:
	float frontValue;
};

#endif
