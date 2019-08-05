#include "stdafx.h"
#include "SearchLastDayState.h"
#include "SaveResult.h"
#include "glog/logging.h"
SearchLastDayState::SearchLastDayState()
{
}


SearchLastDayState::~SearchLastDayState()
{
}

bool SearchLastDayState::Inter(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber)
{
	if (!daynumber.GetResourceValue().ChackDataSize())
	{
		return false;
	}
	StockDataTable Tagedata = daynumber.GetResourceValue();
	unsigned int index = Tagedata._vTimeDay.size() - 1;
	CDate lastTime = Tagedata._vDate[index];
	StockDataType DMI_DIN = Tagedata._vTableAllIndex[_eDMI_DIN][index];
	StockDataType DMI_DIP = Tagedata._vTableAllIndex[_eDMI_DIP][index];
	StockDataType DeDMI_ADX = Tagedata._vTableAllIndex[_eDMI_ADX][index] - Tagedata._vTableAllIndex[_eDMI_ADX][index - 1];
	//StockDataType DeMACD_DEA = Tagedata._vTableAllIndex[_eMACD_DEA][index] - Tagedata._vTableAllIndex[_eMACD_DEA][index - 1];
	StockDataType MACD_BAR = Tagedata._vTableAllIndex[_eMACD_BAR][index];

	if (DMI_DIN < 30 && DMI_DIP > 30 && DeDMI_ADX > 0 /*&& DeMACD_DEA > 0*/ && MACD_BAR > 0)
	{
		CSaveCodeList codeliTool;
		codeliTool.AddCode(Tagedata._strStockCode);
		LOG(INFO) << Tagedata._strStockCode << " DMI_DIN:" << DMI_DIN << " DMI_DIP:" << DMI_DIP;
	}
	return true;
}
