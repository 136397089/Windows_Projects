#ifndef _STATERECOEDS_H
#define  _STATERECOEDS_H
#include <assert.h>  
#include "IndexNumber.h"
#include "_DMA.h"
#include "Macd.h"
#include "trix.h"
#include "../StockAccountNum.h"
#include "KDJ.h"
#include "ASI.h"
#include "../CDP.h"
#include "../DMI.h"
#include "../log.h"
//�����жϹ�����ʹ�õ�ָ�����
#define MaxSizeOfIndex 7
#define LowPriceIndex 0
#define HighPriceIndex 1
#define MACDIndex 2
#define TRIXIndex 3
#define ASIIndex 4
#define DMAIndex 5
#define KDJIndex 6

//ָ��Ⱥ������״̬�����ж�ʱ�����
struct StockDataPointer
{
	const vector<string>* _pDay;
	const VStockData* _pOpenData;
	const VStockData* _pCloseData;
	const VStockData* _pHigh;
	const VStockData* _pLow;
	const VStockData* _pMACDValue, *_pMa12, *_pMa26, *_pDiff, *_pDEA;
	const VStockData* _pDMAValue, *_pAMAValue;
	const VStockData* _pTRIX, *_pTRMA;
	const VStockData* _pK, *_pD, *_pJ;
	const VStockData* _pAsi, *_pAsit;
};
//���е�ָ�꣭�����շ���ʱ�����
struct SigDayTechIndex
{
	DMA_DATA _DMAData;
	Macd _MacdData;
	TRIX _TrixData;
	KDJ _Kdj;
	ASI _Asi;
	CDP _Cdp;
	Dmi _Dmi;
};



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
//����ָ������ƹ���
enum QuotaTrendStateTpye
{
	_eTopRise_BottomRise,//��������������
	_eTopFall_BottomFall,//���ֶ��½����½�
	_eRangeShrink,//��Χ����������Խ��Խ�ͣ���Խ��Խ��
	_eRangeEnlarge,//��Χ���󣭣���Խ��Խ�ߣ�ͬʱ��Խ��Խ��
	_eTopRise,
	_eBottomRise,
	_eTopFall,
	_eBottomFall,
	_eNoRule,//�޹���
	_eUndefindType_D//û�б�����
};
//��̬����������
enum SigQuotaState
{
	_eNegaTiveUpClose,//��ֵ�����ϴ�
	_eNegaTiveDownClose,//��ֵ�����´�
	_ePositiveUpClose,//��ֵ�����ϴ�
	_ePositiveDownClose,//��ֵ�����´�
	_eUndefindType_S//û�б�����
};
//�ֲ������������
enum LocalFeaturePointsType
{
	_eLocalMin,
	_eLocalMax,
	_eUpCloseZero,
	_eDownCloseZero,
	_eUndefindType_L
};
//һ����̬������
struct FeatPoint_Static
{
	unsigned int _TimeIndex;
	string _Day;
	tySData _Value;
	SigQuotaState _StaticFeat;
	IndexGenre _IndexGenre;
	FeatPoint_Static()
	{
		_TimeIndex = 0;
		_Value = 0;
		_StaticFeat = _eUndefindType_S;
		_Day = "";
		_IndexGenre = _eUnDefine;
	}
};
//һ���ֲ�������
struct FeatPoint_Local
{
	unsigned int _TimeIndex;
	string _Day;
	tySData _Value;
	IndexGenre _IndexGenre;
	LocalFeaturePointsType _IndexSig;
	FeatPoint_Local()
	{
		_TimeIndex = 0;
		_Day = "";
		_Value = 0;
		_IndexGenre = _eUnDefine;
		_IndexSig = _eUndefindType_L;
	}
};
//��������̬״̬�ļ����
struct AllSigParaState_Static
{
	//FeatPoint_Static _HighType;
	FeatPoint_Static _MACDType;
	FeatPoint_Static _DMAType;
	FeatPoint_Static _TRIXType;
	FeatPoint_Static _KDJType;
	FeatPoint_Static _ASIType;
	AllSigParaState_Static(){
		//_HighType._IndexGenre = _eHighPrice;
		_MACDType._IndexGenre = _eMACD;
		_DMAType._IndexGenre = _eDMA;
		_TRIXType._IndexGenre = _eTRIX;
		_KDJType._IndexGenre = _eKDJ;
		_ASIType._IndexGenre = _eASI;
	}
};
//�����ж������е�����ָ��״̬�ļ����
struct TrendState
{
	QuotaTrendStateTpye _AllType[MaxSizeOfIndex];
// 	QuotaTrendStateTpye _LowType;
// 	QuotaTrendStateTpye _HighType;
// 	QuotaTrendStateTpye _DMAType;
// 	QuotaTrendStateTpye _MacdType;
// 	QuotaTrendStateTpye _TrixType;
// 	QuotaTrendStateTpye _AsiType;
	TrendState(){
		for (unsigned int i = 0; i < MaxSizeOfIndex; ++i)
			_AllType[i] = _eUndefindType_D;
	}
	QuotaTrendStateTpye& operator [](unsigned int index)
	{
		if (index < MaxSizeOfIndex)
			return _AllType[index];
		else
			return _AllType[MaxSizeOfIndex - 1];
	}
	QuotaTrendStateTpye operator [](unsigned int index)const
	{
		if (index < MaxSizeOfIndex)
			return _AllType[index];
		else
			return _AllType[MaxSizeOfIndex - 1];
	}
};
//////////////////////////////////////////////////////////////////////////
//ֻ���ڱ��ļ���ʹ�õĽṹ��
namespace
{
	struct BoolNumberUp
	{
		bool isTodayMACDUp;
		bool isTodayDMAUp;
		bool isTodayTRIXUp;
		int isTodayKDJUp;
		BoolNumberUp()
		{
			isTodayMACDUp = false;
			isTodayDMAUp = false;
			isTodayTRIXUp = false;
			isTodayKDJUp = false;
		}
	};
}
//
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


