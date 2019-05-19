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
	//��Ҫ������ͳ�Ƹߵ㣬�͵�۸���CDP�����λ��
	bool CountCDPData(const StockDataTable& _inputdata);
	//���CDP�����Ƿ����
	bool CheckCDPData(const StockDataTable& _inputdata);
	//ͨ��index��ȡһ���CDPֵ��������CDPdata��
	bool GetCDPDataByInedx(const StockDataTable& _inputdata, unsigned int index, CDP& CDPdata);
	//��õ�����߼۴���CDP���ĸ�����
	unsigned int GetHighIntervalIndex(StockDataType highData);
	//��õ�����ͼ۴���CDP���ĸ�����
	unsigned int GetLowIntervalIndex(StockDataType lowData);
	//����_inputdata�д�beginIndex��endIndex�����ֵ
	StockDataType FindMax(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);
	//����_inputdata�д�beginIndex��endIndex�����ֵ
	StockDataType FindMin(const VStockData& _inputdata, unsigned int beginIndex, unsigned int endIndex);
	//��������۸���beginIndex��endIndex�۸��еı���
	bool GetProportionOfPrice(
		VStockData& Proportionlist,
		const StockDataTable& _inputdata,
		StockDataType price,
		unsigned int beginIndex,
		unsigned int endIndex);
	//������Ҫ���ܵķ��շ�Χ
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