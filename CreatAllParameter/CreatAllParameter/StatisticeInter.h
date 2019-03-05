#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStat.h"
#include "number/NumberInter.h"
#include "StatusInter.h"

//上证指数：日线MACD参数为36 78，diff上到0轴以上是上涨概率加大


struct StaticResult
{
	float RiseRate;//上涨天数的占比
	float MaxRise;//上涨的最大比例
};

class CStatisticeInter
{
public:
	CStatisticeInter();
	~CStatisticeInter();
	//
	bool Inition();
	//
	bool Inter(const AllStockData& allnumber, CStatusInter& statusinter);
	//
	bool GroupFreqStatistice(const AllStockData& allnumber, CStatusInter& statusinter);
	//
	bool MACD_EDA_Statistice(const AllStockData& allnumber, const AllStatus& StatusList);
	//
	bool Ma5_Ma30_Statistice(const AllStockData& allnumber, const StatusPointsList& StatusList);
	//unsigned int GetIndexFromTimeList(vector<string> _vTimeDay, string time1);
	//
	bool GetMaxChangeRates(const VStockData PriceChangeResult);
	string _LastError;
};




#endif
