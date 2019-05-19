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


using namespace std;

class CNumbersToSql
{
public:
	CNumbersToSql();
	~CNumbersToSql();
	//��ʼ������ջ�������
	bool IniMysqlTool();
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
	bool InsertData(
		const string& stockcode,
		const vector<unsigned int>& TypeAndFreqData,
		const vector<float>& meanVarData,
		int beginIndex,
		int endIndex);

	//����������ݱ��浽���ݿ�
	bool SaveMACDCacheData();
	bool SaveKDJCacheData();
	bool SaveDayCacheData();
	bool SaveFreqCacheData();

	bool RecoveryMACDData(
		const string& stockCode,
		const string& typeofdata,
		MACDCombin& ReturnMacd
		);
	bool RecoveryKDJData(
		const string& stockCode,
		const string& typeofdata,
		KDJCombin& KDJReturn);
	bool RecoveryPriceData(
		const string& stockCode,
		const string& typeofdata,
		vector<SinDayPriceData>& FrontPrice);
	bool GetMacd(
		const string& stockCode,
		const string& typeofdata,
		MACDCombin& _macd);
	int CheckStockFundCount(const string& stockcode);

	float ReadPEFromSQL(const string& stockcode);
private:
	//���MACD�ı�ͷ
	void GetMacdColumnsType(vector<Column>& vdataType);
	//��������ݵı�ͷ
	void GetDayColumnsType(vector<Column>& vdataType);
	//���KDJ�ı�ͷ
	void GetKDJColumnsType(vector<Column>& vdataType);
	//���KDJ�ı�ͷ
	void SetFreqGroupingColumnsType(int beginGroupIndex, int endGroupIndex);
	//��KDJ������ת��Ϊ�ַ������������ɱ���ָ��
	bool KDJDataConversion(
		const vector<Column>& saveDataType,
		const KDJ& KDJdataToSave,
		const CKDJCal& calData,
		vector<string>& vdataList);
	//
	bool GetMACDValueString(string& valueString,unsigned int beginInedx);
	//
	bool GetDayValueString(string& valueString, unsigned int beginInedx);
	//
	bool GetKDJValueString(string& valueString, unsigned int beginInedx);
	//
	bool GetFreqValueString(string& valueString, unsigned int beginInedx);
	//
	unsigned int GetBeginIndexToSave(vector<CDate>& _vDate,const string& tebleName);
	//
	bool GetDuplicataUpdateValue(vector<Column>& vdataType, unsigned int beginIndex, string& result);
	//
	bool StringChangelateToMACD(const vector<string>& macdstrData, CDate& _date, Macd& MacdData);
	//
	bool StringChangelateToKDJ(const vector<string>& kdjstrData, CDate& _date, KDJ& KDJData);

	vector<Macd> _vMacdDataToSave;
	vector<CDate> _vMacdDate;
	string _MacdTablename;

	vector<KDJ> _vKDJDataToSave;
	vector<CDate> _vKDJDate;
	string _KDJTablename;

	vector<CDate> _DayDataDates;
	vector<DayHLCOV> _vDayDataToSave;
	string _dataTebleName;

	vector<string> _vFreqDataToSave;
	string _freqTebleName;
	int _beginIndex;
	int _endindex;

	MySQLInterFace *psqlTool;
};




#endif
