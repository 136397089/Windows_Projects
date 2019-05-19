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
static vector<Column> FreqGroupingTableHeader;
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
	psqlTool->DataConversion(saveDataType[4], dataToSave.dif,vdataList);
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
		LOG(ERROR) << "MACD cache-data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得MACD的表头
	if (MACDDataHeader.empty())
		GetMacdColumnsType(MACDDataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_MacdTablename))
	{
		psqlTool->CreatTable(_MacdTablename, MACDDataHeader);
		psqlTool->SetColumnUnique(_MacdTablename, "date");
	}
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_MacdTablename, MACDDataHeader))
		psqlTool->AddColumnToTable(_MacdTablename, MACDDataHeader);
	//生成指令
	unsigned int beginIndex = GetBeginIndexToSave(_vMacdDate, _MacdTablename);
	string command = "";
	command = "INSERT IGNORE INTO " + _MacdTablename + "(" + MACDDataHeader[1].Dataname;
	for (unsigned int i = 2; i < MACDDataHeader.size();i++)
	{
		command += "," + MACDDataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetMACDValueString(valuString, beginIndex);
	command += valuString;
	GetDuplicataUpdateValue(MACDDataHeader, 2, valuString);
	command = command + valuString + ";";
	if (!sql_index->RunCommand(command))
	{
		LOG(ERROR) << "Save MACD data command error.";
	}
	_vMacdDate.clear();
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}


