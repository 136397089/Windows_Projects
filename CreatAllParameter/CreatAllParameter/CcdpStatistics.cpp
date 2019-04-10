#include "stdafx.h"
#include "CcdpStatistics.h"


CcdpStatistics::CcdpStatistics()
{
}


CcdpStatistics::~CcdpStatistics()
{
}

bool CcdpStatistics::CountCDPData(const StockDataTable& _inputdata)
{
	const VStockData& AH = _inputdata._vAH_High;
	const VStockData& NH = _inputdata._vNH_NormalHigh;
	const VStockData& CDP = _inputdata._vCDP;
	const VStockData& NL = _inputdata._vNL_NormalLow;
	const VStockData& AL = _inputdata._vAL_Low;
	const VStockData& high = _inputdata._vHigh;
	const VStockData& low = _inputdata._vLow;
	const VStockData& _vopen = _inputdata._vOpen;
	const VStockData& _vclose = _inputdata._vClose;

	if (!CheckCDPData(_inputdata))
	{
		_LastError = "CDP Data size error in function(CountCDPData).";
		return false;
	}

	GetCDPDataByInedx(_inputdata, 0, LastTimeCDP);
	unsigned int j = 0;
	highIndex = 0;
	lowIndex = 0;
	openIndex = 0;
	closeIndex = 0;

	Initon();
	VStockData _vpositions1;
	VStockData _vpositions2;
	VStockData _vpositions3;
	VStockData _vpositions4;
	VStockData _vpositions5;

	for (unsigned int i = 10; i < CDP.size(); i++)
	{
		highIndex = GetHighIntervalIndex(high[i]);
		lowIndex = GetHighIntervalIndex(low[i]);
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
			GetCDPDataByInedx(_inputdata, i, LastTimeCDP);
			continue;
		}
		if (_inputdata._vAR[i - 1] > 0 && _inputdata._vAR[i - 1] < 50)
		{
			CDPGroupOneResult.HighPriceIntervalFreq[highIndex] ++;
			CDPGroupOneResult.LowPriceIntervalFreq[lowIndex] ++;
			CDPGroupOneResult.OpenPriceIntervalFreq[openIndex]++;
			CDPGroupOneResult.ClosePriceIntervalFreq[closeIndex]++;
			CDPGroupOneResult.colseLine[lowIndex - highIndex] ++;
		}
		if (_inputdata._vAR[i - 1] >= 50 && _inputdata._vAR[i - 1] < 150)
		{
			CDPGroupTwoResult.HighPriceIntervalFreq[highIndex] ++;
			CDPGroupTwoResult.LowPriceIntervalFreq[lowIndex] ++;
			CDPGroupTwoResult.OpenPriceIntervalFreq[openIndex]++;
			CDPGroupTwoResult.ClosePriceIntervalFreq[closeIndex]++;
			CDPGroupTwoResult.colseLine[lowIndex - highIndex] ++;
		}
		if (_inputdata._vAR[i - 1] >= 150)
		{
			CDPGroupThreeResult.HighPriceIntervalFreq[highIndex] ++;
			CDPGroupThreeResult.LowPriceIntervalFreq[lowIndex] ++;
			CDPGroupThreeResult.OpenPriceIntervalFreq[openIndex]++;
			CDPGroupThreeResult.ClosePriceIntervalFreq[closeIndex]++;
			CDPGroupThreeResult.colseLine[lowIndex - highIndex] ++;
		}
		GetCDPDataByInedx(_inputdata, i, LastTimeCDP);
	}

	return true;
}

bool CcdpStatistics::CheckCDPData(const StockDataTable& _inputdata)
{
	const VStockData& CDP = _inputdata._vCDP;

	if (_inputdata._vAH_High.size() != CDP.size() ||
		_inputdata._vNH_NormalHigh.size() != CDP.size() ||
		_inputdata._vNL_NormalLow.size() != CDP.size() ||
		_inputdata._vAL_Low.size() != CDP.size() ||
		_inputdata._vHigh.size() != CDP.size() ||
		_inputdata._vLow.size() != CDP.size() ||
		_inputdata._vClose.size() != CDP.size() ||
		CDP.size() == 0)
	{
		_LastError = "CDP Data size error in function(CountCDPData).";
		return false;
	}
	return true;
}


bool CcdpStatistics::GetCDPDataByInedx(const StockDataTable& _inputdata, unsigned int index, CDP& CDPdata)
{
	if (index > _inputdata._vCDP.size())
	{
		_LastError = "Index out of cdp size in function(GetCDPDataByInedx).";
		return false;
	}
	const VStockData& AH = _inputdata._vAH_High;
	const VStockData& NH = _inputdata._vNH_NormalHigh;
	const VStockData& CDP = _inputdata._vCDP;
	const VStockData& NL = _inputdata._vNL_NormalLow;
	const VStockData& AL = _inputdata._vAL_Low;

	CDPdata._AH_High = AH[index];
	CDPdata._NH_NormalHigh = NH[index];
	CDPdata._CDP = CDP[index];
	CDPdata._NL_NormalLow = NL[index];
	CDPdata._AL_Low = AL[index];
	return true;
}

unsigned int CcdpStatistics::GetHighIntervalIndex(StockDataType highData)
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

unsigned int CcdpStatistics::GetLowIntervalIndex(StockDataType lowData)
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

StockDataType CcdpStatistics::FindMax(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex)
{
	if (endIndex >= _inputdata.size())
		endIndex = _inputdata.size();
	if (beginIndex >= endIndex)
		return 0;
	StockDataType maxData = _inputdata[beginIndex];
	for (unsigned int i = beginIndex; i < endIndex;i++)
	{
		if (_inputdata[i] > maxData)
			maxData = _inputdata[i];
	}
	return maxData;
}

StockDataType CcdpStatistics::FindMin(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex)
{
	if (endIndex >= _inputdata.size())
		endIndex = _inputdata.size();
	if (beginIndex >= endIndex)
		return 0;
	StockDataType minData = _inputdata[beginIndex];
	for (unsigned int i = beginIndex; i < endIndex; i++)
	{
		if (_inputdata[i] < minData)
			minData = _inputdata[i];
	}
	return minData;
}

bool CcdpStatistics::GetProportionOfPrice(
	VStockData& Proportionlist,//保存的结果
	const StockDataTable& _inputdata,//所有的数据
	StockDataType price,//目标价格
	unsigned int beginIndex,//开始位置
	unsigned int endIndex)//结束位置
{
	StockDataType maxdata = FindMax(_inputdata._vHigh, beginIndex, endIndex);
	StockDataType mindata = FindMin(_inputdata._vLow, beginIndex, endIndex);
	StockDataType Proportion = (maxdata - price) *100 / price;
	Proportionlist.push_back(Proportion);
	return true;
}

bool CcdpStatistics::GetRiskOfPrice(VStockData& Proportionlist, const StockDataTable& _inputdata, StockDataType price, unsigned int beginIndex, unsigned int endIndex)
{
	StockDataType maxdata = FindMax(_inputdata._vHigh, beginIndex, endIndex);
	StockDataType mindata = FindMin(_inputdata._vLow, beginIndex, endIndex);
	StockDataType Proportion = (mindata - price) *100 / price;
	Proportionlist.push_back(Proportion);
	return true;

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
