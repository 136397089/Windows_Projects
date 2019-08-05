#include "stdafx.h"
#include <numeric>
#include <complex>			//复数
#include <cmath>
#include <stdlib.h>
#include <time.h> 
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "DFT.h"				//快速傅立叶
#include "glog/logging.h"		//Log工具
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
#include "FreqStatistice.h"
#include "NumbersToSql.h"
#include "RateStatistics.h"
#include "SearchLastDayState.h"


CRITICAL_SECTION filterDataMeancs;

CStatisticeInter::CStatisticeInter()
{

}


CStatisticeInter::~CStatisticeInter()
{
	DeleteCriticalSection(&filterDataMeancs);
}
//
bool CStatisticeInter::Inition()
{
	InitializeCriticalSection(&filterDataMeancs);
	return true;
}
//
bool CStatisticeInter::CDPInter(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber)
{
	CcdpStatistics CDPStaTool;
	CDPStaTool.CountCDPData(daynumber, shnumber);

	return true;
}

StockDataType CStatisticeInter::Autocorrelation(const VStockData& _inputdata, unsigned int interval)
{
	if (interval >= _inputdata.size())
	{
		return 0;
	}
	StockDataType Average = accumulate(_inputdata.begin(), _inputdata.end(), 0) / _inputdata.size();

	StockDataType Var = 0;
	for (unsigned int i = 0; i < _inputdata.size();i++)
	{
		Var = Var + (_inputdata[i] - Average)*(_inputdata[i] - Average);
	}

	return 0;
}
//对均线_madata1在均线_madata2之上和之下的时长进行统计
//返回的结果放在_UpCloseresult，_DownCloseresult当中
void CStatisticeInter::MALineStatistice(
	const VStockData& _madata1,
	const VStockData& _madata2,
	vector<unsigned int>& _UpCloseresult,
	vector<unsigned int>& _DownCloseresult)
{
	//
	_UpCloseresult.clear();
	_DownCloseresult.clear();
	if (_madata1.size() != _madata2.size() && _madata1.size() < DATAMINSIZE)
	{
		return;
	}
	unsigned int biggercount = 0;
	unsigned int lesscount = 0;
	VStockData::const_iterator iteMA1 = _madata1.begin();
	VStockData::const_iterator iteMA2 = _madata2.begin();
	while (iteMA1 != _madata1.end()){
		if (*iteMA1 >= *iteMA2){
			biggercount++;
			if (lesscount > 0)
				_UpCloseresult.push_back(lesscount);
			lesscount = 0;
		}
		else{
			lesscount++;
			if (biggercount > 0)
				_DownCloseresult.push_back(biggercount);
			biggercount = 0;
		}
		iteMA1++;
		iteMA2++;
	}
	//对不同的时长进行分组统计
	unsigned int groupsize = 2;
	map<unsigned int, unsigned int> _Upresult2;
	map<unsigned int, unsigned int> _Downresult2;
	_Upresult2.clear();
	_Downresult2.clear();
	for (size_t i = 0; i < 240; i++)
	{
		_Upresult2[i*groupsize] = 0;
		_Downresult2[i*groupsize] = 0;
	}
	GroupStatistice(_UpCloseresult, groupsize, _Upresult2);
	GroupStatistice(_DownCloseresult, groupsize, _Downresult2);
	return;
}



bool CStatisticeInter::SaveMeanVarList_CS(
	const string& Stockcode,
	const MeanVar& _lastMeanVar,
	const MeanVar& _allDataMeanVar)
{
	EnterCriticalSection(&filterDataMeancs);
	CFreqStatistice freqTool;
	filterDataVarList[Stockcode] = _lastMeanVar.var;
	filterDataMeanList[Stockcode] = _lastMeanVar.mean;
	allDataVarList[Stockcode] = _allDataMeanVar.var;
	allDataMeanList[Stockcode] = _allDataMeanVar.mean;
	freqTool.SetEmptydate(_FreqList);
	LeaveCriticalSection(&filterDataMeancs);
	return true;
}


bool CStatisticeInter::ReturnRateStatistics(CIndicatorsInterface& daynumber, CIndicatorsInterface& _wholeSHnumber)
{
	CRateStatistics RateTool;
	RateTool.AnalysisData(daynumber, _wholeSHnumber);
	return true;
}

bool CStatisticeInter::LastDayInter(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber)
{
	SearchLastDayState lastTool;
	lastTool.Inter(daynumber, shnumber);
	return true;
}

