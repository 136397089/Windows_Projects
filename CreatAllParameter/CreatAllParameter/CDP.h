#pragma once
#include "Number\NumberBase.h"


class CCDPCal// :public CNumberManager
{
public:
	CCDPCal();
	~CCDPCal();
	//////////////////////////////////////////////////////////////////////////
	//通过前日的MACD获得今日的MACD值
	//closeData:收盘价
	//mMacd:前日的MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextCDP(const SinCyclePriceData& OneDayData, CDP& mCdp);
};

