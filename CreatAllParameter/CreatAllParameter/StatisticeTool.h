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

	StockDataType RiseRate1;//最大变化率1
	StockDataType RiseRate2;//最大变化率2
	StockDataType RiseRate3;//最大变化率3

	StockDataType FallRate1;//最大变化率1
	StockDataType FallRate2;//最大变化率2
	StockDataType FallRate3;//最大变化率3

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

struct DayPrice
{
	StockDataType _closeData;
	StockDataType _highData;
	StockDataType _lowData;
	StockDataType _openData;
	CDate _date;

	StockDataType _frontclose;
	StockDataType _fronthigh;
	StockDataType _frontlow;
	StockDataType _frontopen;
	CDate _frontdate;
};

class CStatisticeTool
{
public:
	CStatisticeTool();
	~CStatisticeTool();
	//查找不同时间段内priceChangeRate的单日最大最小值
	bool GetMaxChangeRates(const VStockData& priceChangeRate, StaticResults& maxChangeRate);
	//计算每天相对于第一天的变化率
	bool GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate);
	//统计第一天在后面所有时间里的价格百分比
	StockDataType GetPricePosition(const StockDataTable& _inputdata);
	//返回收率的均值
	bool GetRateOfReturn(const VStockData& priceChangeResult,StockDataType returnrate);
	//GetMaxChangeRates当中使用的参数，DaySize代表不同的统计时间段
	unsigned int _statisDaySize1;
	unsigned int _statisDaySize2;
	unsigned int _statisDaySize3;
	//
	string _LastError;

};

//计算收益率的函数
inline StockDataType GetReturnRate_H(DayPrice oneDayPrice);
inline StockDataType GetReturnRate_L(DayPrice oneDayPrice);
inline StockDataType GetReturnRate_C(DayPrice oneDayPrice);
inline StockDataType GetReturnRate_O(DayPrice oneDayPrice);
inline StockDataType GetLogarithmicReturnRate_H(DayPrice oneDayPrice);
inline StockDataType GetLogarithmicReturnRate_L(DayPrice oneDayPrice);
inline StockDataType GetLogarithmicReturnRate_C(DayPrice oneDayPrice);
inline StockDataType GetLogarithmicReturnRate_O(DayPrice oneDayPrice);
StockDataType GetReturnRate_H(DayPrice oneDayPrice){
	StockDataType frontRealPrice = oneDayPrice._frontclose;//(oneDayPrice._frontclose + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = (oneDayPrice._highData - frontRealPrice) / frontRealPrice * 100;
	if (ReturnRate >= 11.0f || ReturnRate <= -11.0f)
		return 0;
	return ReturnRate;
}
StockDataType GetReturnRate_L(DayPrice oneDayPrice){
	StockDataType frontRealPrice = oneDayPrice._frontclose;//(oneDayPrice._frontclose + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = (oneDayPrice._lowData - frontRealPrice) * 100 / frontRealPrice;
	if (ReturnRate >= 11.0f || ReturnRate <= -11.0f)
		return 0;
	return ReturnRate;
}
StockDataType GetReturnRate_C(DayPrice oneDayPrice){
	StockDataType frontRealPrice = oneDayPrice._frontclose;//(oneDayPrice._frontclose + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = (oneDayPrice._closeData - frontRealPrice) * 100 / frontRealPrice;
	if (ReturnRate >= 11.0f || ReturnRate <= -11.0f)
		return 0;
	return ReturnRate;
}
StockDataType GetReturnRate_O(DayPrice oneDayPrice){
	StockDataType frontRealPrice = oneDayPrice._frontclose;//(oneDayPrice._frontclose + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = (oneDayPrice._openData - frontRealPrice) * 100 / frontRealPrice;
	if (ReturnRate >= 11.0f || ReturnRate <= -11.0f)
		return 0;
	return ReturnRate;
}
StockDataType GetLogarithmicReturnRate_H(DayPrice oneDayPrice){
	StockDataType frontRealPrice = (oneDayPrice._frontopen + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = log((oneDayPrice._highData - frontRealPrice) / frontRealPrice + 1);
// 	if (ReturnRate > 10 || ReturnRate < -10)
// 		return 0;
	return ReturnRate;
}
StockDataType GetLogarithmicReturnRate_L(DayPrice oneDayPrice){
	StockDataType frontRealPrice = (oneDayPrice._frontopen + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = log((oneDayPrice._lowData - frontRealPrice) / frontRealPrice + 1);
// 	if (ReturnRate > 10 || ReturnRate < -10)
// 		return 0.0f;
	return ReturnRate;
}
StockDataType GetLogarithmicReturnRate_C(DayPrice oneDayPrice){
	StockDataType frontRealPrice = (oneDayPrice._frontopen + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = log((oneDayPrice._closeData - frontRealPrice) / frontRealPrice + 1);
// 	if (ReturnRate > 10 || ReturnRate < -10)
// 		return 0;
	return ReturnRate;
}
StockDataType GetLogarithmicReturnRate_O(DayPrice oneDayPrice){
	StockDataType frontRealPrice = (oneDayPrice._frontopen + oneDayPrice._fronthigh + oneDayPrice._frontlow) / 3;
	StockDataType ReturnRate = log((oneDayPrice._openData - frontRealPrice) / frontRealPrice + 1);
// 	if (ReturnRate > 10 || ReturnRate < -10)
// 		return 0;
	return ReturnRate;
}

#endif
