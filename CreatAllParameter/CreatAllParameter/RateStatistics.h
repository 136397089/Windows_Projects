#pragma once
#ifndef RATESTATISTICS_H
#define RATESTATISTICS_H
#include "Number/NumberInter.h"
#include "glog/logging.h"//Log工具
#include "NumbersToSql.h"
#include "Group.h"
#include <map>
#include "StatisticeTool.h"
#include "StateInter.h"

class CRateStatistics
{
public:
	CRateStatistics();
	~CRateStatistics();
	bool AnalysisData(CIndicatorsInterface& daynumber, CIndicatorsInterface& _wholeSHnumber);

	void Inition();
private:
	//
	void GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& daynumber) const;
	//
	bool SaveOneGroupFreqData(const string& stockCoed, Group _GroupType, CHistoryGroup& GroupTool);
	//
	void SaveAllGroupFreqData();
	//
	void SetIndicatorsData();
	//
	void CurrentDataRecordToGroup();
	//
	void GetSigDayTechIndex();
	SigDayTechIndex ResourceValueSigDayData;
	StateIterationAnalysis AnaNumberStateTool;
	StockDataTable* _AnaNumber;
	StockDataTable* _Weeknumber;
	StockDataTable* _Mounthnumber;
	StockDataTable* _shNumber;
	StockDataTable* _shWeekNumber;
	StockDataTable* _shMonNumber;
	vector<NumberEvent> IndtData;
	unsigned int CurrentIndex;

	unsigned int FroShInd;
	unsigned int FroShWeInd;
	unsigned int FroShMonInd;
	unsigned int FroMonInd;
	unsigned int FrWeekInd;

	CHistoryGroup GroupTool;
	CProcessGrouping PGroupTool;
	map<Group, MeanVar> meanVarMiddle;
	DayPrice oneDayPrice;//用于暂存日价格数据
};

#endif