void CheckCurrentStockDataAndGetName(
	StockDataTable& AnaNumber,
	StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode)
{
	if (monthIndex != 0 && Currentindex >= AnaNumber._vTimeDay.size() - 1)
	{
		if ((AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1] < 0.00f))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((2 * AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 2] > 0))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 1] < 0.00))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((2 * AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 1] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 2] > 0.00f))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((mounthnumber._vTableAllIndex[_eMACD_DIFF][monthIndex] - mounthnumber._vTableAllIndex[_eMACD_DIFF][monthIndex - 1] < 0))
			LOG(WARNING) << StockCode << " Month diff failure to meet conditions.";
		else if ((allDataMeanVar.mean < 0.00f))
			LOG(WARNING) << StockCode << " Day mean failure to meet conditions.";
		else
		{
			LOG(INFO) << " Get stock name:    " << StockCode;
			LOG(INFO) << " Data: " << AnaNumber._vTimeDay[Currentindex] << " inedx: " << Currentindex;
			LOG(INFO) << " MACD DIF: " << AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] << " ," << AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1];
		}
	}
}




CRealTimeAna::CRealTimeAna(){}
CRealTimeAna::~CRealTimeAna(){}
bool CRealTimeAna::AnalysisRealTimeData(StockDataTable& Indicators)
{
	LOG(INFO) << "Run AnalysisRealTimeData to lookup target stock code.";
	vector<StockDataType>& macdData = Indicators._vTableAllIndex[_eMACD_BAR];
	vector<StockDataType>& DEAData = Indicators._vTableAllIndex[_eMACD_DEA];
	vector<StockDataType>& DiffData = Indicators._vTableAllIndex[_eMACD_DIFF];
	vector<StockDataType>& KDJ_KData = Indicators._vTableAllIndex[_eKDJ_K];//K要求小于20
	vector<StockDataType>& KDJ_DData = Indicators._vTableAllIndex[_eKDJ_D];//K要求上升
	vector<string>& timeData = Indicators._vTimeDay;

	if (	   macdData.size() != DEAData.size() || DiffData.size() != KDJ_KData.size()
		|| DiffData.size() != KDJ_DData.size()	|| macdData.size() != DiffData.size()	|| macdData.size() <= 1)
	{
		LOG(INFO) << "Real time data size" << macdData.size() << DEAData.size() << DiffData.size() << KDJ_KData.size() << KDJ_DData.size();
		LOG(ERROR) << Indicators._strStockCode << " AnalysisReal timedata data size error." ;
		return false;
	}
	unsigned int lastIndex = macdData.size() - 1;
	if (CheckLastIIndicators(Indicators))
	{
		LOG(INFO) << "CRealTimeAna Get Stock code:" << Indicators._strStockCode << " Time:" << Indicators._vDate[lastIndex].GetDateTime();
		CNumbersToSql SQLTool;
		SQLTool.IniMysqlTool();
		LOG(INFO) << "Statistics Number of Stock Funds" << SQLTool.CheckStockFundCount(Indicators._strStockCode);
		cout << "CRealTimeAna Get Stock code:" << Indicators._strStockCode << " Time:" << Indicators._vDate[lastIndex].GetDateTime() << endl;
	}
	else
		LOG(INFO) << "Unuseful Stock code:" << Indicators._strStockCode << " size:" << DiffData.size();
	return true;
}

void CRealTimeAna::Inition()
{
}

void CRealTimeAna::ShowCurrentCode()
{

}

void CRealTimeAna::ShowYesterdayCode()
{

}

bool CRealTimeAna::CheckLastIIndicators(StockDataTable& Indicators)
{
	vector<StockDataType>& macdData = Indicators._vTableAllIndex[_eMACD_BAR];
	vector<StockDataType>& DEAData = Indicators._vTableAllIndex[_eMACD_DEA];
	vector<StockDataType>& DiffData = Indicators._vTableAllIndex[_eMACD_DIFF];
	vector<StockDataType>& DMI_DINData = Indicators._vTableAllIndex[_eDMI_DIN];//
	vector<StockDataType>& DMI_DIPData = Indicators._vTableAllIndex[_eDMI_DIP];//
	vector<StockDataType>& DMI_ADXData = Indicators._vTableAllIndex[_eDMI_ADX];//
	vector<string>& timeData = Indicators._vTimeDay;
	return true;
}



//
