#pragma once
#include <list>
#include "NumberBase.h"

class CTRIXCal// :public CNumberManager
{
public:
	CTRIXCal();
	~CTRIXCal();
	void GetNextTRIX(const SinCyclePriceData& OneDayData, TRIX& mTrix);
	void Inition();
private:
	float GetEMA(const list<StockDataType>& dataList);
	float _ParameterN;
	float _ParameterM;
	float _EMADenominator;
	list<StockDataType> frontClose;
	list<StockDataType> frontTA;
	list<StockDataType> frontTB;
	list<StockDataType> frontTR;
	// vector<TRIX> _vTrix;
	list<StockDataType> TrixData;
	void StaticSizePush(StockDataType mdata, float msize, list<StockDataType>& mList);
};

