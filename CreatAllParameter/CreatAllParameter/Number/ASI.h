#pragma once
#include "NumberBase.h"
#include "../MovingAverage.h"
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
	CMovingAverage MovingSI;
	//list<StockDataType> _vSIList;
	list<StockDataType> _vASIList;
	CMovingAverage MovingASIT;

	//unsigned int _M1;
// 	unsigned int _M2;
};

