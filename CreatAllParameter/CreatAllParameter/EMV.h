#pragma once
#include <list>
#include "Number/NumberBase.h"
#include "MovingAverage.h"

class CEMVCal
{
public:
	CEMVCal();
	~CEMVCal();
	void Inition();
	bool GetNextEmv(const SinCyclePriceData& OneDayData,EMV& _data);

	CMovingAverage VolMA;
	CMovingAverage HighToLowMA;
	CMovingAverage MaTempToHL;
	CMovingAverage EMVMA;
	StockDataType RefHigh;
	StockDataType RefLow;
};

