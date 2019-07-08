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

bool CKDJCal::GetNextKDJ(const SinCyclePriceData& OneDayData, KDJ& mFrontKDJ)
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
	StockDataType HighestN = *max_element(_lHigh.begin(), _lHigh.end());
	StockDataType LowestN = *min_element(_lLow.begin(), _lLow.end());
	KDJ m_kdj = mFrontKDJ;
	//计算KDJ
	StockDataType RealPrice = OneDayData._Low;// (OneDayData._Close + OneDayData._Open + OneDayData._High + OneDayData._Low) / 4;
	StockDataType RSV = (RealPrice - LowestN) / (HighestN - LowestN) * 100;
	mFrontKDJ.K_OF_KDJ = RSV / _M1 + (_M1 - 1) * m_kdj.K_OF_KDJ / _M1;
	mFrontKDJ.D_OF_KDJ = mFrontKDJ.K_OF_KDJ / _M2 + (_M2 - 1) * m_kdj.D_OF_KDJ / _M2;
	mFrontKDJ.J_OF_KDJ = 3 * mFrontKDJ.K_OF_KDJ - 2 * mFrontKDJ.D_OF_KDJ;


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
	const vector<SinCyclePriceData>& FrontPrice,
	const SinCyclePriceData& OneDayData,
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
	return true;
}

bool CKDJCal::CurrentDataToVecter(vector<StockDataType>& CurrentData, const KDJ& mFrontKDJ)
{
	CurrentData.clear();
	CurrentData.push_back(_Nday);
	CurrentData.push_back(_M1);
	CurrentData.push_back(_M2);
	CurrentData.push_back(mFrontKDJ.K_OF_KDJ);
	CurrentData.push_back(mFrontKDJ.D_OF_KDJ);
	CurrentData.push_back(mFrontKDJ.J_OF_KDJ);

	for (list<StockDataType>::iterator ite = _lHigh.begin(); ite != _lHigh.end(); ite++)
		CurrentData.push_back(*ite);
	for (list<StockDataType>::iterator ite = _lLow.begin(); ite != _lLow.end(); ite++)
		CurrentData.push_back(*ite);
	for (list<StockDataType>::iterator ite = _lClose.begin(); ite != _lClose.end(); ite++)
		CurrentData.push_back(*ite);

	return true;
}

bool CKDJCal::RecoveryDataFromVecter(const vector<StockDataType>& CurrentData, KDJ& mFrontKDJ)
{
	if (CurrentData.size() < 6)
		return false;

	_Nday = CurrentData[0];
	_M1 = CurrentData[1];
	_M2 = CurrentData[2];
	mFrontKDJ.K_OF_KDJ = CurrentData[3];
	mFrontKDJ.D_OF_KDJ = CurrentData[4];
	mFrontKDJ.J_OF_KDJ = CurrentData[5];
	if (CurrentData.size() != 6 + 3 * _Nday)
		return false;
	for (unsigned int i = 6 + (_Nday * 0); i < 6 + (_Nday * 1); i++)
		_lHigh.push_back(CurrentData[i]);
	for (unsigned int i = 6 + (_Nday * 1); i < 6 + (_Nday * 2); i++)
		_lLow.push_back(CurrentData[i]);
	for (unsigned int i = 6 + (_Nday * 2); i < 6 + (_Nday * 3); i++)
		_lClose.push_back(CurrentData[i]);
	return true;
}
