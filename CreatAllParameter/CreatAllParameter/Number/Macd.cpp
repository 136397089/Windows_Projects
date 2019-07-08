#include "StdAfx.h"
#include <algorithm>
#include "Macd.h"
#include "../MovingAverage.h"


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
	mMacd.m12 = GetEMA(FrontMacd.m12, SigDayData._Close, shortSmoothness);
	mMacd.m26 = GetEMA(FrontMacd.m26, SigDayData._Close, longSmoothness);
	mMacd.dif = mMacd.m12 - mMacd.m26;
	mMacd.dea = GetEMA(FrontMacd.dea, mMacd.dif, M);
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
