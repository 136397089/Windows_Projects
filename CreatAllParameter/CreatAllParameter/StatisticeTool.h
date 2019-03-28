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

	tyStockData RiseRate1;//���仯��1
	tyStockData RiseRate2;//���仯��2
	tyStockData RiseRate3;//���仯��3

	tyStockData FallRate1;//���仯��1
	tyStockData FallRate2;//���仯��2
	tyStockData FallRate3;//���仯��3

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
	//ͳ�Ʋ�ͬʱ��ε����仯��
	bool GetMaxChangeRates(const VStockData& priceChangeRate, StaticResults& maxChangeRate);

// 	//ͳ�Ƶ�һ���ں�������ʱ����ļ۸�ٷֱ�
// 	float FristDayPricePosition(const AllStockData& newdata);
	//ͳ��ÿ������ڵ�һ��ı仯��
	bool GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate);
	//ͳ�Ƶ�һ���ں�������ʱ����ļ۸�ٷֱ�
	tyStockData GetPricePosition(const StockDataTable& _inputdata);
	//�������ʵľ�ֵ
	bool GetRateOfReturn(const VStockData& priceChangeResult,tyStockData returnrate);
	//GetMaxChangeRates����ʹ�õĲ�����DaySize����ͬ��ͳ��ʱ���
	unsigned int _statisDaySize1;
	unsigned int _statisDaySize2;
	unsigned int _statisDaySize3;
	//
	string _LastError;

};

#endif
