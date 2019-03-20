
#ifndef ALLSTOCKDATA_H
#define ALLSTOCKDATA_H

#include <map>
#include "number/NumberBase.h"
// #include "number/StateRecords.h"
// #include "StockAccountNum.h"

typedef map<IndexType, const VStockData*> StockDataPointer;

//单个股票有所有交易数据和指标
struct StockDataTable
{
	void clear();
	//
	StockDataTable NewDataByIndex(unsigned int beginindex, unsigned int endindex) const;
	//
	bool ChackDataSize() const;
	//
	StockDataPointer GetAllPointer() const;
	//
	StockDataPointer GetIndexPointer() const;
	//读取的股票成交信息
	vector<string> _vTimeDay;
	VStockData _vOpen;
	VStockData _vClose;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	//
	VStockData _vPriChaRate;
	VStockData _vVolChaRate;
	VStockData _vMa1, _vMa2, _vMa3, _vMa4;
	//指标结果保存
	VStockData _vMACDValue, _vMACDMa12, _vMACDMa26, _vDiff, _vDEA;
	VStockData _vDMAValue, _vAMAValue;
	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
	VStockData _vK, _vD, _vJ;
	VStockData _vAsi_i, _vAsit;
	VStockData _vCDP, _vAH_High, _vNH_NormalHigh, _vAL_Low, _vNL_NormalLow;
};

#endif
