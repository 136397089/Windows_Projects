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
	M1 = 14;//DI计算周期
	M2 = 6;//ADXR计算周期
}


CDMI::~CDMI()
{
}


bool CDMI::GetNextDMI(const SinCyclePriceData& TodayDayData, Dmi& mDmi)
{
	DMTR _SigDMTR;
	StockDataType _DayADX;
	//1.1计算今日的上升动向（+DM）
	StockDataType HD = TodayDayData._High - _FrontPriceData._High;
	StockDataType LD = _FrontPriceData._Low - TodayDayData._Low;
	if (HD > LD && HD>0)	_SigDMTR.DMP = HD;
	//1.2计算今日的下降动向（DM）
	if ((LD) > HD && LD>0) _SigDMTR.DMM = LD;
	//1.3计算今日的真实波幅（TR）
	StockDataType A, B, C;
	A = TodayDayData._High - TodayDayData._Low;
	B = abs(TodayDayData._High - _FrontPriceData._Close);
	C = abs(TodayDayData._Low - _FrontPriceData._Close);
	_SigDMTR.TR = max(max(A, B), C);
	//1.4计算DMTR求和
	DMPMA.GetNextMA(_SigDMTR.DMP);
	DMMMA.GetNextMA(_SigDMTR.DMM);
	TRMA.GetNextMA(_SigDMTR.TR);
	//1.5计算DMI中需要返回的DIP和DIN
	mDmi._DIP = (DMPMA.GetDataMovingSum() / TRMA.GetDataMovingSum()) * 100;
	mDmi._DIN = (DMMMA.GetDataMovingSum() / TRMA.GetDataMovingSum()) * 100;
	//1.6计算需要返回的ADX平均值
	_DayADX = abs(mDmi._DIP - mDmi._DIN) / (mDmi._DIP + mDmi._DIN) * 100;
	//1.7求ADX
	mDmi._ADX = DayADXMA.GetNextMA(_DayADX);
	//1.8求ADXR
	mDmi._ADXR = _ADXRMA.GetRefMA(mDmi._ADX);
	 //记录今日的价格，用于下次计算
	_FrontPriceData = TodayDayData;

	return true;
}

bool CDMI::Inition()
{
	M1 = 12;//DI计算周期
	M2 = 2;//ADXR计算周期

	_FrontPriceData.Inition();
	DayADXMA.Inition();
	_ADXRMA.Inition();
	DMPMA.Inition();
	DMMMA.Inition();
	TRMA.Inition();

	return true;
}
