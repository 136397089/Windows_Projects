#pragma once
#include "Number\NumberBase.h"


class CCDPCal// :public CNumberManager
{
public:
	CCDPCal();
	~CCDPCal();
	//////////////////////////////////////////////////////////////////////////
	//ͨ��ǰ�յ�MACD��ý��յ�MACDֵ
	//closeData:���̼�
	//mMacd:ǰ�յ�MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextCDP(const SinCyclePriceData& OneDayData, CDP& mCdp);
};

