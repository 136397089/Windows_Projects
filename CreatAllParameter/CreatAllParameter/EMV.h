#pragma once
#include <list>
#include "Number/NumberBase.h"

struct EMV
{
	StockDataType emv;
	StockDataType emvma;
	StockDataType TemporaryVariable;
	StockDataType highToLowMa;
	EMV():emv(0),emvma(0),TemporaryVariable(1),highToLowMa(1){}
};

class CEMVCal
{
public:
	CEMVCal();
	~CEMVCal();
	void Inition();
	bool GetNextEmv(const DatePriceData& OneDayData,EMV& _data);
	unsigned int EMVPara;
	unsigned int MaPara;

	StockDataType frontHighToHLMaSum;
	StockDataType frontEMVSum;
	StockDataType frontHighDivideLowSum;
	StockDataType frontVolSum;
	list<EMV> EMVList;
	list<DatePriceData> RecordData;
};

