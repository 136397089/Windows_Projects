#include "stdafx.h"
#include "Number/NumberInter.h"
#include "StatisticeInter.h"

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
bool CStatisticeInter::Inter(
	const StockDataTable& daynumber,
	const StockDataTable& weeknumber,
	const StockDataTable& mounthnumber,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	Inition();
	MACD_EDA_Statistice(daynumber, daystate.allIndexStates[_eMACD_BAR]);
	MACD_EDA_Statistice(daynumber, daystate.allIndexStates[_eMACD_BAR]);
	MACD_EDA_Statistice(daynumber, daystate.allIndexStates[_eMACD_BAR]);
	//GroupFreqStatistice(allnumber, stateinter);
	return true;
}


//
bool CStatisticeInter::GroupFreqStatistice(const StockDataTable& stockdata,	CStateInter& stateinter)
{
	CFreqStatistice tempStat;
	changeRateStatis vfreqlist;
	StatePointsList& thePoints = stateinter.allIndexStates[_eMACD_DEA]._trendstate;
	for (unsigned int i = 0; i < thePoints.size(); i++)
	{
		unsigned int timeindex = thePoints[i]._TimeIndex;
		if (thePoints[i]._IndexType == _eTrendBottomRise&&
			stockdata._vDEA[timeindex] > 0)
		{
			unsigned int beginindex = thePoints[i]._TimeIndex;
			StockDataTable tempdata = stockdata.NewDataByIndex(beginindex, beginindex + 30);
			tempStat.Inition();
			tempStat.GetGroupFrqu(tempdata._vPriChaRate, 0.004f, vfreqlist);
		}
	}
	return true;
}

bool CStatisticeInter::MACD_EDA_Statistice(const StockDataTable& allnumber, const StateTable& _State)
{
	const StatePointsList& StaticList = _State._staticstate;
	//输入检查
	if (StaticList.size() < 3)
	{
		_LastError = "StaticList  size error. In MACD_EDA_Statistice.";
		return false;
	}

	CFreqStatistice freqStatTool;
	CStatisticeTool statictool;
	changeRateStatis vfreqlist;

	StockDataTable newdata;
	VStockData PriceChangeResult;

	unsigned int backIndex = 0;
	unsigned int frontIndex = 0;
	StaticResults changerate;
	vector<StaticResults> Upresults;
	vector<StaticResults> Downresults;
	vector<StaticResults> Allresules;
	for (unsigned int i = 0; i < StaticList.size(); i++)
	{
		backIndex = frontIndex;
		frontIndex = StaticList[i]._TimeIndex;

		if (frontIndex <= backIndex)
			continue;
		newdata = allnumber.NewDataByIndex(frontIndex, frontIndex + 30);
		//统计区间上涨和下跌的次数比例
		freqStatTool.Inition();
		freqStatTool.GetGroupFrqu(newdata._vPriChaRate, 0.01f, vfreqlist);
		float rise = freqStatTool.GetFreqByValue(0.00001f, 1.0f);
		float fall = freqStatTool.GetFreqByValue(-1.0f, -0.00001f);
		changerate.RiseDayRate = rise / (float)newdata._vPriChaRate.size();
		changerate.FallDayRate = fall / (float)newdata._vPriChaRate.size();
		changerate.BeginDate.SetDay(newdata._vTimeDay[0]);
		changerate._IndexType = StaticList[i]._IndexType;
		//
		statictool.GetEveryDayChangeRate(newdata._vClose, PriceChangeResult);
		statictool.GetMaxChangeRates(PriceChangeResult, changerate);
		
		if (changerate._IndexType == _eStaticPositiveUpClose)
		{
			Upresults.push_back(changerate);
			Allresules.push_back(changerate);
		}
		else
		{
			Downresults.push_back(changerate);
			Allresules.push_back(changerate);
		}
	}
	Save_MACD_EDA_StatisticeResultTofile(Allresules);
	return true;
}


bool CStatisticeInter::Save_MACD_EDA_StatisticeResultTofile(vector<StaticResults>& result)
{
	fstream outfile("D:\\StockFile\\Log\\StatisticeResult.csv", ios::out);
	outfile
		<< "date" << ","
		<< "RiseRate" << ","
		<< "FallRate" << ","
		<< "RiseRate1" << ","
		<< "RiseRate2" << ","
		<< "RiseRate3" << ","
		<< "FallRate1" << ","
		<< "FallRate2" << ","
		<< "FallRate3" << ","
		<< "IndexType"
		<< endl;
	for (vector<StaticResults>::iterator ite = result.begin(); ite != result.end(); ite++)
	{
		outfile
		<< ite->BeginDate.GetDay()<<","
		<< ite->RiseDayRate << ","
		<< ite->FallDayRate << ","
		<< ite->RiseRate1 << ","
		<< ite->RiseRate2 << ","
		<< ite->RiseRate3 << ","
		<< ite->FallRate1 << ","
		<< ite->FallRate2 << ","
		<< ite->FallRate3 << ","
		<< ite->_IndexType << ",";
		outfile << endl;
	}

	outfile.close();
	return true;
}



