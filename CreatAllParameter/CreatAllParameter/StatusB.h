#pragma once
#ifndef STATOSTICS_H
#define STATOSTICS_H

#include "Number/IndexNumber.h"
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

//趋势判断过程中使用的指标个数
#define MaxSizeOfIndex 7
#define LowPriceIndex 0
#define HighPriceIndex 1
#define MACDIndex 2
#define TRIXIndex 3
#define ASIIndex 4
#define DMAIndex 5
#define KDJIndex 6

//////////////////////////////////////////////////////////////////////////
//特征点所描述的指标
enum IndexGenre
{
	_eLowPrice = LowPriceIndex,
	_eHighPrice = HighPriceIndex,
	_eMACD = MACDIndex,
	_eTRIX = TRIXIndex,
	_eASI = ASIIndex,
	_eDMA = DMAIndex,
	_eKDJ = KDJIndex,
	_eUnDefine
};

//所有的指标－－单日分析时的入参
struct SigDayTechIndex
{
	tySData _Pchangerate;
	tySData _Volchagrate;
	DMA_DATA _DMAData;
	Macd _MacdData;
	TRIX _TrixData;
	KDJ _Kdj;
	ASI _Asi;
	CDP _Cdp;
	Dmi _Dmi;
	MA _Ma;
};
class CStatisticsB
{
public:
	CStatisticsB();
	~CStatisticsB();
};
//指针群，最终状态进行判定时的入参
// struct StockDataPointer
// {
// 	const vector<string>* _pDay;
// 	const VStockData* _pOpenData;
// 	const VStockData* _pCloseData;
// 	const VStockData* _pHigh;
// 	const VStockData* _pLow;
// 	const VStockData* _pMACDValue, *_pMa12, *_pMa26, *_pDiff, *_pDEA;
// 	const VStockData* _pDMAValue, *_pAMAValue;
// 	const VStockData* _pTRIX, *_pTRMA;
// 	const VStockData* _pK, *_pD, *_pJ;
// 	const VStockData* _pAsi, *_pAsit;
// };


//特征点类型
enum CharaPointType
{
	_eStaticNegaTiveUpClose,//负值区域上穿
	_eStaticNegaTiveDownClose,//负值区域下穿
	_eStaticPositiveUpClose,//正值区域上穿
	_eStaticPositiveDownClose,//正值区域下穿
	//
	_eLocalLow,//
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
struct StatusPoint
{
	unsigned int _TimeIndex;
	unsigned int _OtherSideIndex;
	CDate _Date;
	tySData _Value;
	CharaPointType _IndexType;
	StatusPoint()
	{
		_TimeIndex = 0;
		_Value = 0;
		_OtherSideIndex = 0;
		_IndexType = _eUndefindType;
	}
};


typedef vector<StatusPoint> StatusPointsList;
typedef vector<unsigned int> IndexList;

//单个指标所有的特征位置
class AllStatus
{
public:
	
	StatusPointsList _staticstatus;//静态特征
	StatusPointsList _staticIng;//静态积分
	StatusPointsList _localstatus;//局部特征
	StatusPointsList _trendstatus;//趋势特征

	VStockData GetStaticStatusPointsValue() const;
	VStockData GetStaticIntegPointsValue() const;
	VStockData GetLocalStatusPointsValue() const;
	VStockData GetTrendStatusPointsValue() const;

	IndexList GetStaticStausIndex() const;
	IndexList GetStaticIngIndex() const;
	IndexList GetLocalStausIndex() const;
	IndexList GetTrendStausIndex() const;

	vector<string> GetLocalDay()  const;
	vector<string> GetStaticIntegDay()  const;
	vector<string> GetStaticDay() const;
	vector<string> GetTrendDay() const;

	template <class T1,class T2>
	T1 ClosePoint(T1 index) const;
	
	StatusPoint CloselyStaticPoint(unsigned int index) const ;
	StatusPoint CloselyLocalPoint(unsigned int index) const;
	StatusPoint CloselyTrendPoint(unsigned int index) const;

	StatusPoint CloselyStaticPoint(CDate index);
	StatusPoint CloselyLocalPoint(CDate index) const;
	StatusPoint CloselyTrendPoint(CDate index) const;
};


//趋势判定过程中的所有指标状态的简单组合
struct TrendStatus
{
	CharaPointType _AllType[MaxSizeOfIndex];
	TrendStatus(){
		for (unsigned int i = 0; i < MaxSizeOfIndex; ++i)
			_AllType[i] = _eUndefindType;
	}
	CharaPointType& operator [](unsigned int index)
	{
		if (index < MaxSizeOfIndex)
			return _AllType[index];
		else
			return _AllType[MaxSizeOfIndex - 1];
	}
	CharaPointType operator [](unsigned int index)const
	{
		if (index < MaxSizeOfIndex)
			return _AllType[index];
		else
			return _AllType[MaxSizeOfIndex - 1];
	}
};
//////////////////////////////////////////////////////////////////////////

#define MACD_UP		1<<1
#define MACD_NEGATIVE_CLOSE	1<<2
#define DMA_UP		1<<3
#define DMA_NEGATIVE_CLOSE		1<<4
#define TRIX_UP		1<<5
#define TRIX_NEGATIVE_CLOSE	1<<6
#define KDJ_UP		1<<7
#define SetItUp(StateIndex) _State = _State | (StateIndex)
#define SetItDown(StateIndex) _State = _State & (~(StateIndex))
#define IsTrue(StateIndex) ((_State & (StateIndex)) > 0)


#endif

