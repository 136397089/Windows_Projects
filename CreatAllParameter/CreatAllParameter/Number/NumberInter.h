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




typedef map<IndexType, StateTable> IndexStateMap;
typedef map<IndexType, StateTable*> IndexStatePointer;
typedef map<IndexType, const StateTable*> IndexStateConstPointer;


//定义参数
#define MACD_INDEX1 36.0f
#define MACD_INDEX2 78.0f
#define KDJ_INDEX1 9.0f
#define KDJ_INDEX2 3.0f
#define KDJ_INDEX3 3.0f

class SinDayData: public SinDayPriceData
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
class CNumberInterface //:protected CNumberManager
{
public:
	CNumberInterface();
	~CNumberInterface();
	//////////////////////////////////////////////////////////////////////////
	//从文件中读数据，并计算指标
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndNumbers(const string& filename ,const string& FilePath);

	//
	const StockDataTable& GetDayValue() const;
	//
	const StockDataTable& GetWeekValue() const;
	//
	const StockDataTable& GetMonthValue() const;
	//从文件夹中读出当日的数据
	bool GetTodayData(const string& fileFullPath, map<string, SinDayData>& TodayData);
private:
	//////////////////////////////////////////////////////////////////////////
	//从文件中读出所有的数据
	//////////////////////////////////////////////////////////////////////////
	void RaedDateFromFile(const string& strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//读横向排列的数据
	//////////////////////////////////////////////////////////////////////////
	void ProcessingTransverseData(const StringBlock& AllString);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool ResizeData(CDate beginData);
	bool _IsInSale;
	int index;
	//CLog resultFile;
	double startTime;
	StockDataTable mDayValue;
	StockDataTable mWeekValue;
	StockDataTable mMonthValue;
};



#endif
