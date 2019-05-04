#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStatistice.h"
#include "number/NumberInter.h"
#include "StateInter.h"
#include "StatisticeTool.h"
#include "MeanVariance.h"

//��ָ֤��������MACD����Ϊ36 78��diff�ϵ�0�����������Ǹ��ʼӴ�

#define DATAMINSIZE 10




class CStatisticeInter
{
public:
	CStatisticeInter();
	~CStatisticeInter();
	//
	bool Inition();
	//
	bool StatisticeHistoryData(
		const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		const StockDataTable& shnumber,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate,
		string Stockcode);
	bool StatisticeASIData(
		const StockDataTable& daynumber,
		CStateInter& daystate,
		const string& Stockcode);
	//
	bool Inter(const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate);
	//
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
	//����ģ��CDP�ĸߵ͵���������Ʊ
	void simulation(
		const StockDataTable& daynumber,
		const StockDataTable& weeknumber);
	//�Ծ���_madata1�ھ���_madata2֮�Ϻ�֮�µ�ʱ������ͳ��
	void MALineStatistice(
		const VStockData& _inputdata1,
		const VStockData& _inputdata2,
		vector<unsigned int>& _UpCloseresult,
		vector<unsigned int>& _DownCloseresult);
	//����ͳ��Ƶ��
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
	bool SaveMeanVarList_CS(
		const string& Stockcoed,
		const MeanVar& _lastMeanVar,
		const MeanVar& _allDataMeanVar);
	map<string, StockDataType> filterDataVarList;
	map<string, StockDataType> filterDataMeanList;
	map<string, StockDataType> allDataVarList;
	map<string, StockDataType> allDataMeanList;
	FreqListType _FreqList;
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

class CRealTimeAna
{
public:
	CRealTimeAna();
	~CRealTimeAna();

	bool AnalysisRealTimeData(const StockDataTable& number,string stockCode);
private:

};
//
static void CheckCurrentStockDataAndGetName(
	const StockDataTable& AnaNumber,
	const StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode);
//
bool GroupFreqStatistice(
	const StockDataTable& daynumber,
	CStateInter& stateinter);

#endif
