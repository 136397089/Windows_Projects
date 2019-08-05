#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStatistice.h"
#include "number/NumberInter.h"
#include "StateInter.h"
#include "StatisticeTool.h"
#include "MeanVariance.h"

//上证指数：日线MACD参数为36 78，diff上到0轴以上是上涨概率加大

#define DATAMINSIZE 10

class CStatisticeInter
{
public:
	CStatisticeInter();
	~CStatisticeInter();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool Inition();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool CDPInter(CIndicatorsInterface& daynumber,	CIndicatorsInterface& shnumber);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool LastDayInter(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber);
	//////////////////////////////////////////////////////////////////////////
	//对均线_madata1在均线_madata2之上和之下的时长进行统计
	//返回的结果放在_UpCloseresult，_DownCloseresult当中
	//////////////////////////////////////////////////////////////////////////
	void MALineStatistice(const VStockData& _inputdata1, const VStockData& _inputdata2,
		vector<unsigned int>& _UpCloseresult,vector<unsigned int>& _DownCloseresult);
	//////////////////////////////////////////////////////////////////////////
	//
	//收益率统计
	//////////////////////////////////////////////////////////////////////////
	bool ReturnRateStatistics(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber);

private:
	//////////////////////////////////////////////////////////////////////////
	//分组统计频次
	//////////////////////////////////////////////////////////////////////////
	template <class T>
	void GroupStatistice(	const vector<T>& _inputdata1,T GroupSize, map<T,	unsigned int>& _result);
	//////////////////////////////////////////////////////////////////////////
	//求处相关系数
	//////////////////////////////////////////////////////////////////////////
	StockDataType Autocorrelation(const VStockData& _inputdata, unsigned int interval);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool SaveMeanVarList_CS(const string& Stockcoed, const MeanVar& _lastMeanVar, const MeanVar& _allDataMeanVar);
	//
	map<string, StockDataType> filterDataVarList;
	map<string, StockDataType> filterDataMeanList;
	map<string, StockDataType> allDataVarList;
	map<string, StockDataType> allDataMeanList;
	FreqListType _FreqList;
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
// 	bool SaveOneGroupFreqData(const string& stockCoed, Group _GroupType, CHistoryGroup& GroupTool);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
// 	void GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& daynumber) const;
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
// 	inline StockDataType GetRandData(int _databegin, int _dataend);
	string _LastError;
};

template <class T>
void CStatisticeInter::GroupStatistice(
	const vector<T>& _inputdata1,
	T GroupSize, map<T,
	unsigned int>& _result)
{
	for (size_t i = 0; i < _inputdata1.size(); i++)
	{
		unsigned int group = floor(_inputdata1[i] / GroupSize);
		_result[group * GroupSize]++;
	}
}

class CRealTimeAna
{
public:
	CRealTimeAna();
	~CRealTimeAna();

	bool AnalysisRealTimeData(StockDataTable& Indicators);

	void Inition();
	//查最后的状态，如果满足则返回true
	bool CheckLastIIndicators(StockDataTable& Indicators);
	void  ShowCurrentCode();
	void  ShowYesterdayCode();
private:
// 	CCurrentGrouping CurrentGroupingTool;
// 	CCurrentGrouping YesterdayGroupingTool;

};
//
static void CheckCurrentStockDataAndGetName(
	StockDataTable& AnaNumber,
	StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode);

#endif
