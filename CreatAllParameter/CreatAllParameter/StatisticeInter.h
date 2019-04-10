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
	//
	bool Inition();
	//
	bool StatisticeDebugFunstion(
		const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate,
		string Stockcode);
	//
	bool Inter(const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate);
	//
	bool GroupFreqStatistice(
		const StockDataTable& daynumber,
		CStateInter& stateinter);
	//
	bool MACD_EDA_Statistice(
		const StockDataTable& allnumber,
		const StateTable& StateList);
	//
	bool Save_MACD_EDA_StatisticeResultTofile(vector<StaticResults>& result);
	//
	bool Ma5_Ma30_Statistice(
		const StockDataTable& allnumber,
		const StatePointsList& StateList);
	//////////////////////////////////////////////////////////////////////////
	//根据模拟CDP的高低点来买卖股票
	void simulation(
		const StockDataTable& daynumber,
		const StockDataTable& weeknumber);
	//对均线_madata1在均线_madata2之上和之下的时长进行统计
	void MALineStatistice(
		const VStockData& _inputdata1,
		const VStockData& _inputdata2,
		vector<unsigned int>& _UpCloseresult,
		vector<unsigned int>& _DownCloseresult);
	//分组统计频次
	template <class T>
	void GroupStatistice(
		const vector<T>& _inputdata1,
		T GroupSize, map<T,
		unsigned int>& _result);
	//void GroupStatistice(const vector<unsigned int>& _inputdata1, unsigned int GroupSize, map<unsigned int, unsigned int>& _result);
	StockDataType Autocorrelation(
		const VStockData& _inputdata,
		unsigned int interval);
	//unsigned int GetIndexFromTimeList(vector<string> _vTimeDay, string time1);
	//
	map<string, StockDataType> filterDataVarList;
	map<string, StockDataType> filterDataMeanList;
	map<string, StockDataType> allDataVarList;
	map<string, StockDataType> allDataMeanList;
private:
	string _LastError;
	const unsigned int moveVarDaySize;
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



#endif
