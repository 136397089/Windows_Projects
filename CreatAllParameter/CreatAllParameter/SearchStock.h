#pragma once
#include "number/NumberInter.h"
#include "StateInter.h"

class CSearchStock
{
public:
	CSearchStock();
	~CSearchStock();
	bool Inter(const StockDataTable& daydata,
		const StockDataTable& weekdata,
		const StockDataTable& mounthdata,
		CStateInter& daystate,
		CStateInter& weekstate,
		CStateInter& monthstate);
};

