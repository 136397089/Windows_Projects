#pragma once
#include <string>
#include "DateTool.h"
#include "Number/NumberBase.h"
#include "Number/Macd.h"
#include "Number/KDJ.h"
#include "Number/ASI.h"
#include "Number/commonfunction.h"
#include "MySQLInterFace.h"


using namespace std;


struct DayHLCOV
{
	DayHLCOV(
		StockDataType _hlcopen,
		StockDataType _hlchigh,
		StockDataType  _hlclow,
		StockDataType _hlcclose,
		StockDataType  _hlcvol)
		:_HLCOpen(_hlcopen),
		_HLCHigh(_hlchigh),
		_HLCLow(_hlclow),
		_HLCClose(_hlcclose),
		_HLCVol(_hlcvol){}
	StockDataType _HLCOpen;
	StockDataType _HLCHigh;
	StockDataType _HLCLow;
	StockDataType _HLCClose;
	StockDataType _HLCVol;
};
class CNumbersToSql
{
public:
	CNumbersToSql();
	~CNumbersToSql();
	//保存单个数据到数据库－MACD
	bool SaveSingleData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const Macd& dataToSave);
	//保存单个数据到数据库-KDJ
	bool SaveSingleData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const KDJ& dataToSave,
		const CKDJCal& calData);
	//初始化，清空缓存数据
	bool IniMysqlTool();
	//将缓存数据保存到数据库
	void InsertData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const Macd& dataToSave);
	//将缓存数据保存到数据库
	bool InsertData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const KDJ& dataToSave,
		const CKDJCal& calData);
	bool InsertData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const DayHLCOV& _daydata);

	//将缓存的数据保存到数据库
	bool SaveMACDCacheData();
	bool SaveKDJCacheData();
	bool SaveDayCacheData();

	bool RecoveryData(const CDate& _date, Macd& _macd);
	bool RecoveryData(const CDate& _date, KDJ& _kdj,CKDJCal& calData);

private:
	//获得MACD的表头
	void GetMacdColumnsType(vector<Column>& vdataType);
	//获得日数据的表头
	void GetDayColumnsType(vector<Column>& vdataType);
	//获得KDJ的表头
	void GetKDJColumnsType(vector<Column>& vdataType);
	//将KDJ的数据转换为字符串，用于生成保存指令
	bool KDJDataConversion(
		const vector<Column>& saveDataType,
		const KDJ& KDJdataToSave,
		const CKDJCal& calData,
		vector<string>& vdataList);
	//
	bool GetMACDValueString(string& valueString);
	//
	bool GetDayValueString(string& valueString);

	bool GetKDJValueString(string& valueString);
	vector<Macd> _vMacdDataToSave;
	vector<CDate> _vMacdDate;
	string _MacdTablename;

	vector<KDJ> _vKDJDataToSave;
	vector<CDate> _vKDJDate;
	string _KDJTablename;

	vector<CDate> _Dates;
	vector<DayHLCOV> _vHLCToSave;
	string _dataTebleName;

	MySQLInterFace *psqlTool;
	HANDLE _MYSQLMutex;
};

