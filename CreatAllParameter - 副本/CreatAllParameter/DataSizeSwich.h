#pragma once
#include "Number/NumberInter.h"
class CDataSizeSwich
{
public:
	CDataSizeSwich();
	~CDataSizeSwich();

	void Inition();

	bool DayToWeek(const StockDataTable& allstockdata, StockDataTable& returnstockdata);
	bool DayToMonth(const StockDataTable& allstockdata, StockDataTable& returnstockdata);


	string _LastError;
};

