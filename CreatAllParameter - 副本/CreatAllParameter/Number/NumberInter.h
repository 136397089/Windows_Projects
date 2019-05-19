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




class StateRecords;
class CNumberInterface //:protected CNumberManager
{
public:
	CNumberInterface();
	~CNumberInterface();
	//////////////////////////////////////////////////////////////////////////
	//分析目标文件中的数据
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndNumber(const string& filename ,const string& FilePath);

	//
	const StockDataTable& GetDayValue() const;
	//
	const StockDataTable& GetWeekValue() const;
	//
	const StockDataTable& GetMonthValue() const;
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
	//将计算完的数据保存到文件当中
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToFile(const string& strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void CheckAndPrintKDJMin(const string& filename);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool ResizeData(CDate beginData);
	bool _IsInSale;
	int index;
	CLog resultFile;
	double startTime;
	StockDataTable mDayValue;
	StockDataTable mWeekValue;
	StockDataTable mMonthValue;
};



#endif
