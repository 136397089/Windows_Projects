#pragma once
#ifndef CDPSTATISTICE_H
#define CDPSTATISTICE_H
#include <string>
#include "Number/NumberBase.h"
#include "Number/commonfunction.h"
#include "StockDataTable.h"
#include "CDP.h"
using namespace std;

#define CDPSTATISTICESIZE 6
struct  CDPStatistics
{
	CDPStatistics();
	int HighPriceIntervalFreq[CDPSTATISTICESIZE];
	int LowPriceIntervalFreq[CDPSTATISTICESIZE];
	int OpenPriceIntervalFreq[CDPSTATISTICESIZE];
	int ClosePriceIntervalFreq[CDPSTATISTICESIZE];
	int colseLine[CDPSTATISTICESIZE];
	void clear();
};

class CcdpStatistics
{
public:
	CcdpStatistics();
	~CcdpStatistics();


	void Initon();
	//主要函数，统计高点，低点价格在CDP区间的位置
	bool CountCDPData(const StockDataTable& _data);
	//检查CDP数据是否出错
	bool CheckCDPData(const StockDataTable& _data);
	//通过index提取一天的CDP值，保存在CDPdata中
	bool GetCDPDataByInedx(const StockDataTable& _data, unsigned int index, CDP& CDPdata);
	//获得当日最高价处于CDP的哪个区间
	unsigned int GetHighIntervalIndex(tyStockData highData);
	//获得当日最低价处于CDP的哪个区间
	unsigned int GetLowIntervalIndex(tyStockData lowData);
	string _LastError;

	CDP LastTimeCDP;

	CDPStatistics CDPStatisticeResult;
	CDPStatistics CDPHighResult;
	CDPStatistics CDPLowResult;
};

#endif