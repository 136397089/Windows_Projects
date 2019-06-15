#pragma once
#include <list>
#include "Number/NumberBase.h"


class CEMVCal
{
public:
	CEMVCal();
	~CEMVCal();
	void Inition();
	bool GetNextEmv(const SinCyclePriceData& OneDayData,EMV& _data);
	inline StockDataType GetMaHighToLow(StockDataType HighToLow);
	inline StockDataType GetMaTempToHL(StockDataType HighToLow);
	inline StockDataType GetEMVMA(StockDataType _emv);
	unsigned int EMVPara;
	unsigned int MaPara;

	StockDataType VolSum;
	StockDataType HighToLowSum;
	StockDataType EMVResourceSum;
	StockDataType EMVSum;
	StockDataType RefHigh;
	StockDataType RefLow;
	list<StockDataType> HistoryVol;
	list<StockDataType> HistoryHighToLow;
	list<StockDataType> HistoryTEMPtoHL;
	list<StockDataType> HistoryEMV;
};