//
//���첻��Ҫ����
//�๦�ܣ���������Ĳ���ָ�꣬����ָ���������̬�;�̬��̬
//��6��public����
//
class StateRecords
{
public:
	StateRecords();
	~StateRecords();
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����м�¼
	//////////////////////////////////////////////////////////////////////////
	void DailyStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData);
	//////////////////////////////////////////////////////////////////////////
	//���ؾֲ��������
	//////////////////////////////////////////////////////////////////////////
	inline bool GetLocalIndex(const IndexGenre& valueType , vector<FeatPoint_Local>& _vIndexRecord_Local);
	//////////////////////////////////////////////////////////////////////////
	//���ؾ�̬�������
	//////////////////////////////////////////////////////////////////////////
 	AllSigParaState_Static GetNowStaticState();
	//////////////////////////////////////////////////////////////////////////
	//�������Ʒ������
	//////////////////////////////////////////////////////////////////////////
	TrendState GetNowTrendnamicState();
	//////////////////////////////////////////////////////////////////////////
	//�����յ����ݽ��������ж�
	//////////////////////////////////////////////////////////////////////////
	bool GetFinalTrend(const StockDataPointer& pointers);
	//////////////////////////////////////////////////////////////////////////
	//��ʼ������������ڲ����ƽ��
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//////////////////////////////////////////////////////////////////////////
	//�������һ�γ�����Ϣ
	//////////////////////////////////////////////////////////////////////////
	string GetLastError();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void ResultPrint();
