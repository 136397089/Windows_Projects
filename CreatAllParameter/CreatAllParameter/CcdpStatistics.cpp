#include "stdafx.h"
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
#include "NumbersToSql.h"
#include "glog/logging.h"
CcdpStatistics::CcdpStatistics()
{
}


CcdpStatistics::~CcdpStatistics()
{
}

bool CcdpStatistics::CountCDPData(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber)
{
	Inition();
	Inputdata = &daynumber.GetResourceValue();
	inputNumber = &daynumber;
	inputSh = &shnumber;
	StockCode = Inputdata->_strStockCode;
	const VStockData& high = Inputdata->_vHigh;
	const VStockData& low = Inputdata->_vLow;
	const VStockData& _vopen = Inputdata->_vOpen;
	const VStockData& _vclose = Inputdata->_vClose;
	if (!CheckCDPDataSize())	return false;

	Account1.Inition(Inputdata->_strStockCode);
	Account2.Inition(Inputdata->_strStockCode);
	for (index = 10; index < high.size(); index++)
	{
		GetDayPriceByIndex();
		GetCDPDataByInedx(LastTimeCDP);
		AnaIncomeRate();
		highSectionIndex = GetIntervalIndex(high[index]);
		lowSectionIndex = GetIntervalIndex(low[index]);
		openSectionIndex = GetIntervalIndex(_vopen[index]);
		closeSectionIndex = GetIntervalIndex(_vclose[index]);
		IncreaseCDPStatistice(CDPStatisticeResult , HistorySection);
		if (Inputdata->_vTableAllIndex[_eAR][index - 1] > 0 && Inputdata->_vTableAllIndex[_eAR][index - 1] < 50)
			IncreaseCDPStatistice(CDPGroupResult1);
		if (Inputdata->_vTableAllIndex[_eAR][index - 1] >= 50 && Inputdata->_vTableAllIndex[_eAR][index - 1] < 150)
			IncreaseCDPStatistice(CDPGroupResult2);
		if (Inputdata->_vTableAllIndex[_eAR][index - 1] >= 150)
			IncreaseCDPStatistice(CDPGroupResult3);
	} 
	SaveIncomeRateData();
	return true;
}

bool CcdpStatistics::CheckCDPDataSize()
{
	const VStockData& CDP = Inputdata->_vTableAllIndex[_eCDP_CDP];

	if (Inputdata->_vTableAllIndex[_eCDP_AH].size() != CDP.size() ||
		Inputdata->_vTableAllIndex[_eCDP_NH].size() != CDP.size() ||
		Inputdata->_vTableAllIndex[_eCDP_AL].size() != CDP.size() ||
		Inputdata->_vTableAllIndex[_eCDP_NL].size() != CDP.size() ||
		Inputdata->_vTableAllIndex[_eCDP_LOWEST].size() != CDP.size() ||
		Inputdata->_vHigh.size() != CDP.size() ||
		Inputdata->_vLow.size() != CDP.size() ||
		Inputdata->_vClose.size() != CDP.size() ||
		CDP.size() == 0)
	{
		_LastError = "CDP Data size error in function(CountCDPData).";
		return false;
	}
	return true;
}


bool CcdpStatistics::GetCDPDataByInedx( CDP& CDPdata)
{
	if (index >= Inputdata->_vTableAllIndex[_eCDP_CDP].size())
	{
		_LastError = "Index out of cdp size in function(GetCDPDataByInedx).";
		return false;
	}
	const VStockData& AH = Inputdata->_vTableAllIndex[_eCDP_AH];
	const VStockData& NH = Inputdata->_vTableAllIndex[_eCDP_NH];
	const VStockData& CDP = Inputdata->_vTableAllIndex[_eCDP_CDP];
	const VStockData& NL = Inputdata->_vTableAllIndex[_eCDP_NL];
	const VStockData& AL = Inputdata->_vTableAllIndex[_eCDP_AL];
	const VStockData& LEST = Inputdata->_vTableAllIndex[_eCDP_LOWEST];

	CDPdata._AH_High = AH[index];
	CDPdata._NH_NormalHigh = NH[index];
	CDPdata._CDP = CDP[index];
	CDPdata._NL_NormalLow = NL[index];
	CDPdata._AL_Low = AL[index];
	CDPdata._Lowest = LEST[index];
	return true;
}

unsigned int CcdpStatistics::GetIntervalIndex(StockDataType highData)
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


