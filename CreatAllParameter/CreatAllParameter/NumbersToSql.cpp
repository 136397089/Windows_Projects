#include "stdafx.h"
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>
#include "NumbersToSql.h"
#include "DateTool.h"
#include "Locker.h"
#include "glog/logging.h"
using namespace std;

static const string DataBaseName = "StockData";
static MySQLInterFace* sql_index = NULL;
static vector<Column> IndicatorsHeader;
static vector<Column> ProGroupdataHeader;
static vector<Column> FreqGroupingTableHeader;
static CRITICAL_SECTION _NUMTOMYSQLCS;
static string _TemporaryNumberTableName = "TempNumber";
static map<DataTypeToSave, vector<StockDataType>> _IndiIndex;


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

bool CNumbersToSql::IniMysqlTool()
{
	_TemporaryNumberTableName = "TempNumber";
	return true;
}


CNumbersToSql::~CNumbersToSql()
{
// 	CloseHandle(_MYSQLMutex);
// 	DeleteCriticalSection(&MYSQLCS);
}





void CNumbersToSql::SaveProGroupRate(
	const string& tableName,
	const string& codeName,
	const vector<StockDataType>& returnRate,
	const vector<string>& GroupType) const
{
	if (GroupType.size() == 0)
		return;
	unsigned int columnSize = 1000;
	if (returnRate.size() > columnSize)
		return;
	string _TebleName = tableName;
	EnterCriticalSection(&_NUMTOMYSQLCS);
	if (ProGroupdataHeader.empty())
		SetProGroupColumnsType(ProGroupdataHeader, columnSize, GroupType);
	string strColumnName = "";
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(tableName))
	{
		psqlTool->CreatTable(tableName, ProGroupdataHeader);
		psqlTool->SetColumnIndex(tableName, "indexname");
	}
	//生成指令
	string command = "INSERT INTO " + tableName + "(" + ProGroupdataHeader[1].Dataname;
	for (unsigned int i = 2; i < ProGroupdataHeader.size(); i++)
	{
		command += "," + ProGroupdataHeader[i].Dataname;
	}
	string GroupTypeString = "";
	for (unsigned int groupIndex = 0; groupIndex < GroupType.size(); groupIndex++)
		GroupTypeString = GroupTypeString + "," + GroupType[groupIndex];
	command += ") VALUES (" + codeName + GroupTypeString;
	string valueString = "";
	GetReturnRateValueString(valueString, GroupType.size()+2/**/, returnRate);
	command = command + valueString + ");";
	sql_index->RunCommand(command);
	LeaveCriticalSection(&_NUMTOMYSQLCS);
}



