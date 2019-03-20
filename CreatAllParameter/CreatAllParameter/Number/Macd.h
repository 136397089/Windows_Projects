#ifndef _MACD_H
#define _MACD_H

#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include "NumberBase.h"
#include "CommonFunction.h"


using namespace std;
class CMacdManager;

//////////////////////////////////////////////////////////////////////////
//MACD
//
//
//////////////////////////////////////////////////////////////////////////
struct Macd/*:public IndexBaseType*/
{
public:
	Macd() :m12(0), m26(0), diff(0), dea(0), bar(0){}
	tyStockData m12;
	tyStockData m26;
	tyStockData diff;
	tyStockData dea;
	tyStockData bar;
// 		Macd& operator = (const Macd& rhs){
// 		_Index1 = rhs._Index1;
// 		_Index2 = rhs._Index2;
// 		_Index3 = rhs._Index3;
// 		_Index4 = rhs._Index4;
// 		_Index5 = rhs._Index5;
// 		return *this;
// 	}
};



//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CMacdManager// : public CNumberManager
{
public:
	CMacdManager(tyStockData shortsmoothness, tyStockData longsmoothness,tyStockData m);
	~CMacdManager(void);

	//////////////////////////////////////////////////////////////////////////
	//通过前日的MACD获得今日的MACD值
	//closeData:收盘价
	//mMacd:前日的MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextMacd(const DatePriceData& OneDayData, Macd& mMacd);
private:
	const tyStockData shortSmoothness;
	const tyStockData longSmoothness;
	const tyStockData M;
};


#endif
