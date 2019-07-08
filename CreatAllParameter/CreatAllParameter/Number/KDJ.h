#pragma once
#include "NumberBase.h"


#ifndef KDJ_H
#define KDJ_H
///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CKDJCal 
	//public CNumberManager
{
	friend class CNumbersToSql;
public:
	CKDJCal(StockDataType day1 = 90, StockDataType day2 = 30, StockDataType day3 = 30);
	~CKDJCal(void);
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	bool GetNextKDJ(const SinCyclePriceData& OneDayData, KDJ& mFrontKDJ);
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	bool StaticGetNextKDJ(const vector<SinCyclePriceData>& FrontPrice, const SinCyclePriceData& OneDayData, KDJ& mFrontKDJ) const;

	bool CurrentDataToVecter(vector<StockDataType>& CurrentData, const KDJ& mFrontKDJ);

	bool RecoveryDataFromVecter(const vector<StockDataType>& CurrentData, KDJ& mFrontKDJ);

private:
	//CKDJCal();
	void StaticSizePush(StockDataType mdata, float msize, list<StockDataType>& mList);
	list<StockDataType> _lHigh;
	list<StockDataType> _lLow;
	list<StockDataType> _lClose;
	//KDJ指标的参数
	StockDataType _Nday, _M1, _M2;
};
#endif
