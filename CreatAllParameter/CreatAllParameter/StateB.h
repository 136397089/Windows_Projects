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




//���е�ָ�꣭�����շ���ʱ�����
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
	//ͨ�õ�����㺯��
	StatePoint VersatileCloselyPoint(CDate index, const StatePointsList& PointList) const;
	StatePoint VersatileCloselyPoint(unsigned int index, const StatePointsList& PointList) const;
};



//////////////////////////////////////////////////////////////////////////


#endif

