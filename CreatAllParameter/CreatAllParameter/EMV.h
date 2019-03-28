#pragma once
#include <list>
#include "Number/NumberBase.h"

struct EMV
{
	tyStockData emv;
	tyStockData emvma;
	tyStockData TemporaryVariable;
	tyStockData highToLowMa;
	EMV():emv(0),emvma(0),TemporaryVariable(1),highToLowMa(1){}
};

class CEMV
{
public:
	CEMV();
	~CEMV();
	bool GetNextEmv(const DatePriceData& OneDayData,EMV& _data);
	unsigned int EMVPara;
	unsigned int MaPara;

	tyStockData frontHighToHLMaSum;
	tyStockData frontEMVSum;
	tyStockData frontHighDivideLowSum;
	tyStockData frontVolSum;
	list<EMV> EMVList;
	list<DatePriceData> RecordData;
};

