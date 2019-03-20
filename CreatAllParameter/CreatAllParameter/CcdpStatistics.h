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
	string _LastError;

	CDP LastTimeCDP;

	CDPStatistics CDPStatisticeResult;
	CDPStatistics CDPHighResult;
	CDPStatistics CDPLowResult;
};

#endif