#pragma once
#include <list>
#include "Number/NumberBase.h"


class CEMVCal
{
public:
	CEMVCal();
	~CEMVCal();
	void Inition();
	bool GetNextEmv(const SinDayPriceData& OneDayData,EMV& _data);
	unsigned int EMVPara;
	unsigned int MaPara;

	StockDataType frontHighToHLMaSum;
	StockDataType frontEMVSum;
	StockDataType frontHighDivideLowSum;
	StockDataType frontVolSum;
	list<EMV> EMVList;
	list<SinDayPriceData> RecordData;
};

