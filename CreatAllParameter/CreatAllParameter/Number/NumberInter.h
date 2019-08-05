#pragma once
#ifndef _NUMBERALL_H
#define _NUMBERALL_H
#include <fstream>
#include <map>
#include "NumberBase.h"
#include "../StockAccountNum.h"
#include "../log.h"
#include "../StockDataTable.h"
#include "StateRecords.h"
#include "../NumbersToSql.h"



typedef map<StockNumberType, StateTable> IndexStateMap;
typedef map<StockNumberType, StateTable*> IndexStatePointer;
typedef map<StockNumberType, const StateTable*> IndexStateConstPointer;


//定义参数
#define MACD_INDEX1 36.0f
#define MACD_INDEX2 78.0f
#define KDJ_INDEX1 9.0f
#define KDJ_INDEX2 3.0f
#define KDJ_INDEX3 3.0f

class SinDayData: public SinCyclePriceData
{
public:
	StockDataType changepercent;//涨跌幅
	StockDataType turnoverratio;//换手率
	StockDataType per; //市盈率
	StockDataType pb;//市净率
	StockDataType mktcap;//总市值
	StockDataType nmc;//流通市值
// 	StockDataType _Open;
// 	StockDataType _Close;
// 	StockDataType _High;
// 	StockDataType _Low;
// 	StockDataType _Volume;
	SinDayData()
		:changepercent(0), turnoverratio(0), per(0), pb(0), mktcap(0), nmc(0)
		/*,_Open(0), _Close(0), _High(0), _Low(0), _Volume(0)*/{}
};


class StateRecords;
class CIndicatorsInterface //:protected CNumberManager
{
public:
	CIndicatorsInterface();
	~CIndicatorsInterface();
	//////////////////////////////////////////////////////////////////////////
	//从文件中读数据，并计算指标
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndIndicators_History(const string& filename, const string& FilePath);

	//
	bool MappingToSH(const StockDataTable& shdata);

	bool GetDataAndIndicators_SH(const string& filename, const string& FilePath);

	//////////////////////////////////////////////////////////////////////////
	//从文件中读数据，并计算指标
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndIndicatorMintue_History(const string& filename, const string& FilePath);
	//
	StockDataTable& GetResourceValue();
	//
	StockDataTable& GetWeekValue();
	//
	StockDataTable& GetMonthValue();
	//
	StockDataTable& GetRealtimeValue();
	//从MYSQL中读出所有股票的实时数据
	void RefreshAllStockDate_RealTime();
	//
	bool GetIndicators_Realtime(const string& _stockName, CycleType dataCycle);
	//
	CDate GetLastDate();
private:
	//返回实时的价格数据中目前股票的数据
	bool GetStockPriceData_RealTime(const string& stockData, CycleType dataCycle,map<RealDataIndex, SinCyclePriceData>& returnData);
	//计算指标
	bool Cal30MinuteIndicators();
	//计算指标
	bool CalResourceIndicators();
	//计算指标
	bool CalWeekIndicators();
	//计算指标
	bool CalMonthIndicators();
	//////////////////////////////////////////////////////////////////////////
	//从文件中读出所有的数据，全部保存为字符串在 AllString中
	//////////////////////////////////////////////////////////////////////////
	void RaedDateFromFile(const string& strFilePath, StringBlock& AllString);
	//////////////////////////////////////////////////////////////////////////
	//将字符串数据翻译成可用于计算的数据
	//////////////////////////////////////////////////////////////////////////
	void ProcessingTransverseData(const StringBlock& AllString, StockDataTable& mTargetValue);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool ResizeData(CDate beginData);
	//获得周期对应的时间
	CDate GetCycleTime(CycleType dataCycle);
	StockDataTable mResourceValue;
	StockDataTable mWeekValue;
	StockDataTable mMonthValue;
	bool IsLongitudinal;//标志数据是否横向排列
	//用于存放实时数据
	StockDataTable mRealTimeValue;
	map<RealDataIndex, SinCyclePriceData> CurrentData;
	vector<RealDataIndex> NoUpdatesTableType;
	map<unsigned int, unsigned int> IndexMap;
};



#endif
