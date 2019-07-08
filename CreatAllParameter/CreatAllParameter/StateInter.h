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
	//��̬����
	CStaticRecordTool staticrecord;
	//��̬�ֶλ���
	CStaticIntegral IngTool;
	//�ֲ�����
	CLocalRecordTool LocalPartRecord;
	//���Ʒ���
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
	//��̬����
	CStaticRecordTool StaticRecordVR;
	CStaticRecordTool StaticRecordLow;
	//��̬�ֶλ���
	CStaticIntegral IngRecordDiff;
	CStaticIntegral IngRecordLow;
	//�ֲ�����
	CLocalRecordTool LocalRecordVR;
	CLocalRecordTool LocalRecordLow;
	//���Ʒ���
	CTrandRecordTool TrandRecordVR;
	CTrandRecordTool TrandRecordLow;

};


#endif
