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
	//���浥�����ݵ����ݿ⣭MACD
	bool SaveSingleData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const Macd& dataToSave);
	//���浥�����ݵ����ݿ�-KDJ
	bool SaveSingleData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const KDJ& dataToSave,
		const CKDJCal& calData);
	//��ʼ������ջ�������
	bool IniMysqlTool();
	//���������ݱ��浽���ݿ�
	void InsertData(
		const string& stockcode,
		const string& typeofdata,
		const CDate& date,
		const Macd& dataToSave);
	//���������ݱ��浽���ݿ�
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

	//����������ݱ��浽���ݿ�
	bool SaveMACDCacheData();
	bool SaveKDJCacheData();
	bool SaveDayCacheData();

	bool RecoveryData(const CDate& _date, Macd& _macd);
	bool RecoveryData(const CDate& _date, KDJ& _kdj,CKDJCal& calData);

private:
	//���MACD�ı�ͷ
	void GetMacdColumnsType(vector<Column>& vdataType);
	//��������ݵı�ͷ
	void GetDayColumnsType(vector<Column>& vdataType);
	//���KDJ�ı�ͷ
	void GetKDJColumnsType(vector<Column>& vdataType);
	//��KDJ������ת��Ϊ�ַ������������ɱ���ָ��
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

