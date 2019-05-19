#include "StdAfx.h"
#include <valarray>
#include <cmath>
#include "KDJ.h"

using namespace std;
CKDJCal::CKDJCal(StockDataType day1, StockDataType day2, StockDataType day3)
	:_Nday(day1),
	_M1(day2),
	_M2(day3)
{ 
}

CKDJCal::~CKDJCal(void)
{
}

bool CKDJCal::GetNextKDJ(const SinDayPriceData& OneDayData, KDJ& mFrontKDJ)
{
	if (_lHigh.size() != _lLow.size() || _lHigh.size() != _lClose.size())
		return false;
	if (_Nday == 0 || _M1 == 0 || _M2 == 0)
		return false;
	//
	StaticSizePush(OneDayData._High, _Nday, _lHigh);
	StaticSizePush(OneDayData._Low, _Nday, _lLow);
	StaticSizePush(OneDayData._Close, _Nday, _lClose);

	if (_lHigh.size() < _Nday)
	{
		mFrontKDJ.K_OF_KDJ = 0;
		mFrontKDJ.D_OF_KDJ = 0;
		mFrontKDJ.J_OF_KDJ = 0;
		return true;
	}
	//找到最大值和最小值
	float HighestN = *max_element(_lHigh.begin(), _lHigh.end());
	float LowestN = *min_element(_lLow.begin(), _lLow.end());
	KDJ m_kdj = mFrontKDJ;
	//计算KDJ
	float RSV = (OneDayData._Close - LowestN) / (HighestN - LowestN) * 100;
	mFrontKDJ.K_OF_KDJ = RSV / _M1 + (_M1 - 1) * m_kdj.K_OF_KDJ / _M1;
	mFrontKDJ.D_OF_KDJ = mFrontKDJ.K_OF_KDJ / _M2 + (_M2 - 1) * m_kdj.D_OF_KDJ / _M2;
	mFrontKDJ.J_OF_KDJ = 3 * mFrontKDJ.K_OF_KDJ - 2 * mFrontKDJ.D_OF_KDJ;
	//KDJ参数有门限在0到100之间
// 	if (mFrontKDJ.K_OF_KDJ > 100)
// 		mFrontKDJ.K_OF_KDJ = 100;
// 	if (mFrontKDJ.D_OF_KDJ > 100)
// 		mFrontKDJ.D_OF_KDJ = 100;
// 	if (mFrontKDJ.J_OF_KDJ > 100)
// 		mFrontKDJ.J_OF_KDJ = 100;
// 	if (mFrontKDJ.K_OF_KDJ < 0)
// 		mFrontKDJ.K_OF_KDJ = 0;
// 	if (mFrontKDJ.D_OF_KDJ < 0)
// 		mFrontKDJ.D_OF_KDJ = 0;
// 	if (mFrontKDJ.J_OF_KDJ < 0)
// 		mFrontKDJ.J_OF_KDJ = 0;


	return true;
}

void CKDJCal::StaticSizePush(StockDataType mdata, float msize, list<StockDataType>& mList)
{
	mList.push_back(mdata);
	if (mList.size() > msize)
		mList.pop_front();
}

void CKDJCal::Inition()
{
	_lHigh.clear();
	_lLow.clear();
	_lClose.clear();
}

bool CKDJCal::StaticGetNextKDJ(
	const vector<SinDayPriceData>& FrontPrice,
	const SinDayPriceData& OneDayData,
	KDJ& mFrontKDJ) const
{
	float HighestN = FrontPrice[0]._High;
	float LowestN = FrontPrice[0]._Low;
	for (unsigned int index = 0; index < FrontPrice.size();index++)
	{
		if (FrontPrice[index]._High > HighestN)
			HighestN = FrontPrice[index]._High;
		if (FrontPrice[index]._Low < LowestN)
			LowestN = FrontPrice[index]._Low;
	}
	if (OneDayData._High > HighestN)
		HighestN = OneDayData._High;
	if (OneDayData._Low < LowestN)
		LowestN = OneDayData._Low;
	KDJ m_kdj = mFrontKDJ;
	float RSV = (OneDayData._Close - LowestN) / (HighestN - LowestN) * 100;
	mFrontKDJ.K_OF_KDJ = RSV / _M1 + (_M1 - 1) * m_kdj.K_OF_KDJ / _M1;
	mFrontKDJ.D_OF_KDJ = mFrontKDJ.K_OF_KDJ / _M2 + (_M2 - 1) * m_kdj.D_OF_KDJ / _M2;
	mFrontKDJ.J_OF_KDJ = 3 * mFrontKDJ.K_OF_KDJ - 2 * mFrontKDJ.D_OF_KDJ;
	//KDJ参数有门限在0到100之间
// 	if (mFrontKDJ.K_OF_KDJ > 100)
// 		mFrontKDJ.K_OF_KDJ = 100;
// 	if (mFrontKDJ.D_OF_KDJ > 100)
// 		mFrontKDJ.D_OF_KDJ = 100;
// 	if (mFrontKDJ.J_OF_KDJ > 100)
// 		mFrontKDJ.J_OF_KDJ = 100;
// 	if (mFrontKDJ.K_OF_KDJ < 0)
// 		mFrontKDJ.K_OF_KDJ = 0;
// 	if (mFrontKDJ.D_OF_KDJ < 0)
// 		mFrontKDJ.D_OF_KDJ = 0;
// 	if (mFrontKDJ.J_OF_KDJ < 0)
// 		mFrontKDJ.J_OF_KDJ = 0;
	return true;
}
