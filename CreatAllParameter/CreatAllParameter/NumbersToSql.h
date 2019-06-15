#pragma once

#ifndef NUMBERTOSQL_H
#define NUMBERTOSQL_H

#include <string>
#include "DateTool.h"
#include "Number/NumberBase.h"
#include "Number/Macd.h"
#include "Number/KDJ.h"
#include "Number/ASI.h"
#include "Number/commonfunction.h"
#include "MySQLInterFace.h"


struct DataTypeToSave
{
	string StockCode;//股票代码
	BasisCycleType cycle;//数据的周期
	CDate _time;//记录数据的时间点
	string IndicatorsName;//数据名
	void Inition(const string& StockName, BasisCycleType _cycle, const CDate& strtime, const string& Number);
	bool operator ==(const DataTypeToSave& d)const;
	bool operator <(const DataTypeToSave& d)const;
// 	bool operator <=(const DataTypeToSave& d)const;
};


struct RealDataIndex
{
	CDate _date;//记录数据的时间点
	BasisCycleType _cycletype;//数据的周期
	int _stockCode;//股票代码

	void SetStockCode(const string& codename);
	RealDataIndex& operator=(const RealDataIndex& d);
	bool operator ==(const RealDataIndex& d)const; 
	bool operator <(const RealDataIndex& d)const;
	bool operator <=(const RealDataIndex& d)const;
};

using namespace std;

class CNumbersToSql
{
public:
	CNumbersToSql();
	~CNumbersToSql();
	//初始化，清空缓存数据
	bool IniMysqlTool();
	//
	bool RecoveryPriceData(const string& stockCode,const string& typeofdata,vector<SinCyclePriceData>& FrontPrice);
	//检查基金是否持有的股票，返回持有该股票的基金数
	int CheckStockFundCount(const string& stockcode);
	//
	float ReadPEFromSQL(const string& stockcode);
	//保存分组之后的收益率
	void SaveProGroupRate(const string& codeName, const vector<StockDataType>& returnRate,const vector<string>& GroupType) const;
	//
	bool SaveTemporaryIndicatorsData(const DataTypeToSave& numbertype, const vector<StockDataType>& _DataToSave);
	//
	bool GetTemporaryNumberData(const DataTypeToSave& numbertype, vector<StockDataType>& _Data);
	//
	bool RefreshTemporaryNumberData(const CDate& _date);
	//
	bool GetCurrentData(map<RealDataIndex, SinCyclePriceData>& CurrentData, vector<RealDataIndex>& NoUpdatesTableType);
private:
	//
	void GetNumberColumnsType(vector<Column>& vdataType);
	//
	bool GetNumberValueString(string& valueString, const vector<StockDataType>& _vNumberDataToSave, const DataTypeToSave& numbertype);
	//
	void SetProGroupColumnsType(vector<Column>& vdataType, unsigned int typeSize) const ;
	//
	void SetFreqGroupingColumnsType(int beginGroupIndex, int endGroupIndex);
	//
	bool GetReturnRateValueString(string& valueString, const vector<StockDataType>& returnRate) const ;
	//
	unsigned int GetBeginIndexToSave(vector<CDate>& _vDate,const string& tebleName);
	//
	bool GetDuplicataUpdateValue(vector<Column>& vdataType, unsigned int beginIndex,unsigned int count, string& result);
	//
	bool CheckRealtimeTableName(const string& tableName, CDate& tebleDate, BasisCycleType& tableCycleType);
	//
	MySQLInterFace *psqlTool;
	//
};




#endif
