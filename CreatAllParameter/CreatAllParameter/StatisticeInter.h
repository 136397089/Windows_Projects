#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStatistice.h"
#include "number/NumberInter.h"
#include "StateInter.h"
#include "StatisticeTool.h"
#include "MeanVariance.h"
#include "Group.h"

//��ָ֤��������MACD����Ϊ36 78��diff�ϵ�0�����������Ǹ��ʼӴ�

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
	bool Inter(StockDataTable& daynumber, StockDataTable& weeknumber, StockDataTable& mounthnumber,
		CStateInter& daystate,	CStateInter& weekstate,CStateInter& monthstate);
	//////////////////////////////////////////////////////////////////////////
	//�Ծ���_madata1�ھ���_madata2֮�Ϻ�֮�µ�ʱ������ͳ��
	//���صĽ������_UpCloseresult��_DownCloseresult����
	//////////////////////////////////////////////////////////////////////////
	void MALineStatistice(const VStockData& _inputdata1, const VStockData& _inputdata2,
		vector<unsigned int>& _UpCloseresult,vector<unsigned int>& _DownCloseresult);
	//////////////////////////////////////////////////////////////////////////
	//
	//������ͳ��
	//////////////////////////////////////////////////////////////////////////
	bool ReturnStatistics(CIndicatorsInterface& daynumber, StockDataTable& shnumber);

private:
	//////////////////////////////////////////////////////////////////////////
	//����ͳ��Ƶ��
	//////////////////////////////////////////////////////////////////////////
	template <class T>
	void GroupStatistice(	const vector<T>& _inputdata1,T GroupSize, map<T,	unsigned int>& _result);
	//////////////////////////////////////////////////////////////////////////
	//�����ϵ��
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
	bool GetMeanFromSQL(string columns, unsigned int Numtype);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool SaveOneGroupFreqData(const string& stockCoed, Group _GroupType, CHistoryGroup& GroupTool);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& daynumber) const;
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	inline StockDataType GetRandData(int _databegin, int _dataend);
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
