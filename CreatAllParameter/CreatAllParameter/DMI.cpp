#include "stdafx.h"
#include <cmath>
#include "DMI.h"

CDMI::CDMI()
{
	M1 = 14;//DI��������
	M2 = 6;//ADXR��������
	ADXSum = 0.0f;
	_DMTRSum.TR = 0.0f;
	_DMTRSum.DMP = 0.0f;
	_DMTRSum.DMM = 0.0f;
}


CDMI::~CDMI()
{
}


bool CDMI::GetNextDMI(const SinCyclePriceData& TodayDayData, Dmi& mDmi)
{
	DMTR _DayDMTR;
	StockDataType _DayADX;
	//1.1������յ���������+DM��
	_DayDMTR.DMP = 0;
	StockDataType HD = TodayDayData._High - _FrontPriceData._High;
	StockDataType LD = _FrontPriceData._Low - TodayDayData._Low;
	if (HD > LD && HD>0)
		_DayDMTR.DMP = HD;
	//1.2������յ��½�����DM��
	_DayDMTR.DMM = 0;
	if ((LD) > HD && LD>0)
		_DayDMTR.DMM = LD;
	//1.3������յ���ʵ������TR��
	//StockDataType TR;
	StockDataType A, B, C;
	A = TodayDayData._High - TodayDayData._Low;
	B = abs(TodayDayData._High - _FrontPriceData._Close);
	C = abs(TodayDayData._Low - _FrontPriceData._Close);
	_DayDMTR.TR = max(max(A, B), C);

	HistoryDMTRList.push_back(_DayDMTR);
	//1.4����DMTR���
	_DMTRSum.TR += _DayDMTR.TR;
	_DMTRSum.DMM += _DayDMTR.DMM;
	_DMTRSum.DMP += _DayDMTR.DMP;
	if (HistoryDMTRList.size() > M1)
	{
		_DMTRSum.TR -= HistoryDMTRList.front().TR;
		_DMTRSum.DMM -= HistoryDMTRList.front().DMM;
		_DMTRSum.DMP -= HistoryDMTRList.front().DMP;
		HistoryDMTRList.pop_front();
	}
	//1.5����DMI����Ҫ���ص�DIP��DIN
	mDmi._DIP = (_DMTRSum.DMP / _DMTRSum.TR) * 100;
	mDmi._DIN = (_DMTRSum.DMM / _DMTRSum.TR) * 100;
	//1.6������Ҫ���ص�ADXƽ��ֵ
	_DayADX = abs(mDmi._DIP - mDmi._DIN) / (mDmi._DIP + mDmi._DIN) * 100;
	HistoryDayADX.push_back(_DayADX);
	//1.7ADX���
	if (HistoryDayADX.size() > M2)
	{
		ADXSum -= HistoryDayADX.front();
		HistoryDayADX.pop_front();
	}
	ADXSum += _DayADX;
	mDmi._ADX = ADXSum / (float)M2;
	HistoryADX.push_back(mDmi._ADX);
	//1.8��ADXR
	mDmi._ADXR = mDmi._ADX;
	if (HistoryADX.size() > M2)
	{
		mDmi._ADXR = (mDmi._ADXR + HistoryADX.front()) / 2;
		HistoryADX.pop_front();
	}
	 //��¼���ǵļ۸������´μ���
	_FrontPriceData = TodayDayData;

	return true;
}

bool CDMI::Inition()
{
	M1 = 12;//DI��������
	M2 = 2;//ADXR��������
	ADXSum = 0.0f;
	_DMTRSum.TR = 0.0f;
	_DMTRSum.DMP = 0.0f;
	_DMTRSum.DMM = 0.0f;



	HistoryDMTRList;
	HistoryDayADX;
	ADXSum = 0.0f;
	_FrontPriceData.Inition();

	HistoryDMTRList.clear();
	HistoryDayADX.clear();
	HistoryADX.clear();
	return true;
}
