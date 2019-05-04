#include "stdafx.h"
#include "NumbersToSql.h"
#include <vector>
#include <string>
#include "DateTool.h"
#include "Locker.h"
#include "glog/logging.h"
using namespace std;

static const string DataBaseName = "StockData";
static MySQLInterFace* sql_index = NULL;
static vector<Column> MACDDataHeader;
static vector<Column> KDJdataHeader;
static vector<Column> DaydataHeader;
static CRITICAL_SECTION _NUMTOMYSQLCS;

CNumbersToSql::CNumbersToSql()
{
// 	_MYSQLMutex = CreateMutex(nullptr, true, "SqlMutex");
//	CLocker sqlSaveLock(_MYSQLMutex, 10000);

	if (NULL == sql_index)
	{
		//初始化数据库
		InitializeCriticalSection(&_NUMTOMYSQLCS);
		EnterCriticalSection(&_NUMTOMYSQLCS);
		sql_index = new MySQLInterFace;
		psqlTool = sql_index;
		psqlTool->Inition();
		psqlTool->UseDatabases(DataBaseName);
		LeaveCriticalSection(&_NUMTOMYSQLCS);
	}
	else
	{
		psqlTool = sql_index;
	}
}


CNumbersToSql::~CNumbersToSql()
{
// 	CloseHandle(_MYSQLMutex);
// 	DeleteCriticalSection(&MYSQLCS);
}

