#pragma once
#ifndef CDPSTATISTICE_H
#define CDPSTATISTICE_H
#include "Number/NumberBase.h"
#include "StockDataTable.h"

struct  CDPStatistics
{
	int IntervalFreq1;
	int IntervalFreq2;
	int IntervalFreq3;
	int IntervalFreq4;
	int IntervalFreq5;
	int IntervalFreq6;

};

class CcdpStatistics
{
public:
	CcdpStatistics();
	~CcdpStatistics();


	void Initon();
	bool SetCDPData(const StockDataTable& daynumber);
};

#endif