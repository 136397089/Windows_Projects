#pragma once
#ifndef STATOSTICS_H
#define STATOSTICS_H

#include "Number/NumberBase.h"
#include "Number/_DMA.h"
#include "Number/Macd.h"
#include "Number/trix.h"
#include "StockAccountNum.h"
#include "Number/KDJ.h"
#include "Number/ASI.h"
#include "CDP.h"
#include "DMI.h"
#include "log.h"
#include "Ma.h"
#include "ARBRCRVRPSY.h"
#include "EMV.h"




//所有的指标－－单日分析时的入参
struct SigDayTechIndex
{
	StockDataType _Pchangerate;
	StockDataType _Volchagrate;
	DMA_DATA _DMAData;
	Macd _MacdData;
	TRIX _TrixData;
	KDJ _Kdj;
	ASI _Asi;
	CDP _Cdp;
	Dmi _Dmi;
	MA _Ma;
	ARBRCRVRPSY _ArBrVrPsy;
	EMV _Emv;
	BOOLIndex _Bool;
	RSI _rsi;
	void Inition();
};


//特征点类型
enum CharaPointType
{
	_eStaticNegaTiveUpClose,//负值区域上穿
	_eStaticNegaTiveDownClose,//负值区域下穿
	_eStaticPositiveUpClose,//正值区域上穿
	_eStaticPositiveDownClose,//正值区域下穿
	//
	_eLocalLow,
	_eLocalHigh,
	_eLocalUpCloseZero,
	_eLocalDownCloseZero,
	//
	_eTrendTopRise_BottomRise,//出现上升底上升
	_eTrendTopFall_BottomFall,//出现顶下降底下降
	_eTrendRangeShrink,//范围收缩－－顶越来越低，底越来越高
	_eTrendRangeEnlarge,//范围扩大－－顶越来越高，同时底越来越低
	_eTrendTopRise,
	_eTrendBottomRise,
	_eTrendTopFall,
	_eTrendBottomFall,
	_eTrendNoRule,//无规律
	//
	_eUndefindType//没有被定义
};

//一个特征点
struct StatePoint
{
	unsigned int _TimeIndex;
	CDate _Date;
	StockDataType _Value;
	CharaPointType _IndexType;
	unsigned int _OtherSideIndex;
	StatePoint()
	{
		_TimeIndex = 0;
		_Value = 0;
		_OtherSideIndex = 0;
		_IndexType = _eUndefindType;
	}
};
typedef vector<StatePoint> StatePointsList;
typedef vector<unsigned int> IndexList;
//单个指标所有的特征位置
class StateTable
{
public:
	
	StatePointsList _staticstate;//静态特征
	StatePointsList _staticIng;//静态积分
	StatePointsList _localstate;//局部特征
	StatePointsList _trendstate;//趋势特征
	//获得状态值的列表
	VStockData GetStaticStatePointsValue() const;
	VStockData GetStaticIntegPointsValue() const;
	VStockData GetLocalStatePointsValue() const;
	VStockData GetTrendStatePointsValue() const;
	//获得index列表
	IndexList GetStaticStateIndex() const;
	IndexList GetStaticIngIndex() const;
	IndexList GetLocalStateIndex() const;
	IndexList GetTrendStateIndex() const;
	//获得日期列表
	vector<string> GetLocalDay()  const;
	vector<string> GetStaticIntegDay()  const;
	vector<string> GetStaticDay() const;
	vector<string> GetTrendDay() const;
	//通过index获得最近的状态
	StatePoint CloselyStaticPoint(unsigned int index) const;
	StatePoint CloselyIntegralPoint(unsigned int index) const;
	StatePoint CloselyLocalPoint(unsigned int index) const;
	StatePoint CloselyTrendPoint(unsigned int index) const;
	//通过日期获得最近的状态
	StatePoint CloselyStaticPoint(CDate index)const;
	StatePoint CloselyIntegralPoint(CDate index)const;
	StatePoint CloselyLocalPoint(CDate index) const;
	StatePoint CloselyTrendPoint(CDate index) const;
	//通用的最近点函数
	StatePoint VersatileCloselyPoint(CDate index, const StatePointsList& PointList) const;
	StatePoint VersatileCloselyPoint(unsigned int index, const StatePointsList& PointList) const;
};



//////////////////////////////////////////////////////////////////////////


#endif

