#include "stdafx.h"
#include "EMV.h"
#include "glog/logging.h"

CEMV::CEMV()
{
	EMVPara = 14;
	MaPara = 9;
	frontEMVSum = 0;
	frontHighDivideLowSum = 0;
	frontHighToHLMaSum = 0;
	frontVolSum = 0;
}


CEMV::~CEMV()
{
}

bool CEMV::GetNextEmv(const DatePriceData& TodayDayData, EMV& _data)
{
	if (RecordData.size() == 0)
	{
		RecordData.push_front(TodayDayData);
		EMVList.push_front(_data);
		return true;
	}
	DatePriceData refData = *RecordData.begin();
	frontVolSum = frontVolSum + TodayDayData._Volume;
	frontHighToHLMaSum = frontHighToHLMaSum + TodayDayData._High - TodayDayData._Low;
	RecordData.push_front(TodayDayData);
	if (RecordData.size() > EMVPara)
	{
		frontVolSum = frontVolSum - RecordData.back()._Volume;
		frontHighToHLMaSum = frontHighToHLMaSum - RecordData.back()._High + RecordData.back()._Low;
		RecordData.pop_back();
	}

	tyStockData TodayHighToHLMaSum = frontHighToHLMaSum;
	tyStockData TodayVolSum = frontVolSum;

	tyStockData volME = TodayVolSum / TodayDayData._Volume;
	_data.highToLowMa = TodayHighToHLMaSum / RecordData.size();
	tyStockData MID = 100 * (TodayDayData._High + TodayDayData._Low - (refData._High + refData._Low)) / (TodayDayData._High + TodayDayData._Low);
	_data.TemporaryVariable = 0;
	if (TodayDayData._High != TodayDayData._Low)
		_data.TemporaryVariable = MID * volME * (TodayDayData._High - TodayDayData._Low);

	if (EMVList.size() > EMVPara)
	{
		frontHighDivideLowSum = frontHighDivideLowSum - EMVList.back().TemporaryVariable / EMVList.back().highToLowMa;
		frontEMVSum = frontEMVSum - EMVList.back().emv;
		EMVList.pop_back();
	}
	frontHighDivideLowSum = frontHighDivideLowSum + _data.TemporaryVariable / _data.highToLowMa;
	frontEMVSum = frontEMVSum + TodayHighToHLMaSum / EMVPara;

	tyStockData TodayEMVSun = frontEMVSum;
	tyStockData TodayHighDivideLowSum = frontHighDivideLowSum;

	_data.emv = TodayHighDivideLowSum / EMVPara;
	_data.emvma = TodayEMVSun / MaPara;
	EMVList.push_front(_data);
	

	return true;
}
