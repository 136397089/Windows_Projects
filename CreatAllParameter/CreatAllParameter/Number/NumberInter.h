#pragma once
#ifndef _NUMBERALL_H
#define _NUMBERALL_H
#include <fstream>
#include <map>
#include "IndexNumber.h"
#include "../StockAccountNum.h"
#include "_DMA.h"
#include "Macd.h"
#include "trix.h"
#include "KDJ.h"
#include "ASI.h"
#include "../CDP.h"
#include "../DMI.h"
#include "StatusRecords.h"
#include "../log.h"
#include "../Ma.h"

typedef map<IndexType, AllStatus> IndexStatusMap;
typedef map<IndexType, AllStatus*> IndexStatusPointer;
typedef map<IndexType, const AllStatus*> IndexStatusConstPointer;
typedef map<IndexType, const VStockData*> StockDataPointer;

//单个股票有所有交易数据和指标
struct AllStockData
{
	void clear();
	//
	AllStockData NewDataByIndex(unsigned int beginindex,unsigned int endindex) const;
	//
	inline bool ChackDataSize();
	//
	StockDataPointer GetAllPointer() const;
	//
	StockDataPointer GetIndexPointer() const;
	//读取的股票成交信息
	VStockData _vOpenData;
	VStockData _vCloseData;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	VStockData _vPriChaRate;
	VStockData _vVolChaRate;
	VStockData _vMa1, _vMa2, _vMa3, _vMa4;
	vector<string> _vTimeDay;
	//指标结果保存
	VStockData _vMACDValue, _vMACDMa12, _vMACDMa26, _vDiff, _vDEA;
	VStockData _vDMAValue, _vAMAValue;
	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
	VStockData _vK, _vD, _vJ;
	VStockData _vAsi, _vAsit;
};





class StatusRecords;
class CNumberInterface //:protected CNumberManager
{
public:
	CNumberInterface();
	~CNumberInterface();
	//////////////////////////////////////////////////////////////////////////
	//分析目标文件中的数据
	//////////////////////////////////////////////////////////////////////////
	bool AnalyseTheFile(const string& filename ,const string& FilePath);


	const AllStockData& GetAllValue();

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
	//获得所有数据的指针
	//////////////////////////////////////////////////////////////////////////
// 	StockDataPointer GetDataPointers();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	inline void PushBackIndex(const SigDayTechIndex& AllIndex);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	//inline bool ChackDataSizeRaedFromFile();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void CheckAndPrintKDJMin(const string& filename);
	bool _IsInSale;
	int index;
	CLog resultFile;
	double startTime;
	AllStockData mAll;
};



#endif
