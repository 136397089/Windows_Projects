#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStatistice.h"
#include "number/NumberInter.h"
#include "StateInter.h"
#include "StatisticeTool.h"
//��ָ֤��������MACD����Ϊ36 78��diff�ϵ�0�����������Ǹ��ʼӴ�


struct StaticResult
{
};



class CStatisticeInter
{
public:
	CStatisticeInter();
	~CStatisticeInter();
	//
	bool Inition();
	//
	bool Inter(const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate);
	//
	bool GroupFreqStatistice(const StockDataTable& daynumber, CStateInter& stateinter);
	//
	bool MACD_EDA_Statistice(const StockDataTable& allnumber, const StateTable& StateList);
	//
	bool Save_MACD_EDA_StatisticeResultTofile(vector<StaticResults>& result);
	//
	bool Ma5_Ma30_Statistice(const StockDataTable& allnumber, const StatePointsList& StateList);
	//
	void simulation(const StockDataTable& daynumber,const StockDataTable& weeknumber);
	//unsigned int GetIndexFromTimeList(vector<string> _vTimeDay, string time1);
	//
private:
	string _LastError;
};




#endif
