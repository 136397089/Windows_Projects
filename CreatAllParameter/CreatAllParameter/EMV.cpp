#include "stdafx.h"
#include "EMV.h"
#include "glog/logging.h"

CEMVCal::CEMVCal():
VolMA(14),//EMVPara
HighToLowMA(14),//EMVPara
MaTempToHL(14),//EMVPara
EMVMA(9)//MaPara
{
	RefHigh = 0.0f;
	RefLow = 0.0f;
}


CEMVCal::~CEMVCal()
{
}

bool CEMVCal::GetNextEmv(const SinCyclePriceData& TodayDayData, EMV& _data)
{
	StockDataType VOLUME = VolMA.GetNextMA(TodayDayData._Volume) / TodayDayData._Volume;
	StockDataType MID = 100 * (TodayDayData._High + TodayDayData._Low - RefHigh - RefLow) / (TodayDayData._High + TodayDayData._Low);
	StockDataType TEMP = MID * VOLUME * (TodayDayData._High - TodayDayData._Low);
	if (TodayDayData._High == TodayDayData._Low)
		TEMP = 0.0f;
	StockDataType MAHighToLow = HighToLowMA.GetNextMA(TodayDayData._High - TodayDayData._Low);
	_data.emv = MaTempToHL.GetNextMA(TEMP / MAHighToLow);
	_data.emvma = EMVMA.GetNextMA(_data.emv);
	RefHigh = TodayDayData._High;
	RefLow = TodayDayData._Low;
	return true;
}

void CEMVCal::Inition()
{
	RefHigh = 0.0f;
	RefLow = 0.0f;
	VolMA.Inition();
	HighToLowMA.Inition();
	MaTempToHL.Inition();
	EMVMA.Inition();
}
