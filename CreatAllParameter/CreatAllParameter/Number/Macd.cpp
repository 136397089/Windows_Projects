#include "StdAfx.h"
#include <algorithm>
#include "Macd.h"



CMacdManager::CMacdManager(tySData shortsmoothness, tySData longsmoothness,tySData m)
:shortSmoothness(shortsmoothness),
longSmoothness(longsmoothness),
M(m)
{
}

CMacdManager::~CMacdManager(void)
{
}


bool CMacdManager::GetNextMacd(const DatePriceData& 
	SigDayData, Macd& mMacd)
{
	Macd FrontMacd;
	FrontMacd = mMacd;
	mMacd.m12 = (FrontMacd.m12*(shortSmoothness - 1) + (SigDayData._Close) * 2.0f) / (shortSmoothness + 1);
	mMacd.m26 = (FrontMacd.m26*(longSmoothness - 1) + (SigDayData._Close) * 2.0f) / (longSmoothness + 1);
	mMacd.diff = mMacd.m12 - mMacd.m26;
	mMacd.dea = (FrontMacd.dea*(M - 1) + mMacd.diff * 2) / (M + 1);
	mMacd.bar = 2.0f * (mMacd.diff - mMacd.dea);
	return true;
}
