#pragma once
#ifndef STATEINTER_H
#define STATEINTER_H

#include "StaticRecord.h"
#include "StaticIntegral.h"
#include "CLocalRecordTool.h"
#include "TrandRecord.h"
#include "number/NumberInter.h"



class CStateAnalysisInter
{
public:
	CStateAnalysisInter();
	~CStateAnalysisInter();

	void Inter(StockDataTable& allnumber, const string filename);

	bool GetOneNumberState(const VStockData& _datafast, const VStockData& _dataslow, StateTable& _statetable);
	IndexStateMap allIndexStates;
	vector<string>const *_vTimeDay;
	//静态分析
	CStaticRecordTool staticrecord;
	//静态分段积分
	CStaticIntegral IngTool;
	//局部分析
	CLocalRecordTool LocalPartRecord;
	//趋势分析
	CTrandRecordTool trandrecord;
};



class StateIterationAnalysis
{
public:
	StateIterationAnalysis();
	~StateIterationAnalysis();


	void Inition(const string & stockCode);

	bool IterationSingleData(const SigDayTechIndex& data,const string& date);

	void PrintResult();

	bool CheckBeginState();
private:
	string stockCode;
	bool StateIsOK;
	unsigned int FrontLowLocalSize;
	unsigned int FrontLocalSizeVR;
	//静态分析
	CStaticRecordTool StaticRecordVR;
	CStaticRecordTool StaticRecordLow;
	//静态分段积分
	CStaticIntegral IngRecordDiff;
	CStaticIntegral IngRecordLow;
	//局部分析
	CLocalRecordTool LocalRecordVR;
	CLocalRecordTool LocalRecordLow;
	//趋势分析
	CTrandRecordTool TrandRecordVR;
	CTrandRecordTool TrandRecordLow;

};


#endif
