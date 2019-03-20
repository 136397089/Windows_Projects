#include "stdafx.h"
#include "CcdpStatistics.h"


CcdpStatistics::CcdpStatistics()
{
}


CcdpStatistics::~CcdpStatistics()
{
}

bool CcdpStatistics::CountCDPData(const StockDataTable& _data)
{
	const VStockData& AH = _data._vAH_High;
	const VStockData& NH = _data._vNH_NormalHigh;
	const VStockData& CDP = _data._vCDP;
	const VStockData& NL = _data._vNL_NormalLow;
	const VStockData& AL = _data._vAL_Low;
	const VStockData& high = _data._vHigh;
	const VStockData& low = _data._vLow;
	const VStockData& _vopen = _data._vOpen;
	const VStockData& _vclose = _data._vClose;
	if (!CheckCDPData(_data))
	{
		_LastError = "CDP Data size error in function(CountCDPData).";
		return false;
	}

	GetCDPDataByInedx(_data, 0, LastTimeCDP);
	unsigned int j = 0;
	unsigned int highIndex = 0;
	unsigned int lowIndex = 0;
	unsigned int openIndex = 0;
	unsigned int closeIndex = 0;
	Initon();
	for (unsigned int i = 1; i < CDP.size(); i++)
	{
		highIndex = GetHighIntervalIndex(high[i]);
		lowIndex = GetLowIntervalIndex(low[i]);
		openIndex = GetHighIntervalIndex(_vopen[i]);
		closeIndex = GetHighIntervalIndex(_vclose[i]);

		CDPStatisticeResult.HighPriceIntervalFreq[highIndex]++;
		CDPStatisticeResult.LowPriceIntervalFreq[lowIndex]++;
		CDPStatisticeResult.OpenPriceIntervalFreq[openIndex]++;
		CDPStatisticeResult.ClosePriceIntervalFreq[closeIndex]++;

		if (highIndex <= lowIndex)
		{
			CDPStatisticeResult.colseLine[lowIndex - highIndex] ++;
		}
		else
		{
			_LastError = "highIndex less than lowIndex.";
			CDPStatisticeResult.colseLine[0]++;
			GetCDPDataByInedx(_data, i, LastTimeCDP);
			continue;
		}
		if (_vopen[i] > LastTimeCDP._CDP && _vopen[i] < LastTimeCDP._NH_NormalHigh && low[i]< LastTimeCDP._CDP)
		{
			CDPHighResult.HighPriceIntervalFreq[highIndex] ++;
			CDPHighResult.LowPriceIntervalFreq[lowIndex] ++;
			CDPHighResult.colseLine[lowIndex - highIndex] ++;
		}
		if (_vopen[i] < LastTimeCDP._AL_Low /*&& _vopen[i] > LastTimeCDP._AL_Low*/)
		{
			CDPLowResult.HighPriceIntervalFreq[highIndex] ++;
			CDPLowResult.LowPriceIntervalFreq[lowIndex] ++;
			CDPLowResult.colseLine[lowIndex - highIndex] ++;
		}
		GetCDPDataByInedx(_data, i, LastTimeCDP);
	}

	return true;
}

bool CcdpStatistics::CheckCDPData(const StockDataTable& _data)
{
	const VStockData& CDP = _data._vCDP;

	if (_data._vAH_High.size() != CDP.size() ||
		_data._vNH_NormalHigh.size() != CDP.size() ||
		_data._vNL_NormalLow.size() != CDP.size() ||
		_data._vAL_Low.size() != CDP.size() ||
		_data._vHigh.size() != CDP.size() ||
		_data._vLow.size() != CDP.size() ||
		_data._vClose.size() != CDP.size() ||
		CDP.size() == 0)
	{
		_LastError = "CDP Data size error in function(CountCDPData).";
		return false;
	}
	return true;
}


bool CcdpStatistics::GetCDPDataByInedx(const StockDataTable& _data, unsigned int index, CDP& CDPdata)
{
	if (index > _data._vCDP.size())
	{
		_LastError = "Index out of cdp size in function(GetCDPDataByInedx).";
		return false;
	}
	const VStockData& AH = _data._vAH_High;
	const VStockData& NH = _data._vNH_NormalHigh;
	const VStockData& CDP = _data._vCDP;
	const VStockData& NL = _data._vNL_NormalLow;
	const VStockData& AL = _data._vAL_Low;

	CDPdata._AH_High = AH[index];
	CDPdata._NH_NormalHigh = NH[index];
	CDPdata._CDP = CDP[index];
	CDPdata._NL_NormalLow = NL[index];
	CDPdata._AL_Low = AL[index];
	return true;
}

unsigned int CcdpStatistics::GetHighIntervalIndex(tyStockData highData)
{
	if (highData >= LastTimeCDP._AH_High)
		return 0;
	else if (highData >= LastTimeCDP._NH_NormalHigh)
		return 1;
	else if (highData >= LastTimeCDP._CDP)
		return 2;
	else if (highData >= LastTimeCDP._NL_NormalLow)
		return 3;
	else if (highData >= LastTimeCDP._AL_Low)
		return 4;
	else
		return 5;

}

unsigned int CcdpStatistics::GetLowIntervalIndex(tyStockData lowData)
{
	if (lowData <= LastTimeCDP._AL_Low)
		return 5;
	else if (lowData <= LastTimeCDP._NL_NormalLow)
		return 4;
	else if (lowData <= LastTimeCDP._CDP)
		return 3;
	else if (lowData <= LastTimeCDP._NH_NormalHigh)
		return 2;
	else if (lowData <= LastTimeCDP._AH_High)
		return 1;
	else
		return 0;
}

void CcdpStatistics::Initon()
{
	CDPStatisticeResult.clear();
}

void CDPStatistics::clear()
{
	for (unsigned int i = 0; i < CDPSTATISTICESIZE; i++)
	{
		HighPriceIntervalFreq[i] = 0;
		LowPriceIntervalFreq[i] = 0;
		colseLine[i] = 0;
		OpenPriceIntervalFreq[i] = 0;
		ClosePriceIntervalFreq[i] = 0;
	}
}

CDPStatistics::CDPStatistics()
{
	clear();
}