void CcdpStatistics::Inition()
{
	CDPStatisticeResult.clear();
	CDPGroupResult1.clear();
	CDPGroupResult2.clear();
	CDPGroupResult3.clear();
	HistorySection.clear();
	LastTimeCDP._AH_High = 0;
	LastTimeCDP._AL_Low = 0;
	LastTimeCDP._CDP = 0;
	LastTimeCDP._NH_NormalHigh = 0;
	LastTimeCDP._NL_NormalLow = 0;
	StockCode = "";
	highSectionIndex = 0;
	lowSectionIndex = 0;
	openSectionIndex = 0;
	closeSectionIndex = 0;
	_LastError = ""; 
	BuyPermit = false;

}

StockDataType CcdpStatistics::FindMax(const VStockData& _AnaData, unsigned int beginIndex, unsigned int endIndex)
{
	if (endIndex >= _AnaData.size())
		endIndex = _AnaData.size();
	if (beginIndex >= endIndex)
		return 0;
	StockDataType maxData = _AnaData[beginIndex];
	for (unsigned int i = beginIndex; i < endIndex;i++)
	{
		if (_AnaData[i] > maxData)
			maxData = _AnaData[i];
	}
	return maxData;
}

StockDataType CcdpStatistics::FindMin(const VStockData& AnaData, unsigned int beginIndex, unsigned int endIndex)
{
	if (endIndex >= AnaData.size())
		endIndex = AnaData.size();
	if (beginIndex >= endIndex)
		return 0;
	StockDataType minData = AnaData[beginIndex];
	for (unsigned int i = beginIndex; i < endIndex; i++)
	{
		if (AnaData[i] < minData)
			minData = AnaData[i];
	}
	return minData;
}

bool CcdpStatistics::GetProportionOfPrice(
	VStockData& Proportionlist,//保存的结果
	StockDataType price,//目标价格
	unsigned int beginIndex,//开始位置
	unsigned int endIndex)//结束位置
{
	StockDataType maxdata = FindMax(Inputdata->_vHigh, beginIndex, endIndex);
	StockDataType mindata = FindMin(Inputdata->_vLow, beginIndex, endIndex);
	StockDataType Proportion = (maxdata - price) * 100 / (maxdata - mindata);
	Proportionlist.push_back(Proportion);
	return true;
}

bool CcdpStatistics::GetRiskOfPrice(VStockData& Proportionlist, StockDataType price, unsigned int beginIndex, unsigned int endIndex)
{
	StockDataType maxdata = FindMax(Inputdata->_vHigh, beginIndex, endIndex);
	StockDataType mindata = FindMin(Inputdata->_vLow, beginIndex, endIndex);
	StockDataType Proportion = (mindata - price) *100 / price;
	Proportionlist.push_back(Proportion);
	return true;

}

void CcdpStatistics::IncreaseCDPStatistice(OCHLSectionCount& CDPGroup)
{
	CDPGroup.HighPriceIntervalFreq[highSectionIndex] ++;
	CDPGroup.LowPriceIntervalFreq[lowSectionIndex] ++;
	CDPGroup.OpenPriceIntervalFreq[openSectionIndex]++;
	CDPGroup.ClosePriceIntervalFreq[closeSectionIndex]++;
	CDPGroup.colseLine[lowSectionIndex - highSectionIndex] ++;
}
void CcdpStatistics::IncreaseCDPStatistice(OCHLSectionCount& CDPGroup, vector<StockDataType>& Section)
{
	CDPGroup.HighPriceIntervalFreq[highSectionIndex] ++;
	CDPGroup.LowPriceIntervalFreq[lowSectionIndex] ++;
	CDPGroup.OpenPriceIntervalFreq[openSectionIndex]++;
	CDPGroup.ClosePriceIntervalFreq[closeSectionIndex]++;
	CDPGroup.colseLine[lowSectionIndex - highSectionIndex] ++;
	Section.push_back(StockDataType(openSectionIndex * 1000 + closeSectionIndex * 100 + highSectionIndex * 10 + lowSectionIndex));
}


void CcdpStatistics::GetDayPriceByIndex()
{
	oneDayPrice._openData = Inputdata->_vOpen[index];
	oneDayPrice._closeData = Inputdata->_vClose[index];
	oneDayPrice._highData = Inputdata->_vHigh[index];
	oneDayPrice._lowData = Inputdata->_vLow[index];
	oneDayPrice._date.SetDay(Inputdata->_vTimeDay[index]);

	oneDayPrice._frontOpen = Inputdata->_vOpen[index - 1];
	oneDayPrice._frontClose = Inputdata->_vClose[index - 1];
	oneDayPrice._frontHigh = Inputdata->_vHigh[index - 1];
	oneDayPrice._frontLow = Inputdata->_vLow[index - 1];
	oneDayPrice._frontdate.SetDay(Inputdata->_vTimeDay[index - 1]);
}

