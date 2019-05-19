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
	float RiseDayRate;//����������ռ��
	float FallDayRate;//����������ռ��
	float MaxRise;//���ǵ�������
	CDate BeginDate;

	StockDataType RiseRate1;//���仯��1
	StockDataType RiseRate2;//���仯��2
	StockDataType RiseRate3;//���仯��3

	StockDataType FallRate1;//���仯��1
	StockDataType FallRate2;//���仯��2
	StockDataType FallRate3;//���仯��3

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
	//���Ҳ�ͬʱ�����priceChangeRate�ĵ��������Сֵ
	bool GetMaxChangeRates(const VStockData& priceChangeRate, StaticResults& maxChangeRate);
	//����ÿ������ڵ�һ��ı仯��
	bool GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate);
	//ͳ�Ƶ�һ���ں�������ʱ����ļ۸�ٷֱ�
	StockDataType GetPricePosition(const StockDataTable& _inputdata);
	//�������ʵľ�ֵ
	bool GetRateOfReturn(const VStockData& priceChangeResult,StockDataType returnrate);
	//GetMaxChangeRates����ʹ�õĲ�����DaySize����ͬ��ͳ��ʱ���
	unsigned int _statisDaySize1;
	unsigned int _statisDaySize2;
	unsigned int _statisDaySize3;
	//
	string _LastError;

};

//���������ʵĺ���
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
