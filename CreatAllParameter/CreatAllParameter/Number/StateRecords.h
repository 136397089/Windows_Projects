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
//趋势判断过程中使用的指标个数
#define MaxSizeOfIndex 7
#define LowPriceIndex 0
#define HighPriceIndex 1
#define MACDIndex 2
#define TRIXIndex 3
#define ASIIndex 4
#define DMAIndex 5
#define KDJIndex 6

//指针群，最终状态进行判定时的入参
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
//所有的指标－－单日分析时的入参
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
//单个指标的趋势过程
enum QuotaTrendStateTpye
{
	_eTopRise_BottomRise,//出现上升底上升
	_eTopFall_BottomFall,//出现顶下降底下降
	_eRangeShrink,//范围收缩－－顶越来越低，底越来越高
	_eRangeEnlarge,//范围扩大－－顶越来越高，同时底越来越低
	_eTopRise,
	_eBottomRise,
	_eTopFall,
	_eBottomFall,
	_eNoRule,//无规律
	_eUndefindType_D//没有被定义
};
//静态特征点类型
enum SigQuotaState
{
	_eNegaTiveUpClose,//负值区域上穿
	_eNegaTiveDownClose,//负值区域下穿
	_ePositiveUpClose,//正值区域上穿
	_ePositiveDownClose,//正值区域下穿
	_eUndefindType_S//没有被定义
};
//局部特征点的类型
enum LocalFeaturePointsType
{
	_eLocalMin,
	_eLocalMax,
	_eUpCloseZero,
	_eDownCloseZero,
	_eUndefindType_L
};
//一个静态特征点
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
//一个局部特征点
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
//单参数静态状态的简单组合
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
//趋势判定过程中的所有指标状态的简单组合
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
//只能在本文件内使用的结构体
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
//构造不需要参数
//类功能，根据输入的参数指标，计算指标的趋势形态和静态形态
//有6个public函数
//
class StateRecords
{
public:
	StateRecords();
	~StateRecords();
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行记录
	//////////////////////////////////////////////////////////////////////////
	void DailyStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData);
	//////////////////////////////////////////////////////////////////////////
	//返回局部分析结果
	//////////////////////////////////////////////////////////////////////////
	inline bool GetLocalIndex(const IndexGenre& valueType , vector<FeatPoint_Local>& _vIndexRecord_Local);
	//////////////////////////////////////////////////////////////////////////
	//返回静态分析结果
	//////////////////////////////////////////////////////////////////////////
 	AllSigParaState_Static GetNowStaticState();
	//////////////////////////////////////////////////////////////////////////
	//返回趋势分析结果
	//////////////////////////////////////////////////////////////////////////
	TrendState GetNowTrendnamicState();
	//////////////////////////////////////////////////////////////////////////
	//对最终的数据进行趋势判断
	//////////////////////////////////////////////////////////////////////////
	bool GetFinalTrend(const StockDataPointer& pointers);
	//////////////////////////////////////////////////////////////////////////
	//初始化，清除所有内部趋势结果
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//////////////////////////////////////////////////////////////////////////
	//返回最后一次出错信息
	//////////////////////////////////////////////////////////////////////////
	string GetLastError();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void ResultPrint();
private:
	/*
	单日分析中使用到的函数
	*/
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行单日静态分析
	//////////////////////////////////////////////////////////////////////////
	void DailyStaticStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData);
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行单日局部分析
	//////////////////////////////////////////////////////////////////////////
	void DailyLocalStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData);
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行单日趋势分析
	//////////////////////////////////////////////////////////////////////////
	void DailyTrendStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData);
	//////////////////////////////////////////////////////////////////////////
	//进行单日单指标静态分析
	//////////////////////////////////////////////////////////////////////////
	void StaticStateRecordSigPoint(
		const IndexGenre& valueType,
		const tySData& FrontData,
		const tySData& BackData,
		const string& Day);
	//////////////////////////////////////////////////////////////////////////
	//进行单日单指标局部分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最低点分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep_Low(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最高点分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep_High(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);
	///////////////////////////////////////////////////////////////////////////
	//单日单指标趋势分析
	//////////////////////////////////////////////////////////////////////////
	void TrendStateRecordSingleStep(
		const IndexGenre& valueType,
		const tySData& dayValue,
		const string& day);

	/*
	最终分析的使用到的函数
	*/
	//////////////////////////////////////////////////////////////////////////
	//检查所有容器指针是否正常，
	//////////////////////////////////////////////////////////////////////////
	bool ChckeAllPointer(const StockDataPointer& pointers);
	//////////////////////////////////////////////////////////////////////////
	//最终分析中，查找局部最低点的index
	//////////////////////////////////////////////////////////////////////////
	bool GetDownIndex(
		const VStockData& pointers, 
		const unsigned int parameter, 
		vector<unsigned int>& indexVector,
		VStockData& LowValue);
	//////////////////////////////////////////////////////////////////////////
	//最终分析中，查找局部最高点的index
	//////////////////////////////////////////////////////////////////////////
	bool GetUpIndex(
		const VStockData& pointers, 
		const unsigned int parameter, 
		vector<unsigned int>& indexVector,
		VStockData& HighValue);
	//////////////////////////////////////////////////////////////////////////
	//将静态分析的结果输出到Log文件
	//////////////////////////////////////////////////////////////////////////
	void StaticResultPrint();
	//////////////////////////////////////////////////////////////////////////
	//将局部分析的结果输出到Log文件
	//////////////////////////////////////////////////////////////////////////
	void LocalResultPrint();
	//////////////////////////////////////////////////////////////////////////
	//将趋势分析的结果输出到Log文件
	//////////////////////////////////////////////////////////////////////////
	void TrendResultPrint();
private:
	/*
	静态分析中，静态特征点的历史记录
	*/
	//
	//unsigned int _FrontCloseMark_Static[MaxSizeOfIndex];
	//当前的静态特征
	FeatPoint_Static _NowState_Static[MaxSizeOfIndex];
	//一般情况下为false,如果当前的静态特征点已经被修改则设为true
	//如果静态特征点被修改过则会后面将当前的静态特征点加入到_vIndexRecord_State,再将其置为false
	bool _NowStateChange;
	//
	vector<FeatPoint_Static> _vIndexRecord_State;

	/*
	局部分析中，局部特征点的历史记录
	*/
	//
	unsigned int _Parameter_Local[MaxSizeOfIndex];
	//上次的Mark
	unsigned int _frontMark_Local[MaxSizeOfIndex];
	//上次的是不是最低点
	bool _forntMarkIsLow_Local[MaxSizeOfIndex];
	//上次Mark是日期
	string _frontMarkDay_Local[MaxSizeOfIndex];
	//上次Mark的值
	tySData _frontMarkValue_Local[MaxSizeOfIndex];
	//保存记录
	vector<FeatPoint_Local> _vIndexRecord_Local;

	/*
	趋势分析中使用的数据
	*/
	//
	TrendState _NowState_Trend;
	//TrendState _BottonState_Tr;
	unsigned int _TrAnaBeginSize;
	vector<QuotaTrendStateTpye> _vIndexRecord_Trend;
	//单日分析中，记录当前的Mark
	unsigned int _CurrentMark;
	CLog log;
	//最后一次出错信息
	string _LastError;
};


#endif