bool CNumbersToSql::SaveKDJCacheData()
{
	if (_vKDJDataToSave.size() == 0 || _vKDJDataToSave.size() != _vKDJDate.size())
	{
		LOG(ERROR) << "KDJ cache-data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得KDJ的表头
	if (KDJdataHeader.empty())
		GetKDJColumnsType(KDJdataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_KDJTablename))
	{
		psqlTool->CreatTable(_KDJTablename, KDJdataHeader);
		psqlTool->SetColumnUnique(_KDJTablename, "date");
	}
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_KDJTablename, KDJdataHeader))
		psqlTool->AddColumnToTable(_KDJTablename, KDJdataHeader);
	//生成指令
	unsigned int beginIndex = GetBeginIndexToSave(_vKDJDate, _KDJTablename);
	string command = "";
	command = "INSERT IGNORE INTO " + _KDJTablename + "(" + KDJdataHeader[1].Dataname;
	for (unsigned int i = 2; i < KDJdataHeader.size(); i++)
	{
		command += "," + KDJdataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetKDJValueString(valuString, beginIndex);
	command += valuString;
	GetDuplicataUpdateValue(KDJdataHeader, 2, valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	_vKDJDate.clear();
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}


bool CNumbersToSql::SaveDayCacheData()
{
	if (_vDayDataToSave.size() == 0 || _vDayDataToSave.size() != _DayDataDates.size())
	{
		LOG(ERROR) << "Day cache-data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得Day数据的表头
	if (DaydataHeader.empty())
		GetDayColumnsType(DaydataHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_dataTebleName))
	{
		psqlTool->CreatTable(_dataTebleName, DaydataHeader);
		psqlTool->SetColumnUnique(_dataTebleName, "date");
	}
	//如果表头不存在，则加入新列
	if (!psqlTool->CheckTableColumnsExists(_dataTebleName, DaydataHeader))
		psqlTool->AddColumnToTable(_dataTebleName, DaydataHeader);
	//设置date列不可以重复
	//生成指令
	unsigned int beginIndex = GetBeginIndexToSave(_DayDataDates, _dataTebleName);
	string command = "";
	command = "INSERT IGNORE INTO " + _dataTebleName + "(" + DaydataHeader[1].Dataname;
	for (unsigned int i = 2; i < DaydataHeader.size(); i++)
	{
		command += "," + DaydataHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetDayValueString(valuString, beginIndex);
	command += valuString;
	GetDuplicataUpdateValue(DaydataHeader, 2, valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	_DayDataDates.clear();
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::SaveFreqCacheData()
{
	if (_vFreqDataToSave.size() == 0)
	{
		LOG(ERROR) << "Frequency cache-data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得Day数据的表头
	if (FreqGroupingTableHeader.empty())
		SetFreqGroupingColumnsType(_beginIndex,_endindex);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_freqTebleName))
	{
		psqlTool->CreatTable(_freqTebleName, FreqGroupingTableHeader);
		//设置以下三列是分组，不可以重复
		vector<string> _ColumnName;
		_ColumnName.push_back(FreqGroupingTableHeader[1].Dataname);
		_ColumnName.push_back(FreqGroupingTableHeader[2].Dataname);
		_ColumnName.push_back(FreqGroupingTableHeader[3].Dataname);
		psqlTool->SetColumnCombinUnique(_freqTebleName,_ColumnName);
	}
	//如果表头不存在，则加入新列
	//if (!psqlTool->CheckTableColumnsExists(_freqTebleName, FreqGroupingTableHeader))
	//	psqlTool->AddColumnToTable(_freqTebleName, FreqGroupingTableHeader);
	//生成指令
	//unsigned int beginIndex = GetBeginIndexToSave(_DayDataDates, _freqTebleName);
	string command = "";
	//这里人DaydataHeader[1]插入数据，因为第一个columns是自动增长，所以插入数据时不需要指定具体的值
	command = "INSERT INTO " + _freqTebleName + "(" + FreqGroupingTableHeader[1].Dataname;
	for (unsigned int i = 2; i < FreqGroupingTableHeader.size(); i++)
	{
		command += "," + FreqGroupingTableHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetFreqValueString(valuString, 0);
	command += valuString;
	GetDuplicataUpdateValue(FreqGroupingTableHeader, 4, valuString);
	command += valuString + ";";
	sql_index->RunCommand(command);
	_vFreqDataToSave.clear();
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}




bool CNumbersToSql::IniMysqlTool()
{
	_MacdTablename = "";
	_vMacdDate.clear();
	_vMacdDataToSave.clear();

	_vKDJDataToSave.clear();
	_vKDJDate.clear();
	_KDJTablename.clear();

	_DayDataDates.clear();
	_vDayDataToSave.clear();
	_dataTebleName.clear();

	_freqTebleName.clear();
	_freqTebleName.clear();
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
	_DayDataDates.push_back(date);
	_vDayDataToSave.push_back(_daydata);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return true;
}

bool CNumbersToSql::InsertData(
	const string& stockcode, 
	const vector<unsigned int>& TypeAndFreqData,
	const vector<float>& meanVarData,
	int beginIndex,
	int endIndex)
{
	if (TypeAndFreqData.size() == 0 || meanVarData.size() != 2)
	{
		return false;
	}
	_beginIndex = beginIndex;
	_endindex = endIndex;
	_freqTebleName = stockcode + "_FreqData";
	string OneRowOfData = "(";
	ostringstream  dataoss;
	dataoss.str("");
	dataoss << TypeAndFreqData[0];
	OneRowOfData += dataoss.str();
	for (unsigned int i = 1; i < TypeAndFreqData.size();i++)
	{
		dataoss.str("");
		dataoss << TypeAndFreqData[i];
		OneRowOfData += ","+dataoss.str();
	}
	dataoss.str("");
	dataoss << meanVarData[0];
	OneRowOfData += "," + dataoss.str();
	dataoss.str("");
	dataoss << meanVarData[1];
	OneRowOfData += "," + dataoss.str()+")";
	_vFreqDataToSave.push_back(OneRowOfData);
	return true;
}

bool CNumbersToSql::GetMACDValueString(string& valueString,unsigned int beginInedx)
{
	if (beginInedx >= _vMacdDataToSave.size())
	{
		LOG(ERROR) << "begin Inedx error.";
		return false;
	}
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++MACDDataHeader.begin()), MACDDataHeader.end());
// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _vMacdDate[beginInedx], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vMacdDataToSave[beginInedx].m12, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vMacdDataToSave[beginInedx].m26, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vMacdDataToSave[beginInedx].dif, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[4], _vMacdDataToSave[beginInedx].dea, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[5], _vMacdDataToSave[beginInedx].bar, vdataList) + ")";
	valueString = lineString;
	for (unsigned int i = beginInedx+1; i < _vMacdDataToSave.size(); i++)
	{
		lineString = "";
		vdataList.clear();
		valueString +=
			",(" +
			psqlTool->DataConversion(saveDataType[0], _vMacdDate[i], vdataList) + "," +
			psqlTool->DataConversion(saveDataType[1], _vMacdDataToSave[i].m12, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[2], _vMacdDataToSave[i].m26, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[3], _vMacdDataToSave[i].dif, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[4], _vMacdDataToSave[i].dea, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[5], _vMacdDataToSave[i].bar, vdataList) + ")";
	}
	return true;
}

bool CNumbersToSql::GetKDJValueString(string& valueString, unsigned int beginInedx)
{
	if (beginInedx >= _vKDJDate.size())
	{
		LOG(ERROR) << "begin Inedx error.";
		return false;
	}
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++KDJdataHeader.begin()), KDJdataHeader.end());
	// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _vKDJDate[beginInedx], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vKDJDataToSave[beginInedx].K_OF_KDJ, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vKDJDataToSave[beginInedx].D_OF_KDJ, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vKDJDataToSave[beginInedx].J_OF_KDJ, vdataList) + ")";
	valueString += lineString;
	for (unsigned int i = beginInedx+1; i < _vKDJDataToSave.size(); i++)
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

bool CNumbersToSql::GetDayValueString(string& valueString, unsigned int beginInedx)
{
	if (beginInedx >= _vDayDataToSave.size())
	{
		LOG(ERROR) << "begin Inedx error.";
		return false;
	}
	valueString = "";
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++DaydataHeader.begin()), DaydataHeader.end());
	// 	string valueString = "";
	string lineString = "";
	vector<string> vdataList;

	lineString =
		"(" +
		psqlTool->DataConversion(saveDataType[0], _DayDataDates[beginInedx], vdataList) + "," +
		psqlTool->DataConversion(saveDataType[1], _vDayDataToSave[beginInedx]._HLCHigh, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[2], _vDayDataToSave[beginInedx]._HLCLow, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[3], _vDayDataToSave[beginInedx]._HLCClose, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[4], _vDayDataToSave[beginInedx]._HLCOpen, vdataList) + "," +
		psqlTool->DataConversion(saveDataType[5], _vDayDataToSave[beginInedx]._HLCVol, vdataList) + ")";
		valueString = lineString;
		for (unsigned int i = beginInedx+1; i < _vDayDataToSave.size(); i++)
	{
		lineString = "";
		vdataList.clear();
		valueString +=
			",(" +
			psqlTool->DataConversion(saveDataType[0], _DayDataDates[i], vdataList) + "," +
			psqlTool->DataConversion(saveDataType[1], _vDayDataToSave[i]._HLCHigh, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[2], _vDayDataToSave[i]._HLCLow, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[3], _vDayDataToSave[i]._HLCClose, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[4], _vDayDataToSave[i]._HLCOpen, vdataList) + "," +
			psqlTool->DataConversion(saveDataType[5], _vDayDataToSave[i]._HLCVol, vdataList) + ")";
	}
	return true;
}