void CcdpStatistics::SaveIncomeRateData()
{
	IncomeRate.clear();
	Account1.GetIncomeRateList(IncomeRate);
	Account2.GetIncomeRateList(IncomeRate);
	CNumbersToSql SQLTool;
	SQLTool.IniMysqlTool();
	vector<string> typeString;
	typeString.push_back("1");
	SQLTool.SaveProGroupRate("CDPIncomeRate", StockCode, IncomeRate, typeString);
	LOG(INFO) << "Income Rate:" << Account1.GetCurrentIncomeRate() * Account2.GetCurrentIncomeRate()
		<< " Count:" << Account1.GetCount() + Account2.GetCount();
}

void CcdpStatistics::AnaIncomeRate()
{
	StockDataTable* _shNumber = &inputSh->GetResourceValue();
	unsigned int shIndex = _shNumber->GetCloselyFrontTimeIndexByDate(Inputdata->_vDate[index]);
	if ( Inputdata->_vTableAllIndex[_eDMI_DIN][index - 1] < 20 
		&& Inputdata->_vTableAllIndex[_eDMI_DIP][index - 1] < 40
		&& Inputdata->_vTableAllIndex[_eMACD_DIFF][index - 1] - Inputdata->_vTableAllIndex[_eMACD_DIFF][index - 2] > 0
// 		&& Inputdata->_vTableAllIndex[_eVRMA][index - 1] - Inputdata->_vTableAllIndex[_eVRMA][index - 2] > 0
		&& Inputdata->_vTableAllIndex[_eLimitDownTime][index - 1] < 5
		&& Inputdata->_vTableAllIndex[_eDataEnable][index] > 0.5f )
		BuyPermit = true;
	else
		BuyPermit = false;

	if (Inputdata->_vTableAllIndex[_eDataEnable][index] < 0.5f)
	{
		BuyPermit = false;
		Account1.ClearHoldStockPrice();
		Account2.ClearHoldStockPrice();
	}
	if (   Account1.CheckHoldIncomeRate(Inputdata->_vLow[index]) <= 0.8999
		|| Account2.CheckHoldIncomeRate(Inputdata->_vLow[index]) <= 0.8999
		|| Inputdata->_vTableAllIndex[_ePriceChangeRate][index] >= 0.0999
		|| Inputdata->_vTableAllIndex[_ePriceChangeRate][index] >= 0.0999)
		BuyPermit = false;


	StockNumberType upLine = _eCDP_NH;
	StockNumberType dwonLine = _eCDP_NL;
	if (!Account1.IsHoldStock() && BuyPermit
		&& Inputdata->_vLow[index] <= Inputdata->_vTableAllIndex[dwonLine][index - 1])
		Account1.BuyStock(Inputdata->_vTableAllIndex[dwonLine][index - 1], Inputdata->_vDate[index]);
	else if (!Account2.IsHoldStock() && BuyPermit
		&& Inputdata->_vLow[index] <= Inputdata->_vTableAllIndex[dwonLine][index - 1])
		Account2.BuyStock(Inputdata->_vTableAllIndex[dwonLine][index - 1], Inputdata->_vDate[index]);

	if (Account1.IsHoldStock() && Inputdata->_vHigh[index] >= Inputdata->_vTableAllIndex[upLine][index - 1])
		Account1.SellStock(Inputdata->_vTableAllIndex[upLine][index - 1], Inputdata->_vDate[index]);
	else if (Account2.IsHoldStock() && Inputdata->_vHigh[index] >= Inputdata->_vTableAllIndex[upLine][index - 1])
		Account2.SellStock(Inputdata->_vTableAllIndex[upLine][index - 1], Inputdata->_vDate[index]);


	if (!BuyPermit && Account1.IsHoldStock())
		Account1.SellStock((Inputdata->_vClose[index - 1]) , Inputdata->_vDate[index]);
	if (!BuyPermit && Account2.IsHoldStock())
		Account2.SellStock((Inputdata->_vClose[index - 1]), Inputdata->_vDate[index]);
}

void OCHLSectionCount::clear()
{
	for (unsigned int i = 0; i < CDPSTATISTICESIZE; i++)
	{
		HighPriceIntervalFreq[i] = 0;
		LowPriceIntervalFreq[i] = 0;
		OpenPriceIntervalFreq[i] = 0;
		ClosePriceIntervalFreq[i] = 0;
		colseLine[i] = 0;
	}
}

OCHLSectionCount::OCHLSectionCount()
{
	clear();
}
