#include "stdafx.h"
#include <cmath>
#include "DMI.h"

CDMI::CDMI():
DMPMA(14),
DMMMA(14),
TRMA(14),
DayADXMA(6),//M2
_ADXRMA(6)//M2
{
	M1 = 14;//DI��������
	M2 = 6;//ADXR��������
}


CDMI::~CDMI()
{
}


bool CDMI::GetNextDMI(const SinCyclePriceData& TodayDayData, Dmi& mDmi)
{
	DMTR _SigDMTR;
	StockDataType _DayADX;
	//1.1������յ���������+DM��
	StockDataType HD = TodayDayData._High - _FrontPriceData._High;
	StockDataType LD = _FrontPriceData._Low - TodayDayData._Low;
	if (HD > LD && HD>0)	_SigDMTR.DMP = HD;
	//1.2������յ��½�����DM��
	if ((LD) > HD && LD>0) _SigDMTR.DMM = LD;
	//1.3������յ���ʵ������TR��
	StockDataType A, B, C;
	A = TodayDayData._High - TodayDayData._Low;
	B = abs(TodayDayData._High - _FrontPriceData._Close);
	C = abs(TodayDayData._Low - _FrontPriceData._Close);
	_SigDMTR.TR = max(max(A, B), C);
	//1.4����DMTR���
	DMPMA.GetNextMA(_SigDMTR.DMP);
	DMMMA.GetNextMA(_SigDMTR.DMM);
	TRMA.GetNextMA(_SigDMTR.TR);
	//1.5����DMI����Ҫ���ص�DIP��DIN
	mDmi._DIP = (DMPMA.GetDataMovingSum() / TRMA.GetDataMovingSum()) * 100;
	mDmi._DIN = (DMMMA.GetDataMovingSum() / TRMA.GetDataMovingSum()) * 100;
	//1.6������Ҫ���ص�ADXƽ��ֵ
	_DayADX = abs(mDmi._DIP - mDmi._DIN) / (mDmi._DIP + mDmi._DIN) * 100;
	//1.7��ADX
	mDmi._ADX = DayADXMA.GetNextMA(_DayADX);
	//1.8��ADXR
	mDmi._ADXR = _ADXRMA.GetRefMA(mDmi._ADX);
	 //��¼���յļ۸������´μ���
	_FrontPriceData = TodayDayData;

	return true;
}

bool CDMI::Inition()
{
	M1 = 12;//DI��������
	M2 = 2;//ADXR��������

	_FrontPriceData.Inition();
	DayADXMA.Inition();
	_ADXRMA.Inition();
	DMPMA.Inition();
	DMMMA.Inition();
	TRMA.Inition();

	return true;
}