private:
	/*
	���շ�����ʹ�õ��ĺ���
	*/
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����е��վ�̬����
	//////////////////////////////////////////////////////////////////////////
	void DailyStaticStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData);
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����е��վֲ�����
	//////////////////////////////////////////////////////////////////////////
	void DailyLocalStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData);
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����е������Ʒ���
	//////////////////////////////////////////////////////////////////////////
	void DailyTrendStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData);
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ�꾲̬����
	//////////////////////////////////////////////////////////////////////////
	void StaticStateRecordSigPoint(
		const IndexGenre& valueType,
		const tySData& FrontData,
		const tySData& BackData,
		const string& Day);
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ��ֲ�����
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���͵����
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep_Low(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���ߵ����
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep_High(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	///////////////////////////////////////////////////////////////////////////
	//���յ�ָ�����Ʒ���
	//////////////////////////////////////////////////////////////////////////
	void TrendStateRecordSingleStep(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);

	/*
	���շ�����ʹ�õ��ĺ���
	*/
	//////////////////////////////////////////////////////////////////////////
	//�����������ָ���Ƿ�������
	//////////////////////////////////////////////////////////////////////////
	bool ChckeAllPointer(const StockDataPointer& pointers);
	//////////////////////////////////////////////////////////////////////////
	//���շ����У����Ҿֲ���͵��index
	//////////////////////////////////////////////////////////////////////////
	bool GetDownIndex(
		const VStockData& pointers, 
		const unsigned int parameter, 
		vector<unsigned int>& indexVector,
		VStockData& LowValue);
	//////////////////////////////////////////////////////////////////////////
	//���շ����У����Ҿֲ���ߵ��index
	//////////////////////////////////////////////////////////////////////////
	bool GetUpIndex(
		const VStockData& pointers, 
		const unsigned int parameter, 
		vector<unsigned int>& indexVector,
		VStockData& HighValue);
	//////////////////////////////////////////////////////////////////////////
	//����̬�����Ľ�������Log�ļ�
	//////////////////////////////////////////////////////////////////////////
	void StaticResultPrint();
	//////////////////////////////////////////////////////////////////////////
	//���ֲ������Ľ�������Log�ļ�
	//////////////////////////////////////////////////////////////////////////
	void LocalResultPrint();
	//////////////////////////////////////////////////////////////////////////
	//�����Ʒ����Ľ�������Log�ļ�
	//////////////////////////////////////////////////////////////////////////
	void TrendResultPrint();
private:
	/*
	��̬�����У���̬���������ʷ��¼
	*/
	//
	//unsigned int _FrontCloseMark_Static[MaxSizeOfIndex];
	//��ǰ�ľ�̬����
	FeatPoint_Static _NowState_Static[MaxSizeOfIndex];
	//һ�������Ϊfalse,�����ǰ�ľ�̬�������Ѿ����޸�����Ϊtrue
	//�����̬�����㱻�޸Ĺ������潫��ǰ�ľ�̬��������뵽_vIndexRecord_State,�ٽ�����Ϊfalse
	bool _NowStateChange;
	//
	vector<FeatPoint_Static> _vIndexRecord_State;

	/*
	�ֲ������У��ֲ����������ʷ��¼
	*/
	//
	unsigned int _Parameter_Local[MaxSizeOfIndex];
	//�ϴε�Mark
	unsigned int _frontMark_Local[MaxSizeOfIndex];
	//�ϴε��ǲ�����͵�
	bool _forntMarkIsLow_Local[MaxSizeOfIndex];
	//�ϴ�Mark������
	string _frontMarkDay_Local[MaxSizeOfIndex];
	//�ϴ�Mark��ֵ
	tySData _frontMarkValue_Local[MaxSizeOfIndex];
	//�����¼
	vector<FeatPoint_Local> _vIndexRecord_Local;

	/*
	���Ʒ�����ʹ�õ�����
	*/
	//
	TrendState _NowState_Trend;
	//TrendState _BottonState_Tr;
	unsigned int _TrAnaBeginSize;
	vector<QuotaTrendStateTpye> _vIndexRecord_Trend;
	//���շ����У���¼��ǰ��Mark
	unsigned int _CurrentMark;
	CLog log;
	//���һ�γ�����Ϣ
	string _LastError;
};


#endif