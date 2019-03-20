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

//�����жϹ�����ʹ�õ�ָ�����
#define MaxSizeOfIndex 7
#define LowPriceIndex 0
#define HighPriceIndex 1
#define MACDIndex 2
#define TRIXIndex 3
#define ASIIndex 4
#define DMAIndex 5
#define KDJIndex 6

//////////////////////////////////////////////////////////////////////////
//��������������ָ��
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

//���е�ָ�꣭�����շ���ʱ�����
struct SigDayTechIndex
{
	tyStockData _Pchangerate;
	tyStockData _Volchagrate;
	DMA_DATA _DMAData;
	Macd _MacdData;
	TRIX _TrixData;
	KDJ _Kdj;
	ASI _Asi;
	CDP _Cdp;
	Dmi _Dmi;
	MA _Ma;
};
// class CStatisticsB
// {
// public:
// 	CStatisticsB();
// 	~CStatisticsB();
// };



//����������
enum CharaPointType
{
	_eStaticNegaTiveUpClose,//��ֵ�����ϴ�
	_eStaticNegaTiveDownClose,//��ֵ�����´�
	_eStaticPositiveUpClose,//��ֵ�����ϴ�
	_eStaticPositiveDownClose,//��ֵ�����´�
	//
	_eLocalLow,
	_eLocalHigh,
	_eLocalUpCloseZero,
	_eLocalDownCloseZero,
	//
	_eTrendTopRise_BottomRise,//��������������
	_eTrendTopFall_BottomFall,//���ֶ��½����½�
	_eTrendRangeShrink,//��Χ����������Խ��Խ�ͣ���Խ��Խ��
	_eTrendRangeEnlarge,//��Χ���󣭣���Խ��Խ�ߣ�ͬʱ��Խ��Խ��
	_eTrendTopRise,
	_eTrendBottomRise,
	_eTrendTopFall,
	_eTrendBottomFall,
	_eTrendNoRule,//�޹���
	//
	_eUndefindType//û�б�����
};

//һ��������
struct StatePoint
{
	unsigned int _TimeIndex;
	unsigned int _OtherSideIndex;
	CDate _Date;
	tyStockData _Value;
	CharaPointType _IndexType;
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
//����ָ�����е�����λ��
class StateTable
{
public:
	
	StatePointsList _staticstate;//��̬����
	StatePointsList _staticIng;//��̬����
	StatePointsList _localstate;//�ֲ�����
	StatePointsList _trendstate;//��������
	//���״ֵ̬���б�
	VStockData GetStaticStatePointsValue() const;
	VStockData GetStaticIntegPointsValue() const;
	VStockData GetLocalStatePointsValue() const;
	VStockData GetTrendStatePointsValue() const;
	//���index�б�
	IndexList GetStaticStateIndex() const;
	IndexList GetStaticIngIndex() const;
	IndexList GetLocalStateIndex() const;
	IndexList GetTrendStateIndex() const;
	//��������б�
	vector<string> GetLocalDay()  const;
	vector<string> GetStaticIntegDay()  const;
	vector<string> GetStaticDay() const;
	vector<string> GetTrendDay() const;
	//ͨ��index��������״̬
	StatePoint CloselyStaticPoint(unsigned int index) const;
	StatePoint CloselyIntegralPoint(unsigned int index) const;
	StatePoint CloselyLocalPoint(unsigned int index) const;
	StatePoint CloselyTrendPoint(unsigned int index) const;
	//ͨ�����ڻ�������״̬
	StatePoint CloselyStaticPoint(CDate index)const;
	StatePoint CloselyIntegralPoint(CDate index)const;
	StatePoint CloselyLocalPoint(CDate index) const;
	StatePoint CloselyTrendPoint(CDate index) const;
	//
	StatePoint CloselyPoint(CDate index, const StatePointsList& PointList) const;
	StatePoint CloselyPoint(unsigned int index, const StatePointsList& PointList) const;
};



//////////////////////////////////////////////////////////////////////////


#endif
