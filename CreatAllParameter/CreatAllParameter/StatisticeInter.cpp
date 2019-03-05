#include "stdafx.h"
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "PriceRate.h"
#include "glog/logging.h"

CStatisticeInter::CStatisticeInter()
{
}


CStatisticeInter::~CStatisticeInter()
{
}
//
bool CStatisticeInter::Inition()
{
	return true;
}
//
bool CStatisticeInter::Inter(const AllStockData& allnumber, CStatusInter& statusinter)
{
	MACD_EDA_Statistice(allnumber, statusinter.allIndexStatus[_eMACD_DEA]);
	//GroupFreqStatistice(allnumber, statusinter);
	return true;
}


//
bool CStatisticeInter::GroupFreqStatistice(const AllStockData& allnumber, CStatusInter& statusinter)
{
	CFreqStat tempStat;
	changeRateStatis vfreqlist;
	StatusPointsList& thePoints = statusinter.allIndexStatus[_eMACD_DEA]._trendstatus;
	for (unsigned int i = 0; i < thePoints.size(); i++)
	{
		unsigned int timeindex = thePoints[i]._TimeIndex;
		if (thePoints[i]._IndexType == _eTrendBottomRise&&
			allnumber._vDEA[timeindex] > 0)
		{
			unsigned int beginindex = thePoints[i]._TimeIndex;
			AllStockData tempdata = allnumber.NewDataByIndex(beginindex, beginindex + 30);
			tempStat.Inition();
			tempStat.GetGroupFrqu(tempdata._vPriChaRate, 0.004f, vfreqlist);
		}
	}
	return true;
}

bool CStatisticeInter::MACD_EDA_Statistice(const AllStockData& allnumber, const AllStatus& _Status)
{
	const StatusPointsList& StaticList = _Status._staticstatus;
	// ‰»ÎºÏ≤È
	if (StaticList.size() < 3)
	{
		_LastError = "StaticList  size error. In MACD_EDA_Statistice.";
		return false;
	}

	CFreqStat freqStatTool;
	CPriceRate priceRateTool;
	changeRateStatis vfreqlist;

	AllStockData tempdata;
	VStockData PriceChangeResult;

	unsigned int backIndex = 0;
	unsigned int frontIndex = 0;
	for (unsigned int i = 1; i < StaticList.size(); i++)
	{
		backIndex = frontIndex;
		frontIndex = StaticList[i]._TimeIndex;

		tempdata = allnumber.NewDataByIndex(backIndex, StaticList[i]._TimeIndex);

		freqStatTool.Inition();
		freqStatTool.GetGroupFrqu(tempdata._vPriChaRate, 0.01f, vfreqlist);
		int rise = freqStatTool.GetFreqByValue(0.000001f, 1.0f);
		int fall = freqStatTool.GetFreqByValue(-1.0f, 0.000001f);

		priceRateTool.GetEveryDayChangeRate(tempdata._vCloseData,PriceChangeResult);
	}

	return true;
}



