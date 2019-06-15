#include "stdafx.h"
#include "EMV.h"
#include "glog/logging.h"

CEMVCal::CEMVCal()
{
	EMVPara = 14;
	MaPara = 9;
	VolSum = 0.0f;
	HighToLowSum = 0.0f;
	EMVResourceSum = 0.0f;
	RefHigh = 0.0f;
	RefLow = 0.0f;
	EMVSum = 0.0f;
}


CEMVCal::~CEMVCal()
{
}

bool CEMVCal::GetNextEmv(const SinCyclePriceData& TodayDayData, EMV& _data)
{
	VolSum += TodayDayData._Volume;
	HistoryVol.push_back(TodayDayData._Volume);
	if (HistoryVol.size() > EMVPara)
	{
		VolSum -= HistoryVol.front();
		HistoryVol.pop_front();
	}
	StockDataType VOLUME = VolSum / EMVPara / TodayDayData._Volume;
	StockDataType MID = 100 * (TodayDayData._High + TodayDayData._Low - RefHigh - RefLow) / (TodayDayData._High + TodayDayData._Low);
	StockDataType TEMP = MID*VOLUME*(TodayDayData._High - TodayDayData._Low);
	if (TodayDayData._High == TodayDayData._Low)
		TEMP = 0.0f;
	StockDataType MAHighToLow = GetMaHighToLow(TodayDayData._High - TodayDayData._Low);
	_data.emv = GetMaTempToHL(TEMP / MAHighToLow);
	_data.emvma = GetEMVMA(_data.emv);
	RefHigh = TodayDayData._High;
	RefLow = TodayDayData._Low;
	return true;
}

void CEMVCal::Inition()
{
	VolSum = 0.0f;
	HighToLowSum = 0.0f;
	EMVResourceSum = 0.0f;
	EMVSum = 0.0f;
	RefHigh = 0.0f;
	RefLow = 0.0f;
	HistoryVol.clear();
	HistoryHighToLow.clear();
	HistoryTEMPtoHL.clear();
	HistoryEMV.clear();

}

StockDataType CEMVCal::GetMaHighToLow(StockDataType HighToLow)
{
	HighToLowSum += HighToLow;
	HistoryHighToLow.push_back(HighToLow);
	if (HistoryHighToLow.size() > EMVPara)
	{
		HighToLowSum -= HistoryHighToLow.front();
		HistoryHighToLow.pop_front();
	}
	return (HighToLowSum / EMVPara);
}

StockDataType CEMVCal::GetMaTempToHL(StockDataType HighToLow)
{
	EMVResourceSum += HighToLow;
	HistoryTEMPtoHL.push_back(HighToLow);
	if (HistoryTEMPtoHL.size() > EMVPara)
	{
		EMVResourceSum -= HistoryTEMPtoHL.front();
		HistoryTEMPtoHL.pop_front();
	}
	return (EMVResourceSum / EMVPara);
}

StockDataType CEMVCal::GetEMVMA(StockDataType _emv)
{
	EMVSum += _emv;
	HistoryEMV.push_back(_emv);
	if (HistoryEMV.size() > MaPara)
	{
		EMVSum -= HistoryEMV.front();
		HistoryEMV.pop_front();
	}
	return (EMVSum / MaPara);

}
