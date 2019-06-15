
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
	bool ChackDataSize() const;
	//返回所有数据的指针，用于保存数据到文件夹中，
	StockDataPointer const GetAllPointerToSave();
	//返回所有指标的指针
	StockDataPointer GetNumberPointer() const;
	//字符串形式日期转为CDate形式的日期
	void SetDate();
	//通过日期查找前一时刻的Index
	unsigned int GetLastTimeIndexByDate(string strDate) const;
	//通过日期查找前一时刻的Index
	unsigned int GetLastTimeIndexByDate(CDate date)const;
	//读取的股票成交信息
	string _strStockCode;
	vector<string> _vTimeDay;
	vector<CDate> _vDate;
	VStockData _vOpen;
	VStockData _vClose;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	//
	map<StockNumberType, VStockData> _vTableAllIndex;
// 	VStockData _vPriChaRate;
// 	VStockData _vVolChaRate;
// 	VStockData _vMa1, _vMa2, _vMa3, _vMa4,_vMaUpRate;
	//指标结果保存
// 	VStockData _vMACDValue, _vMACDMa12, _vMACDMa26, _vDiff, _vDEA;
// 	VStockData _vDMAValue, _vAMAValue;
// 	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
// 	VStockData _vK, _vD, _vJ;
// 	VStockData _vAsi_i, _vAsit;
// 	VStockData _vCDP, _vAH_High, _vNH_NormalHigh, _vAL_Low, _vNL_NormalLow;
// 	VStockData _vAR, _vBR, _vVR, _vPSY, _vCR, _vCRMA1, _vCRMA2, _vCRMA3, _vCRMA4;
// 	VStockData _vEMV,_vEMVMA;
// 	VStockData _vBOOLMid, _vBOOLUp, _vBOOLDown,_vBOOLSTD;
// 	VStockData _vDIP, _vDIN, _vADX, _vADXR;
// 	VStockData _vRSI1, _vRSI2, _vRSI3;
};

#endif
