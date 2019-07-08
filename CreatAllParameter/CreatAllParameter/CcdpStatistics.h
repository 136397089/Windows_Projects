#pragma once
#ifndef CDPSTATISTICE_H
#define CDPSTATISTICE_H
#include <string>
#include "Number/NumberBase.h"
#include "Number/commonfunction.h"
#include "Number/numberInter.h"
#include "StockAccountNum.h"
#include "StockDataTable.h"
#include "CDP.h"
using namespace std;

#define CDPSTATISTICESIZE 6
struct  OCHLSectionCount
{
	OCHLSectionCount();
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


	//主要函数，统计高点，低点价格在CDP区间的位置
	bool CountCDPData(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber);
private:
	void Inition();//
	bool CheckCDPDataSize();//检查CDP数据是否出错
	bool GetCDPDataByInedx(CDP& CDPdata);//通过index提取一天的CDP值，保存在CDPdata中
	unsigned int GetIntervalIndex(StockDataType highData);//获得当日最高价处于CDP的哪个区间
	void IncreaseCDPStatistice(OCHLSectionCount& CDPGroup);//
	void AnaIncomeRate();
	void IncreaseCDPStatistice(OCHLSectionCount& CDPGroup, vector<StockDataType>& HisSection);//
	StockDataType FindMax(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);//查找_inputdata中从beginIndex到endIndex的最大值
	StockDataType FindMin(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);//查找_inputdata中从beginIndex到endIndex的最大值
	bool GetProportionOfPrice(	VStockData& Proportionlist,StockDataType price,	unsigned int beginIndex,	unsigned int endIndex);//计算输入价格在beginIndex到endIndex价格当中的比例
	bool GetRiskOfPrice(VStockData& Proportionlist ,StockDataType price ,unsigned int beginIndex ,unsigned int endIndex);//计算需要承受的风险范围
	void SaveIncomeRateData();
	void GetDayPriceByIndex();//



	string _LastError;
	string StockCode;
	CDP LastTimeCDP;

	OCHLSectionCount CDPStatisticeResult;
	OCHLSectionCount CDPGroupResult1;
	OCHLSectionCount CDPGroupResult2;
	OCHLSectionCount CDPGroupResult3;

	vector<StockDataType> HistorySection;
	vector<StockDataType> IncomeRate;
	unsigned int highSectionIndex;
	unsigned int lowSectionIndex;
	unsigned int openSectionIndex;
	unsigned int closeSectionIndex;

	bool BuyPermit;
	StockDataTable* Inputdata;
	CIndicatorsInterface *inputNumber;
	CIndicatorsInterface *inputSh;
	unsigned int index;
	DayPrice oneDayPrice;
	CSimpleAccount Account1;
	CSimpleAccount Account2;
};

#endif