#pragma once
#ifndef STATICTOOL_H
#define  STATICTOOL_H
#include <string>
#include <vector>
#include <algorithm>
#include "number/NumberInter.h"
#include "Number/commonfunction.h"
using namespace std;

struct StaticResults
{
	float RiseDayRate;//上涨天数的占比
	float FallDayRate;//上涨天数的占比
	float MaxRise;//上涨的最大比例
	CDate BeginDate;

	tyStockData RiseRate1;//最大变化率1
	tyStockData RiseRate2;//最大变化率2
	tyStockData RiseRate3;//最大变化率3

	tyStockData FallRate1;//最大变化率1
	tyStockData FallRate2;//最大变化率2
	tyStockData FallRate3;//最大变化率3

	CharaPointType _IndexType;
	StaticResults(){
		RiseRate1 = 0;
		RiseRate2 = 0;
		RiseRate3 = 0;
		RiseDayRate = 0;
		FallDayRate = 0;
		MaxRise = 0;
		_IndexType = _eUndefindType;
	}
};
class CStatisticeTool
{
public:
	CStatisticeTool();
	~CStatisticeTool();
	//统计不同时间段的最大变化量
	bool GetMaxChangeRates(const VStockData& priceChangeRate, StaticResults& maxChangeRate);

// 	//统计第一天在后面所有时间里的价格百分比
// 	float FristDayPricePosition(const AllStockData& newdata);
	//统计每天相对于第一天的变化率
	bool GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate);
	//统计第一天在后面所有时间里的价格百分比
	tyStockData GetPricePosition(const StockDataTable& _inputdata);
	//返回收率的均值
	bool GetRateOfReturn(const VStockData& priceChangeResult,tyStockData returnrate);
	//GetMaxChangeRates当中使用的参数，DaySize代表不同的统计时间段
	unsigned int _statisDaySize1;
	unsigned int _statisDaySize2;
	unsigned int _statisDaySize3;
	//
	string _LastError;

};

#endif
