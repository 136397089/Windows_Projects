#pragma once
#include "Numbers.h"
#include "DateTool.h"
#include "StateInter.h"

class CStateMachineInter
{
public:
	CStateMachineInter();
	~CStateMachineInter();

	bool Interface(const CDate& time, 
		const StockDataTable& daynumber,
		const StockDataTable& weeknumber,
		const StockDataTable& mounthnumber,
		CStateAnalysisInter& daystate,
		CStateAnalysisInter& weekstate,
		CStateAnalysisInter& monthstate);
};

