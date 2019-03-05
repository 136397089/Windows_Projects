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
//ָ��Ⱥ������״̬�����ж�ʱ�����
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


//����������
enum CharaPointType
{
	_eStaticNegaTiveUpClose,//��ֵ�����ϴ�
	_eStaticNegaTiveDownClose,//��ֵ�����´�
	_eStaticPositiveUpClose,//��ֵ�����ϴ�
	_eStaticPositiveDownClose,//��ֵ�����´�
	//
	_eLocalLow,//
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

//����ָ�����е�����λ��
class AllStatus
{
public:
	
	StatusPointsList _staticstatus;//��̬����
	StatusPointsList _staticIng;//��̬����
	StatusPointsList _localstatus;//�ֲ�����
	StatusPointsList _trendstatus;//��������

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


//�����ж������е�����ָ��״̬�ļ����
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

