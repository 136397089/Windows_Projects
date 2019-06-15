#include "StdAfx.h"
#include <algorithm>
#include "Macd.h"



CMacdCal::CMacdCal(StockDataType shortsmoothness, StockDataType longsmoothness,StockDataType m)
:shortSmoothness(shortsmoothness),
longSmoothness(longsmoothness),
M(m)
{
}

CMacdCal::~CMacdCal(void)
{
}


bool CMacdCal::GetNextMacd(
	const SinCyclePriceData& SigDayData,
	Macd& mMacd)
{
	Macd FrontMacd;
	FrontMacd = mMacd;
	mMacd.m12 = (FrontMacd.m12*(shortSmoothness - 1) + (SigDayData._Close) * 2.0f) / (shortSmoothness + 1);
	mMacd.m26 = (FrontMacd.m26*(longSmoothness - 1) + (SigDayData._Close) * 2.0f) / (longSmoothness + 1);
	//mMacd.diff = mMacd.m12 - mMacd.m26;
	mMacd.dif = 11.0f * FrontMacd.m12 / 13.0f - 25.0f * FrontMacd.m26 / 27.0f + (2.0f / 13.0f - 2.0f / 27.0f)*SigDayData._Close;
	mMacd.dea = (FrontMacd.dea*(M - 1) + mMacd.dif * 2) / (M + 1);
	mMacd.bar = 2.0f * (mMacd.dif - mMacd.dea);
	return true;
}

bool CMacdCal::CurrentDataToVecter(vector<StockDataType>& CurrentData, const Macd& mMacd)
{
	CurrentData.clear();
	CurrentData.push_back(mMacd.bar);
	CurrentData.push_back(mMacd.dif);
	CurrentData.push_back(mMacd.dea);
	CurrentData.push_back(mMacd.m12);
	CurrentData.push_back(mMacd.m26);
	CurrentData.push_back(shortSmoothness);
	CurrentData.push_back(longSmoothness);
	CurrentData.push_back(M);
	return true;
}

bool CMacdCal::RecoveryDataFromVecter(const vector<StockDataType>& CurrentData, Macd& mMacd)
{
	if (CurrentData.size() != 8)
		return false;
	mMacd.bar = CurrentData[0];
	mMacd.dif = CurrentData[1];
	mMacd.dea = CurrentData[2];
	mMacd.m12 = CurrentData[3];
	mMacd.m26 = CurrentData[4];
	shortSmoothness = CurrentData[5];
	longSmoothness = CurrentData[6];
	M = CurrentData[7];
	return true;
}

void CMacdCal::Inition()
{
	return;
}
