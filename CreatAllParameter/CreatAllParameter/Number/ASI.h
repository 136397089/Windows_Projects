#pragma once
#include "NumberBase.h"
class CAsiCal// :public CNumberManager
{
public:
	CAsiCal();
	~CAsiCal();
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	bool GetNextASI(const SinCyclePriceData& OneDayData, ASI& mFrontMa);

	bool Inition();
private:
	SinCyclePriceData _YesterdayData;
	list<StockDataType> _vSIList;
	list<StockDataType> _vASIList;

	unsigned int _M1;
	unsigned int _M2;
};