unsigned int CNumbersToSql::GetBeginIndexToSave(vector<CDate>& _vDateList, const string& tebleName)
{
	Column tempcolumn;
	tempcolumn.Dataname = "date";
	vector<Column> columnlist;
	columnlist.push_back(tempcolumn);
	vector<vector<string>> result;
	sql_index->SearchDataFromTable(tebleName, columnlist, result);
	if (result.size() <=1)
		return 0;
	vector<string> lastData = result.back();
	string lastDate = lastData.back();
	CDate LastdateFromSQl;
	LastdateFromSQl.SetDay(lastDate);
	for (unsigned int i = 0; i < _vDateList.size();i++)
	{
		if (_vDateList[i] > LastdateFromSQl)
		{
			return i;
		}
	}
	return (_vDateList.size()-1);
}


//生成频次统计表的表头
void CNumbersToSql::SetFreqGroupingColumnsType(int beginGroupIndex,int endGroupIndex)
{
	vector<Column>&vdataType = FreqGroupingTableHeader;
	if (vdataType.size() != 0)
		return;
	vdataType.clear();
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("NumType", _eINT, 0, (DataAttribute)(_eNULL | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("NPType", _eINT, 0, (DataAttribute)(_eNULL | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("CRType", _eINT, 0, (DataAttribute)(_eNULL | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	ostringstream  dataoss;
	string groupIndex;
	for (int i = beginGroupIndex; i < endGroupIndex; i++)
	{
		dataoss.str("");
		if (i < 0)
			dataoss << "GN_" << -i;
		else
			dataoss << "GP_" << i;
		tempColumn.InitionData(dataoss.str(), _eINT, 0, _eNULL, "");
		vdataType.push_back(tempColumn);
	}
	tempColumn.InitionData("Count", _eINT, 0, (DataAttribute)(_eNULL | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("Mean", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("Var", _eFLOAT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
}

bool CNumbersToSql::GetFreqValueString(string& valueString, unsigned int beginInedx)
{
	valueString = "";
	if (_vFreqDataToSave.size() == 0)
	{
		valueString = "";
		return true;
	}
	valueString = _vFreqDataToSave[0];
	for (unsigned int index = 1; index < _vFreqDataToSave.size(); index++)
	{
		valueString = "," + _vFreqDataToSave[index];
	}
	return true;
}

bool CNumbersToSql::GetDuplicataUpdateValue(vector<Column>& vdataType,unsigned int beginIndex,string& result)
{
	result = "";
	if (vdataType.size() <= beginIndex )
	{
		return false;
	}
	result += " on duplicate key update " + vdataType[beginIndex].Dataname + "=values(" + vdataType[beginIndex].Dataname + ")";
	for (unsigned int i = beginIndex +1; i < vdataType.size();i++)
	{
		result += " ," + vdataType[i].Dataname + "=values(" + vdataType[i].Dataname + ")";
	}
	return true;
}

bool CNumbersToSql::RecoveryMACDData(
	const string& stockCode,
	const string& typeofdata,
	MACDCombin& ReturnMacd
	)
{
	string sqlcommand = "select date,m12,m26,diff,dea,bar from " + stockCode + "_" + typeofdata + "MACD order by date desc limit 3;";
	if (!sql_index->RunCommand(sqlcommand))
		return false;
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() != 4)
	{
		LOG(ERROR) << "can not find macd data from mysql.";
		LOG(ERROR) << "error command: " << sqlcommand;
		return false;
	}

	vector<string>& macdstrData = resultTable[1];
	StringChangelateToMACD(resultTable[1], ReturnMacd.Today, ReturnMacd.TodayMacd);
	StringChangelateToMACD(resultTable[2], ReturnMacd.Yesterday, ReturnMacd.YesterdayMacd);
	StringChangelateToMACD(resultTable[3], ReturnMacd.BefoYes, ReturnMacd.BefoYesMacd);

	return true;
}
bool CNumbersToSql::StringChangelateToMACD(const vector<string>& macdstrData, CDate& _date, Macd& MacdData)
{
	if (macdstrData.size() != 6)
	{
		LOG(ERROR) << "error:return data size error.";
		return false;
	}
	_date.SetDay(macdstrData[0].c_str());
	MacdData.m12 = (StockDataType)atof(macdstrData[1].c_str());
	MacdData.m26 = (StockDataType)atof(macdstrData[2].c_str());
	MacdData.dif = (StockDataType)atof(macdstrData[3].c_str());
	MacdData.dea = (StockDataType)atof(macdstrData[4].c_str());
	MacdData.bar = (StockDataType)atof(macdstrData[5].c_str());

	return true;
}

bool CNumbersToSql::StringChangelateToKDJ(const vector<string>& kdjstrData, CDate& _date, KDJ& KDJData)
{
	if (kdjstrData.size() != 4)
	{
		LOG(ERROR) << "error:return data size error.";
		return false;
	}
	_date.SetDay(kdjstrData[0].c_str());
	KDJData.K_OF_KDJ = (StockDataType)atof(kdjstrData[1].c_str());
	KDJData.D_OF_KDJ = (StockDataType)atof(kdjstrData[2].c_str());
	KDJData.J_OF_KDJ = (StockDataType)atof(kdjstrData[3].c_str());
	return true;
}

bool CNumbersToSql::RecoveryKDJData(
	const string& stockCode,
	const string& typeofdata,
	KDJCombin& KDJReturn)
{
	string sqlcommand = "select date,KDJ_K,KDJ_D,KDJ_J from " + stockCode + "_" + typeofdata + "KDJ order by date desc limit 3;";
	if (!sql_index->RunCommand(sqlcommand))
		return false;
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() < 4)
	{
		LOG(ERROR) << "can not find kdj data from mysql.";
		LOG(ERROR) << "error command: " << sqlcommand;
		return false;
	}
	StringChangelateToKDJ(resultTable[1], KDJReturn.Today, KDJReturn.TodayKDJ);
	StringChangelateToKDJ(resultTable[2], KDJReturn.Yesterday, KDJReturn.YesterdayKDJ);
	StringChangelateToKDJ(resultTable[3], KDJReturn.BefYes, KDJReturn.BefYesKDJ);

	return true;
}

bool CNumbersToSql::RecoveryPriceData(
	const string& stockCode,
	const string& typeofdata,
	vector<SinDayPriceData>& FrontPrice)
{
	ostringstream osSize;
	osSize << FrontPrice.size();
	string sqlcommand = "";
	sqlcommand = "select date,open,high,low,close,vol from " + stockCode + "_" + typeofdata
		+ "_data  order by date desc limit " + osSize.str() + ";";
	if (!sql_index->RunCommand(sqlcommand))
		return false;
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() < FrontPrice.size()+1)
	{
		LOG(ERROR) << "can not find price data from mysql.stockCode="<<stockCode;
		LOG(ERROR) << "error command: " << sqlcommand;
		return false;
	}
	for (int index = 0; index < FrontPrice.size(); index++)
	{
		SinDayPriceData& oneDayPrice = FrontPrice[index];
		vector<string>& priceStrData = resultTable[index+1];
		if (priceStrData.size() != 6)
		{
			LOG(ERROR) << "Price data from mysql size error. " << sqlcommand;
			return false;
		}
		oneDayPrice.mDate.SetDay(priceStrData[0].c_str());
		oneDayPrice._Open = (StockDataType) atof(priceStrData[1].c_str());
		oneDayPrice._High = (StockDataType) atof(priceStrData[2].c_str());
		oneDayPrice._Low = (StockDataType) atof(priceStrData[3].c_str());
		oneDayPrice._Close = (StockDataType)atof(priceStrData[4].c_str());
		oneDayPrice._Volume = (StockDataType)atof(priceStrData[5].c_str());
		//FrontPrice.push_back(oneDayPrice);
	}

	return true;
}


bool CNumbersToSql::GetMacd(
	const string& stockCode,
	const string& typeofdata,
	MACDCombin& _macd)
{
	RecoveryMACDData(stockCode, typeofdata, _macd);
	return true;
}

int CNumbersToSql::CheckStockFundCount(const string& stockcode)
{
	string sqlIns = "select count from financedata.fundstockcount_2019_03_31 where code=" + stockcode + ";";
	if (!sql_index->RunCommand(sqlIns))
		return 0;
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() != 2)
		return 0;
	vector<string> countData = resultTable[1];
	if (countData.size() != 1)
		return 0;
	int count = atoi(countData[0].c_str());
	return count;
}

float CNumbersToSql::ReadPEFromSQL(const string& stockcode)
{
	string sqlIns = "select pe from financedata.basics_2018_1231 where code=" + stockcode + ";";
	if (!sql_index->RunCommand(sqlIns))
		return 0;
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() != 2)
		return 0;
	vector<string> countData = resultTable[1];
	if (countData.size() != 1)
		return 0;
	float count = atof(countData[0].c_str());
	return count;

}



