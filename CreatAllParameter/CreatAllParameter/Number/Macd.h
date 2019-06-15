#ifndef _MACD_H
#define _MACD_H

#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include "NumberBase.h"
#include "CommonFunction.h"


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
	//通过前日的MACD获得今日的MACD值
	//closeData:收盘价
	//mMacd:前日的MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextMacd(const SinCyclePriceData& OneDayData, Macd& mMacd);

	bool CurrentDataToVecter(vector<StockDataType>& CurrentData,const Macd& mMacd);

	bool RecoveryDataFromVecter(const vector<StockDataType>& CurrentData, Macd& mMacd);
	void Inition();
private:
	StockDataType shortSmoothness;
	StockDataType longSmoothness;
	StockDataType M;
};


#endif