void CNumbersToSql::GetNumberColumnsType(vector<Column>& vdataType)
{
	vdataType.clear();
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("CodeName", _eVARCHAR, 6, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("Cycle", _eINT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("date", _eDATETIME, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("NumberName", _eVARCHAR, 20, _eNULL, "");
	vdataType.push_back(tempColumn);
	ostringstream  dataoss;
	for (int i = 0; i < 1000; i++)
	{
		dataoss.str("");
		dataoss << "D" << i;
		tempColumn.InitionData(dataoss.str(), _eFLOAT, 0, _eNULL, "");
		vdataType.push_back(tempColumn);
	}
}


void CNumbersToSql::SetProGroupColumnsType(vector<Column>& vdataType, unsigned int typeSize, const vector<string>& GroupType) const
{
	vdataType.clear();
	ostringstream  DataOss;
	DataOss.str("");
	string strdateGroup = "";
	Column tempColumn;
	tempColumn.InitionData("indexname", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	vdataType.push_back(tempColumn);
	tempColumn.InitionData("CodeName", _eINT, 0, _eNULL, "");
	vdataType.push_back(tempColumn);
	ostringstream ossData;
	for (unsigned int i = 0; i < GroupType.size(); i++)
	{
		ossData.str("");
		ossData << "GroupType" << (unsigned int)i;
		tempColumn.InitionData(ossData.str(), _eINT, 0, DataAttribute(_eNULL | _eUNSIGNED), "");
		vdataType.push_back(tempColumn);
	}
	for (unsigned int i = 1; i <= typeSize; i++)
	{
		DataOss.str("");
		DataOss << "G" << i;
		strdateGroup = DataOss.str();
		tempColumn.InitionData(strdateGroup, _eFLOAT, 0, _eNULL, "");
		vdataType.push_back(tempColumn);
	}
}



bool CNumbersToSql::GetNumberValueString(string& valueString, const vector<StockDataType>& _vIndicatorsDataToSave, const DataTypeToSave& numbertype)
{
	if ( 0 == _vIndicatorsDataToSave.size())
	{
		LOG(ERROR) << "begin Inedx error.";
		return false;
	}
	//第一个表头是索引值，会自动增长，不需要指定值，所以去除第一个表头
	vector<Column> saveDataType((++IndicatorsHeader.begin()), IndicatorsHeader.end());
	valueString = "";
	valueString = "(" 
		+ psqlTool->DataConversion(saveDataType[0], numbertype.StockCode) + ","
		+ psqlTool->DataConversion(saveDataType[1], (int)numbertype.cycle) + ","
		+ psqlTool->DataConversion(saveDataType[2], numbertype._time) + ","
		+ psqlTool->DataConversion(saveDataType[3], numbertype.IndicatorsName);
	for (unsigned int i = 0; i < _vIndicatorsDataToSave.size(); i++)
	{
		valueString += ","+psqlTool->DataConversion(saveDataType[5+i], _vIndicatorsDataToSave[i]) ;
	}
	valueString += ")";
	return true;
}



unsigned int CNumbersToSql::GetBeginIndexToSave(vector<CDate>& _vDateList, const string& tebleName)
{
	Column tempcolumn;
	tempcolumn.Dataname = "date";
	vector<Column> columnlist;
	columnlist.push_back(tempcolumn);
	vector<vector<string>> result;
	sql_index->ReadDataFromTable(tebleName, columnlist, result);
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


bool CNumbersToSql::GetDuplicataUpdateValue(vector<Column>& vdataType, unsigned int beginIndex, unsigned int count, string& result)
{
	result = "";
	if (vdataType.size() <= beginIndex || beginIndex + count > vdataType.size())
	{
		return false;
	}
	result += " on duplicate key update " + vdataType[beginIndex].Dataname + "=values(" + vdataType[beginIndex].Dataname + ")";
	for (unsigned int i = beginIndex + 1; i < beginIndex + count; i++)
	{
		result += " ," + vdataType[i].Dataname + "=values(" + vdataType[i].Dataname + ")";
	}
	return true;
}




bool CNumbersToSql::RecoveryPriceData(
	const string& stockCode,
	const string& typeofdata,
	vector<SinCyclePriceData>& FrontPrice)
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
	for (unsigned int index = 0; index < FrontPrice.size(); index++)
	{
		SinCyclePriceData& oneDayPrice = FrontPrice[index];
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



int CNumbersToSql::CheckStockFundCount(const string& stockcode)
{
	string sqlIns = "select count from fundstockcount_2019_03_31 where code='" + stockcode + "';";
	EnterCriticalSection(&_NUMTOMYSQLCS);
	if (!sql_index->RunCommand(sqlIns))
	{
		LOG(ERROR) << "Get stockfund count error.";
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return 0;
	}
	vector<vector<string>> resultTable;
	sql_index->GetTableResult(resultTable);
	if (resultTable.size() != 2)
	{
		LOG(ERROR) << "stockfund result table count error.";
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return 0;
	}
	vector<string> countData = resultTable[1];
	if (countData.size() != 1)
	{
		LOG(ERROR) << "count data error.";
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return 0;
	}
	int count = atoi(countData[0].c_str());
	LeaveCriticalSection(&_NUMTOMYSQLCS);
	return count;
}

float CNumbersToSql::ReadPEFromSQL(const string& stockcode)
{
	string tablename = "financedata.basics_2018_1231";
	string sqlIns = "select pe from " + tablename + " where code=" + stockcode + ";";
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

bool CNumbersToSql::GetReturnRateValueString(string& valueString, unsigned int typeColumnSize, const vector<StockDataType>& returnRate) const
{
	ostringstream dataOss;
	for (unsigned int i = 0; i < ProGroupdataHeader.size() - typeColumnSize; i++)
	{
		if (i < returnRate.size())
			dataOss << "," << returnRate[i];
		else
			dataOss << ",1000";
	}
	valueString = dataOss.str();
	return true;
}

bool CNumbersToSql::SaveTemporaryIndicatorsData(const DataTypeToSave& numbertype,const vector<StockDataType>& _DataToSave)
{

	if (_DataToSave.size() == 0)
	{
		LOG(ERROR) << "data cache-data size error.";
		return false;
	}
	EnterCriticalSection(&_NUMTOMYSQLCS);
	//获得表头
	if (IndicatorsHeader.empty())
		GetNumberColumnsType(IndicatorsHeader);
	//如果表不存在则创建新表
	if (!psqlTool->CheckTableExists(_TemporaryNumberTableName))
	{
		psqlTool->CreatTable(_TemporaryNumberTableName, IndicatorsHeader);	
 		psqlTool->SetColumnUnique(_TemporaryNumberTableName, "CodeName,Cycle,date,NumberName");
	}

	//生成指令
	string command = "";
	command = "INSERT IGNORE INTO " + _TemporaryNumberTableName + "(" + IndicatorsHeader[1].Dataname;
	for (unsigned int i = 2; i < _DataToSave.size()+5; i++)
	{
		command += "," + IndicatorsHeader[i].Dataname;
	}
	command += ") VALUES ";
	string valuString = "";
	GetNumberValueString(valuString, _DataToSave, numbertype);
	command += valuString;
	GetDuplicataUpdateValue(IndicatorsHeader, 2, _DataToSave.size() + 5, valuString);
	command = command + valuString + ";";
	if (!sql_index->RunCommand(command))
		LOG(ERROR) << "Save Number data command error.";
	LeaveCriticalSection(&_NUMTOMYSQLCS);

	return true;
}

bool CNumbersToSql::GetTemporaryNumberData(const DataTypeToSave& _indicatorsType, vector<StockDataType>& _DataReadFromSQL)
{
	_DataReadFromSQL.clear();
	if (_IndiIndex.count(_indicatorsType) != 0)
	{
		LOG(INFO) << "_TemporaryIndi";
		_DataReadFromSQL.assign(_IndiIndex[_indicatorsType].begin(), _IndiIndex[_indicatorsType].end());
		return true;
	}
	LOG(INFO) << "Can not read data from SQL:" << _indicatorsType.StockCode << " time:" << _indicatorsType._time.GetDateTime() << " Name:" << _indicatorsType.IndicatorsName << " cycle:" << _indicatorsType.cycle;
	return false;
	vector<WhereCommand> whereList;
	WhereCommand command1;
	command1.IniCommand("CodeName", _eIs, _indicatorsType.StockCode, _eAnd);
	whereList.push_back(command1);
	command1.IniCommand("Cycle", _eIs, _indicatorsType.cycle, _eAnd);
	whereList.push_back(command1);
	command1.IniCommand("date", _eIs, "'" + _indicatorsType._time.GetDateTime() + "'", _eAnd);
	whereList.push_back(command1);
	command1.IniCommand("NumberName", _eIs, "'" + _indicatorsType.IndicatorsName + "'", _eAnd);
	whereList.push_back(command1);

	vector<Column> columnTypeList;
	vector<vector<string>> _datas;
	EnterCriticalSection(&_NUMTOMYSQLCS);
	sql_index->ReadDataFromTable(_TemporaryNumberTableName, columnTypeList, whereList, _datas);
	if (_datas.size() != 2)
	{
		return false;
		LeaveCriticalSection(&_NUMTOMYSQLCS);
	}
	vector<string>& ReturnData = _datas[1];
	for (unsigned int i = 5; i < ReturnData.size(); i++)
	{
		if (ReturnData[i]=="NULL")
			break;
		_DataReadFromSQL.push_back(atof(ReturnData[i].c_str()));
	}
	LeaveCriticalSection(&_NUMTOMYSQLCS);

	return true;
}

bool CNumbersToSql::RefreshTemporaryNumberData(const CDate& _date)
{
	_IndiIndex.clear();
	vector<WhereCommand> whereList;
	WhereCommand command1;
	command1.IniCommand("date", _eIs, "'" + _date.GetDateTime() + "'", _eAnd);
	whereList.push_back(command1);

	vector<Column> columnTypeList;
	vector<vector<string>> _datas;
	EnterCriticalSection(&_NUMTOMYSQLCS);
	sql_index->ReadDataFromTable(_TemporaryNumberTableName, columnTypeList, whereList, _datas);
	if (_datas.size() < 2)
	{
		LeaveCriticalSection(&_NUMTOMYSQLCS);
		return false;
	}
	DataTypeToSave datatype;
	vector<StockDataType> IndicatorData;
	for (unsigned int j = 1; j < _datas.size() ;j++)
	{
		IndicatorData.clear();
		vector<string>& ReturnData = _datas[j];
		datatype.StockCode = ReturnData[1];
		datatype.cycle = (BasisCycleType)atoi(ReturnData[2].c_str());
		datatype._time.SetDay(ReturnData[3]);
		datatype.IndicatorsName = ReturnData[4];
		for (unsigned int i = 5; i < ReturnData.size(); i++)
		{
			if (ReturnData[i] == "NULL")
				break;
			IndicatorData.push_back(atof(ReturnData[i].c_str()));
		}
		_IndiIndex[datatype] = IndicatorData;
	}
	LeaveCriticalSection(&_NUMTOMYSQLCS);

	return true;
}

bool CNumbersToSql::GetCurrentData(map<RealDataIndex, SinCyclePriceData>& TodayRealtimeData, vector<RealDataIndex>& _TableType)
{
	//获得当前时间
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	CDate NowTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	//读数据表
	vector<string> _tableList;
	sql_index->GetTableList(_tableList);
	//
	RealDataIndex rDataIndex;
	vector<Column> columnTypeList;
	vector<vector<string>> allDataFromSql;
	SinCyclePriceData priceData;
	stringstream ss;
	for (unsigned int i = 0; i < _tableList.size(); i++)
	{
		if (CheckRealtimeTableName(_tableList[i], rDataIndex._date, rDataIndex._cycletype))
		{
			rDataIndex._stockCode = 0;
			if (!NowTime.IsOnTheSameDay(rDataIndex._date))//如果不是今天的数据则不读出，本函数只对今日的数据有处理
				continue;
			//判断表内的数据是不是已经不更新，已经没有更新的数据可以不再读出
			if (find(_TableType.begin(), _TableType.end(), rDataIndex) != _TableType.end())
				continue;
			sql_index->ReadDataFromTable(_tableList[i], columnTypeList, allDataFromSql);
			int dataSameNumberOfTimes = 0;
			for (unsigned int j = 1; j < allDataFromSql.size();j++)
			{
				if (allDataFromSql[j].size() != 7)
				{
					LOG(ERROR) << "Data read from realtime table error.";
					return false;
				}
				if (dataSameNumberOfTimes > 10)//连续50次的数据是一样的话，判定为目标数据表已经没有更新，后面不再读出
				{
					rDataIndex._stockCode = 0;
					LOG(INFO) << "Get update type." << "-" << rDataIndex._cycletype << "-" << rDataIndex._date.GetDateTime();
					_TableType.push_back(rDataIndex);
					break;
				}
				rDataIndex.SetStockCode(allDataFromSql[j][0]);
				priceData._Open = atof(allDataFromSql[j][1].c_str());
				priceData._Close = atof(allDataFromSql[j][2].c_str());
				priceData._High = atof(allDataFromSql[j][3].c_str());
				priceData._Low = atof(allDataFromSql[j][4].c_str());
				priceData._Volume = atof(allDataFromSql[j][5].c_str());
				if (TodayRealtimeData[rDataIndex] == priceData)
				{
					dataSameNumberOfTimes++;
					continue;
				}
				TodayRealtimeData[rDataIndex] = priceData;
				dataSameNumberOfTimes = 0;
			}
			allDataFromSql.clear();
		}
	}
	return true;
}

bool CNumbersToSql::CheckRealtimeTableName(const string& tableName,CDate& tebleDate, BasisCycleType& tableCycleType)
{
	if (tableName.find("cycletype") != 0)
	{
		LOG(INFO) << "it is not data table" << tableName;
		return false;
	}
	vector<string> stringList;
	CutString(tableName, "_", stringList);
	if (stringList.size() != 6)
	{
		LOG(INFO) << "string list size error.";
		return false;

	}
	tebleDate._year = atoi(stringList[1].c_str());
	tebleDate._month = atoi(stringList[2].c_str());
	tebleDate._day = atoi(stringList[3].c_str());
	tebleDate._hour = atoi(stringList[4].c_str());
	tebleDate._minute = atoi(stringList[5].c_str());
	tebleDate._second = 0;
	if ("cycletype5" == stringList[0])
		tableCycleType = eMinute5;
	else if ("cycletype15" == stringList[0])
		tableCycleType = eMinute15;
	else if ("cycletype30" == stringList[0])
		tableCycleType = eMinute30;
	else if ("cycletype60" == stringList[0])
		tableCycleType = eMinute60;
	else if ("cycletype120" == stringList[0])
		tableCycleType = eMinute120;
	else if ("cycletypeday" == stringList[0])
		tableCycleType = eDay;
	else if ("cycletypeweek" == stringList[0])
		tableCycleType = eWeek;
	else if ("cycletypemonth" == stringList[0])
		tableCycleType = eMonth;
	LOG(INFO) << "tebleDate" << tebleDate.GetDateTime() << "tableCycleType" << tableCycleType;
	return true;
}


void DataTypeToSave::Inition(const string& StockName, BasisCycleType _cycle, const CDate& strtime, const string& Number)
{
	StockCode = StockName;
	cycle = _cycle;
	_time = strtime;
	IndicatorsName = Number;
}

bool DataTypeToSave::operator==(const DataTypeToSave& d) const
{
	return(StockCode == d.StockCode
		&& cycle == d.cycle
		&& _time == d._time
		&& IndicatorsName == d.IndicatorsName);
}

bool DataTypeToSave::operator<(const DataTypeToSave& d) const
{
	return (cycle < d.cycle)
		|| (cycle == d.cycle && _time < d._time)
		|| (cycle == d.cycle && _time == d._time && StockCode < d.StockCode)
		|| (cycle == d.cycle && _time == d._time && StockCode == d.StockCode && IndicatorsName < d.IndicatorsName);

}

RealDataIndex& RealDataIndex::operator=(const RealDataIndex& d)
{
	_stockCode = d._stockCode;
	_cycletype = d._cycletype;
	_date = d._date;
	return *this;
}

bool RealDataIndex::operator==(const RealDataIndex& d) const
{
	return (_date == d._date &&  _cycletype == d._cycletype && _stockCode == d._stockCode);
}

bool RealDataIndex::operator < (const RealDataIndex& d) const
{
	return (_cycletype < d._cycletype)
		|| (_cycletype == d._cycletype && _date < d._date)
		|| (_cycletype == d._cycletype && _date == d._date && _stockCode < d._stockCode);
}

bool RealDataIndex::operator<=(const RealDataIndex& d) const
{
	return (*this == d || *this < d);
}

void RealDataIndex::SetStockCode(const string& codename)
{
	_stockCode = atoi(codename.c_str());
}
