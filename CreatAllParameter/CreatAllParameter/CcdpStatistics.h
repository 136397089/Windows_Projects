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


	//��Ҫ������ͳ�Ƹߵ㣬�͵�۸���CDP�����λ��
	bool CountCDPData(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber);
private:
	void Inition();//
	bool CheckCDPDataSize();//���CDP�����Ƿ����
	bool GetCDPDataByInedx(CDP& CDPdata);//ͨ��index��ȡһ���CDPֵ��������CDPdata��
	unsigned int GetIntervalIndex(StockDataType highData);//��õ�����߼۴���CDP���ĸ�����
	void IncreaseCDPStatistice(OCHLSectionCount& CDPGroup);//
	void AnaIncomeRate();
	void IncreaseCDPStatistice(OCHLSectionCount& CDPGroup, vector<StockDataType>& HisSection);//
	StockDataType FindMax(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);//����_inputdata�д�beginIndex��endIndex�����ֵ
	StockDataType FindMin(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);//����_inputdata�д�beginIndex��endIndex�����ֵ
	bool GetProportionOfPrice(	VStockData& Proportionlist,StockDataType price,	unsigned int beginIndex,	unsigned int endIndex);//��������۸���beginIndex��endIndex�۸��еı���
	bool GetRiskOfPrice(VStockData& Proportionlist ,StockDataType price ,unsigned int beginIndex ,unsigned int endIndex);//������Ҫ���ܵķ��շ�Χ
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