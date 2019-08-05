#ifndef _MACD_H
#define _MACD_H

#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include "NumberBase.h"
#include "CommonFunction.h"
#include "../MovingAverage.h"


using namespace std;
class CMacdCal;

//////////////////////////////////////////////////////////////////////////
//MACD
//
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CMacdCal// : public CNumberManager
{
public:
	CMacdCal(StockDataType shortsmoothness = 12, StockDataType longsmoothness = 26, StockDataType m = 9);
	~CMacdCal(void);

	//////////////////////////////////////////////////////////////////////////
	//ͨ��ǰ�յ�MACD��ý��յ�MACDֵ
	//closeData:���̼�
	//mMacd:ǰ�յ�MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextMacd(const SinCyclePriceData& OneDayData, Macd& mMacd);

	bool CurrentDataToVecter(vector<StockDataType>& CurrentData,const Macd& mMacd);

	bool RecoveryDataFromVecter(const vector<StockDataType>& CurrentData, Macd& mMacd);
	void Inition();
private:
	unsigned int shortSmoothness;
	unsigned int longSmoothness;
	CMovingAverage PriceMa;
	StockDataType M;
};


#endif