bool CNumbersToSql::SaveSingleData(
	const string& stockcode,
	const string& typeofdata,
	const CDate& dateIndex,
	const Macd& dataToSave)
{
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//生成表名
	string dataTableName = "";
	dataTableName = stockcode +"_" + typeofdata + "MACD";
	//获得MACD的表头
	if (MACDDataHeader.empty())
		GetMacdColumnsType(MACDDataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(dataTableName))
		psqlTool->CreatTable(dataTableName, MACDDataHeader);
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(dataTableName, MACDDataHeader))
		psqlTool->AddColumnToTable(dataTableName, MACDDataHeader);

	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++MACDDataHeader.begin()), MACDDataHeader.end());
	vector<string> vdataList;
	psqlTool->DataConversion(saveDataType[0], dateIndex, vdataList);
	psqlTool->DataConversion(saveDataType[1], dataToSave.m12, vdataList);
	psqlTool->DataConversion(saveDataType[2], dataToSave.m26, vdataList);
	psqlTool->DataConversion(saveDataType[3], dataToSave.dea, vdataList);
	psqlTool->DataConversion(saveDataType[4], dataToSave.diff,vdataList);
	psqlTool->DataConversion(saveDataType[5], dataToSave.bar, vdataList);

	WhereCommand whereCom;
	vector<WhereCommand> commandList;
	commandList.push_back(whereCom.IniCommand(saveDataType[0].Dataname, _eIs, vdataList[0], _eAnd));
	
	if (psqlTool->CheckDataExists(dataTableName, commandList))//判断数据是不是已经存在，存在的话就将原数据更新
	{
		//psqlTool->UpDateData(dataTableName, saveDataType, vdataList, commandList);
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return true;
	}
	psqlTool->InsertData(dataTableName, saveDataType, vdataList);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::SaveSingleData(
	const string& stockcode,
	const string& typeofdata,
	const CDate& dateIndex, 
	const KDJ& KDJdataToSave,
	const CKDJCal& calData)
{
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//生成表名
	string dataTableName = "";
	dataTableName = stockcode + "_" + typeofdata + "KDJ";
	//获得MACD的表头
	if (KDJdataHeader.empty())
		GetKDJColumnsType(KDJdataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(dataTableName))
		psqlTool->CreatTable(dataTableName, KDJdataHeader);
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(dataTableName, KDJdataHeader))
		psqlTool->AddColumnToTable(dataTableName, KDJdataHeader);

	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++KDJdataHeader.begin()), KDJdataHeader.end());
	vector<string> vdataList;

	psqlTool->DataConversion(saveDataType[0], dateIndex, vdataList);
	KDJDataConversion(saveDataType, KDJdataToSave, calData, vdataList);

	WhereCommand whereCom;
	vector<WhereCommand> commandList;
	commandList.push_back(whereCom.IniCommand(saveDataType[0].Dataname, _eIs, vdataList[0], _eAnd));

	if (psqlTool->CheckDataExists(dataTableName, commandList))//判断数据是不是已经存在，存在的话就将原数据更新
	{
		psqlTool->UpDateData(dataTableName, saveDataType, vdataList, commandList);
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return true;
	}
	psqlTool->InsertData(dataTableName, saveDataType, vdataList);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::SaveMACDCacheData()
{
	if (_vMacdDataToSave.size() == 0 ||_vMacdDataToSave.size() != _vMacdDate.size())
	{
		LOG(INFO) << "MACD cache data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得MACD的表头
	if (MACDDataHeader.empty())
		GetMacdColumnsType(MACDDataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_MacdTablename))
		psqlTool->CreatTable(_MacdTablename, MACDDataHeader);
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_MacdTablename, MACDDataHeader))
		psqlTool->AddColumnToTable(_MacdTablename, MACDDataHeader);
	//生成指令
	string command = "";
	command = "INSERT INTO " + _MacdTablename + "(" + MACDDataHeader[1].Dataname;
	for (unsigned int i = 2; i < MACDDataHeader.size();i++)
	{
		command += "," + MACDDataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetMACDValueString(valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
}


bool CNumbersToSql::SaveKDJCacheData()
{
	if (_vKDJDataToSave.size() == 0 || _vKDJDataToSave.size() != _vKDJDate.size())
	{
		LOG(INFO) << "KDJ cache data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得KDJ的表头
	if (KDJdataHeader.empty())
		GetKDJColumnsType(KDJdataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_KDJTablename))
		psqlTool->CreatTable(_KDJTablename, KDJdataHeader);
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_KDJTablename, KDJdataHeader))
		psqlTool->AddColumnToTable(_KDJTablename, KDJdataHeader);
	//生成指令
	string command = "";
	command = "INSERT INTO " + _KDJTablename + "(" + KDJdataHeader[1].Dataname;
	for (unsigned int i = 2; i < KDJdataHeader.size(); i++)
	{
		command += "," + KDJdataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetKDJValueString(valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
}


bool CNumbersToSql::SaveDayCacheData()
{
	if (_vHLCToSave.size() == 0 || _vHLCToSave.size() != _Dates.size())
	{
		LOG(INFO) << "Day cache data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得Day数据的表头
	if (DaydataHeader.empty())
		GetDayColumnsType(DaydataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_dataTebleName))
		psqlTool->CreatTable(_dataTebleName, DaydataHeader);
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_dataTebleName, DaydataHeader))
		psqlTool->AddColumnToTable(_dataTebleName, DaydataHeader);
	//生成指令
	string command = "";
	command = "INSERT INTO " + _dataTebleName + "(" + DaydataHeader[1].Dataname;
	for (unsigned int i = 2; i < DaydataHeader.size(); i++)
	{
		command += "," + DaydataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetDayValueString(valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
}

bool CNumbersToSql::IniMysqlTool()
{
	_MacdTablename = "";
	_vMacdDate.clear();
	_vMacdDataToSave.clear();

	_vKDJDataToSave.clear();
	_vKDJDate.clear();
	_KDJTablename.clear();

	_Dates.clear();
	_vHLCToSave.clear();
	_dataTebleName.clear();

// 	SaveData("S000001", "day", tempData, datatemp);
	return true;
}

void CNumbersToSql::GetMacdColumnsType(vector<Column>& vdataType)
{
	vdataType.clear();
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("date", _eDATE, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("m12", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("m26", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("diff", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("dea", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("bar", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);

// 	tempColumn.InitionData("SHORT_SMOOTH", _eINT, 0, _eNULL, "");
// 	vdataType.push_back(tempColumn);
// 	tempColumn.InitionData("LONG_SMOOTH", _eINT, 0, _eNULL, "");
// 	vdataType.push_back(tempColumn);
// 	tempColumn.InitionData("M", _eINT, 0, _eNULL, "");
// 	vdataType.push_back(tempColumn);
}

void CNumbersToSql::GetDayColumnsType(vector<Column>& vdataType)
{
	vdataType.clear();
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("date", _eDATE, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("high", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("low", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("open", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("close", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("vol", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);

}

void CNumbersToSql::GetKDJColumnsType(vector<Column>& vdataType)
{
	vdataType.clear();
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("date", _eDATE, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("KDJ_K", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("KDJ_D", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("KDJ_J", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);

}

bool CNumbersToSql::KDJDataConversion(
	const vector<Column>& saveDataType,
	const KDJ& KDJdataToSave, 
	const CKDJCal& calData, 
	vector<string>& vdataList)
{
	psqlTool->DataConversion(saveDataType[1], KDJdataToSave.K_OF_KDJ, vdataList);
	psqlTool->DataConversion(saveDataType[2], KDJdataToSave.D_OF_KDJ, vdataList);
	psqlTool->DataConversion(saveDataType[3], KDJdataToSave.J_OF_KDJ, vdataList);

	if (calData._lHigh.size() != 9 || calData._lLow.size() != 9|| calData._lClose.size() != 9)
		return false;

	list<StockDataType>::const_iterator ite = calData._lHigh.begin();
	unsigned int i = 0;
	while (ite != calData._lHigh.end()){
		psqlTool->DataConversion(saveDataType[i + 4], *ite, vdataList);
		ite++;
		i++;
	}
	ite = calData._lLow.begin();
	while (ite != calData._lLow.end()){
		psqlTool->DataConversion(saveDataType[i + 4], *ite, vdataList);
		ite++;
		i++;
	}
	ite = calData._lClose.begin();
	while (ite != calData._lClose.end()){
		psqlTool->DataConversion(saveDataType[i + 4], *ite, vdataList);
		ite++;
		i++;
	}

	return true;
}

void CNumbersToSql::InsertData(const string& stockcode, const string& typeofdata, const CDate& date, const Macd& dataToSave)
{
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//生成表名
	if (_MacdTablename.empty())
		_MacdTablename = stockcode + "_" + typeofdata + "MACD";
	_vMacdDate.push_back(date);
	_vMacdDataToSave.push_back(dataToSave);
	LeaveCriticalSection(&_NUMTOMYSQLCS);

}

bool CNumbersToSql::InsertData(const string& stockcode, const string& typeofdata, const CDate& date, const KDJ& dataToSave, const CKDJCal& calData)
{
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//生成表名
	if (_KDJTablename.empty())
		_KDJTablename = stockcode + "_" + typeofdata + "KDJ";
	_vKDJDate.push_back(date);
	_vKDJDataToSave.push_back(dataToSave);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::InsertData(
	const string& stockcode,
	const string& typeofdata,
	const CDate& date,
	const DayHLCOV& _daydata)
{
	EnterCriticalSection(&_NUMTOMYSQLCS);
	if (_dataTebleName.empty())
		_dataTebleName = stockcode + "_" + typeofdata + "_Data";
	_Dates.push_back(date);
	_vHLCToSave.push_back(_daydata);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::GetMACDValueString(string& valueString)
{
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++MACDDataHeader.begin()), MACDDataHeader.end());
// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _vMacdDate[0], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vMacdDataToSave[0].m12, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vMacdDataToSave[0].m26, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vMacdDataToSave[0].dea, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[4], _vMacdDataToSave[0].diff, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[5], _vMacdDataToSave[0].bar, vdataList) + ")";
	valueString = lineString;
	for (unsigned int i = 1; i < _vMacdDataToSave.size(); i++)
	{
		lineString = "";
		vdataList.clear();
		valueString +=
			",(" +
			psqlTool->DataConversion(saveDataType[0], _vMacdDate[i], vdataList) + "," +
			psqlTool->DataConversion(saveDataType[1], _vMacdDataToSave[i].m12, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[2], _vMacdDataToSave[i].m26, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[3], _vMacdDataToSave[i].dea, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[4], _vMacdDataToSave[i].diff, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[5], _vMacdDataToSave[i].bar, vdataList) + ")";
	}
	return true;
}

bool CNumbersToSql::GetKDJValueString(string& valueString)
{
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++KDJdataHeader.begin()), KDJdataHeader.end());
	// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _vKDJDate[0], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vKDJDataToSave[0].K_OF_KDJ, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vKDJDataToSave[0].D_OF_KDJ, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vKDJDataToSave[0].J_OF_KDJ, vdataList) + ")";
	valueString += lineString;
	for (unsigned int i = 1; i < _vKDJDataToSave.size(); i++)
	{
		lineString = "";
		vdataList.clear();
		valueString +=
			",(" +
			psqlTool->DataConversion(saveDataType[0], _vKDJDate[i], vdataList) + "," +
			psqlTool->DataConversion(saveDataType[1], _vKDJDataToSave[i].K_OF_KDJ, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[2], _vKDJDataToSave[i].D_OF_KDJ, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[3], _vKDJDataToSave[i].J_OF_KDJ, vdataList) + ")";
		
	}
	return true;
}

bool CNumbersToSql::GetDayValueString(string& valueString)
{
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++DaydataHeader.begin()), DaydataHeader.end());
	// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _Dates[0], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vHLCToSave[0]._HLCHigh, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vHLCToSave[0]._HLCLow, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vHLCToSave[0]._HLCClose, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[4], _vHLCToSave[0]._HLCOpen, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[5], _vHLCToSave[0]._HLCVol, vdataList) + ")";
		valueString = lineString;
	for (unsigned int i = 1; i < _vHLCToSave.size(); i++)
	{
		lineString = "";
		vdataList.clear();
		valueString +=
			",(" +
			psqlTool->DataConversion(saveDataType[0], _Dates[i], vdataList) + "," +
			psqlTool->DataConversion(saveDataType[1], _vHLCToSave[i]._HLCHigh, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[2], _vHLCToSave[i]._HLCLow, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[3], _vHLCToSave[i]._HLCClose, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[4], _vHLCToSave[i]._HLCOpen, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[5], _vHLCToSave[i]._HLCVol, vdataList) + ")";
	}
	return true;
}

bool CNumbersToSql::RecoveryData(const CDate& _date, Macd& _macd)
{

	return true;
}



