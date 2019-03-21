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
	bool CountCDPData(const StockDataTable& _data);
	//���CDP�����Ƿ����
	bool CheckCDPData(const StockDataTable& _data);
	//ͨ��index��ȡһ���CDPֵ��������CDPdata��
	bool GetCDPDataByInedx(const StockDataTable& _data, unsigned int index, CDP& CDPdata);
	//��õ�����߼۴���CDP���ĸ�����
	unsigned int GetHighIntervalIndex(tyStockData highData);
	//��õ�����ͼ۴���CDP���ĸ�����
	unsigned int GetLowIntervalIndex(tyStockData lowData);
	//����_data�д�beginIndex��endIndex�����ֵ
	tyStockData FindMax(const VStockData& _data, unsigned int beginIndex, unsigned int endIndex);
	//����_data�д�beginIndex��endIndex�����ֵ
	tyStockData FindMin(const VStockData& _data, unsigned int beginIndex, unsigned int endIndex);
	//��������۸���beginIndex��endIndex�۸��еı���
	bool GetProportionOfPrice(
		VStockData& Proportionlist,
		const StockDataTable& _data,
		tyStockData price,
		unsigned int beginIndex,
		unsigned int endIndex);
	//������Ҫ���ܵķ��շ�Χ
	bool GetRiskOfPrice(
		VStockData& Proportionlist,
		const StockDataTable& _data,
		tyStockData price,
		unsigned int beginIndex,
		unsigned int endIndex);
	string _LastError;

	CDP LastTimeCDP;

	CDPStatistics CDPStatisticeResult;
	CDPStatistics CDPHighResult;
	CDPStatistics CDPLowResult;

	unsigned int highIndex;
	unsigned int lowIndex;
	unsigned int openIndex;
	unsigned int closeIndex;

};

#endif