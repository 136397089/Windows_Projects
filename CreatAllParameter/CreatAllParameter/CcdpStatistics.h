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
	bool CountCDPData(const StockDataTable& _inputdata);
	//检查CDP数据是否出错
	bool CheckCDPData(const StockDataTable& _inputdata);
	//通过index提取一天的CDP值，保存在CDPdata中
	bool GetCDPDataByInedx(const StockDataTable& _inputdata, unsigned int index, CDP& CDPdata);
	//获得当日最高价处于CDP的哪个区间
	unsigned int GetHighIntervalIndex(StockDataType highData);
	//获得当日最低价处于CDP的哪个区间
	unsigned int GetLowIntervalIndex(StockDataType lowData);
	//查找_inputdata中从beginIndex到endIndex的最大值
	StockDataType FindMax(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);
	//查找_inputdata中从beginIndex到endIndex的最大值
	StockDataType FindMin(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);
	//计算输入价格在beginIndex到endIndex价格当中的比例
	bool GetProportionOfPrice(
		VStockData& Proportionlist,
		const StockDataTable& _inputdata,
		StockDataType price,
		unsigned int beginIndex,
		unsigned int endIndex);
	//计算需要承受的风险范围
	bool GetRiskOfPrice(
		VStockData& Proportionlist,
		const StockDataTable& _inputdata,
		StockDataType price,
		unsigned int beginIndex,
		unsigned int endIndex);
	string _LastError;

	CDP LastTimeCDP;

	CDPStatistics CDPStatisticeResult;
	CDPStatistics CDPGroupOneResult;
	CDPStatistics CDPGroupTwoResult;
	CDPStatistics CDPGroupThreeResult;

	unsigned int highIndex;
	unsigned int lowIndex;
	unsigned int openIndex;
	unsigned int closeIndex;

};

#endif