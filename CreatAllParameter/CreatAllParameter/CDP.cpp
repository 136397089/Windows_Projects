#include "stdafx.h"
#include "CDP.h"


CCDPCal::CCDPCal()
{
}


CCDPCal::~CCDPCal()
{
}


bool CCDPCal::GetNextCDP(const SinCyclePriceData& TodayDayData, CDP& mCdp)
{
	mCdp._CDP = (TodayDayData._High + TodayDayData._Low + 2 * TodayDayData._Close) / 4;
	mCdp._AH_High = mCdp._CDP + (TodayDayData._High - TodayDayData._Low);
	mCdp._NH_NormalHigh = 2 * mCdp._CDP - TodayDayData._Low;
	mCdp._AL_Low = mCdp._CDP - (TodayDayData._High - TodayDayData._Low);
	mCdp._NL_NormalLow = 2 * mCdp._CDP - TodayDayData._High;
	mCdp._Lowest = TodayDayData._Close * 0.9;

	return true;
}
