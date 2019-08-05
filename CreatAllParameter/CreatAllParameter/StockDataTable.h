
#ifndef ALLSTOCKDATA_H
#define ALLSTOCKDATA_H

#include <map>
#include "number/NumberBase.h"
// #include "number/StateRecords.h"
// #include "StockAccountNum.h"

typedef map<StockNumberType,  VStockData const *> StockDataPointer;

//单个股票有所有交易数据和指标
struct StockDataTable
{
	//
	void clear();
	//
	StockDataTable NewDataByIndex(unsigned int beginindex, unsigned int endindex) const;
	//
	void GetIndexMap(const vector<CDate>& InputDate,map<unsigned int,unsigned int>& indexMap);
	//
	bool ChackDataSize() const;
	//返回所有数据的指针，用于保存数据到文件夹中，
	StockDataPointer const GetAllPointerToSave();
	//返回所有指标的指针
// 	StockDataPointer GetNumberPointer() const;
	//字符串形式日期转为CDate形式的日期
	void SetDate();
	//通过日期查找前一时刻的Index
	unsigned int GetCloselyFrontTimeIndexByDate(string strDate) const;
	//通过日期查找前一时刻的Index
	unsigned int GetCloselyFrontTimeIndexByDate(CDate date)const;
	//读取的股票成交信息
	CycleType DataCycle;
	string _strStockCode;
	vector<string> _vTimeDay;
	vector<CDate> _vDate;
	vector<bool> DataEnable;
	VStockData _vOpen;
	VStockData _vClose;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	VStockData _vCumulativeChangerate;
	VStockData _vMapToMarket;
	//
	map<StockNumberType, VStockData> _vTableAllIndex;
	//指标结果保存
};

#endif
