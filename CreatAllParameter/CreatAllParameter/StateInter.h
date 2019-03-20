#pragma once
#ifndef STATEINTER_H
#define STATEINTER_H

#include "StaticRecord.h"
#include "StaticIntegral.h"
#include "CLocalRecordTool.h"
#include "TrandRecord.h"
#include "number/NumberInter.h"


class CStateInter
{
public:
	CStateInter();
	~CStateInter();

	void Inter(const StockDataTable& allnumber, const string filename);

	StateTable GetOneNumberState(const VStockData& _datafast, const VStockData& _dataslow);

	void GetStateByIndex(unsigned int index);
	IndexStateMap allIndexStates;

	vector<string> _vTimeDay;
	StatePointsList _priceLocalState;
// 	AllState MACDState;
// 	AllState DEAState;

};

#endif
