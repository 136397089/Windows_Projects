#include "stdafx.h"
#include <string>
#include <map>
#include <algorithm>
#include "MySQLInterFace.h"
#include "glog/logging.h"
#pragma comment(lib, "libmysql.lib")

MySQLInterFace::MySQLInterFace()
{
}
MySQLInterFace::~MySQLInterFace()
{
}

bool MySQLInterFace::Inition()
{
	if (NULL == mysql_init(&mydata)){
		_lastError = "mysql_init()failed";
		return false;
	}

	if (0 != mysql_library_init(0, NULL, NULL)){
		_lastError = "mysql_library_init()failed";
		return false;
	}

	if (0 != mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		_lastError = "mysql_options() failed";
		return false;
	}
	//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "", "", 3306, NULL, 0)){
		_lastError = "mysql_real_connect() failed" ;
		return false;
	}
	vector<string> Datalist;
	vector<Column> dataTypeList;
	Column temp;
	temp.InitionData("A", _eINT, 0, DataAttribute(_eNOT_NULL | _ePRIMARY_KEY | _eAUTO_INCREMENT | _eUNSIGNED), "");
	dataTypeList.push_back(temp);
	temp.InitionData("B", _eTINYBLOB, 0, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("C", _eTEXT, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("D", _eVARCHAR, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("E", _eCHAR, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("F", _eMEDIUMTEXT, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("G", _eLONGTEXT, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("H", _eDATE, 3, _eNULL, "");
	dataTypeList.push_back(temp);
	temp.InitionData("I", _eDATETIME, 3, _eNULL, "");
	dataTypeList.push_back(temp);

	UseDatabases("db_name");
	GetTableList(Datalist);
	vector<Column> columnTypeList;
	DeleteTable("testtable1");
	CreatTable("testtable1", dataTypeList);
	GetTableColumns("testtable1", columnTypeList);
	GetTableList(Datalist);

	CreatDataBases("heweichong");
	DeleteDataBases("heweichong");
	mysql_close(&mydata);//关闭mysql数据库

	exit(0);
	return true;
}

bool MySQLInterFace::CreatDataBases(string _Databases)
{
	string sql = "CREATE DATABASE ";
	sql = sql + _Databases + " DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "mysql_query() create table failed";
		return false;
	}
	return true;
}
bool MySQLInterFace::DeleteDataBases(string _Databases)
{
	string sql = "drop database ";
	sql = sql + _Databases + ";";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "mysql_query() create table failed";
		return false;
	}
	return true;
}
bool MySQLInterFace::GetDatabasesList(vector<string>& _DatabasesList)
{
	_DatabasesList.clear();
	string sql = "show databases;";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "show databases  failed";
		return false;
	}

	MYSQL_RES *result = NULL;
	result = mysql_store_result(&mydata);//获取结果
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数

	//取得并打印各字段的名称
	unsigned int fieldcount = mysql_num_fields(result);
	//打印各行
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while (NULL != row) {
		for (unsigned int i = 0; i < fieldcount; i++) {
			_DatabasesList.push_back(row[i]);
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果
	return true;
}
bool MySQLInterFace::UseDatabases(string _Databases)
{
	string sql = "use ";
	sql = sql + _Databases + ";";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "UseDatabases failed";
		return false;
	}
	return true;
}

bool MySQLInterFace::CreatTable(string _TableName, const vector<Column>& columnTypeList)
{
	if (columnTypeList.size() == 0)
	{
		_lastError = "dataTypeList is empty.";
		return false;
	}
	string sqlInstructions = "create table ";
	sqlInstructions = sqlInstructions + _TableName + "(";
	string keyName = "";
	for (unsigned int i = 0; i < columnTypeList.size();i++)
	{
		sqlInstructions = sqlInstructions + columnTypeList[i].Dataname + " "
			+ DateTypeToString(columnTypeList[i])
			//+ GetDateSize(columnTypeList[i].datasize)
			+ GetDateAttribute(columnTypeList[i].attribute) + columnTypeList[i].defData+",";
		if ((columnTypeList[i].attribute & _ePRIMARY_KEY) == _ePRIMARY_KEY)
		{
			if (keyName.size() != 0)
			{
				_lastError == "there is too many Key.";
				return false;
			}
			keyName = columnTypeList[i].Dataname;
		}
	}
	if (keyName.size() == 0)
	{
		_lastError == "No Key Here.";
		return false;
	}
	sqlInstructions = sqlInstructions + "PRIMARY KEY(" + keyName + "))ENGINE = InnoDB DEFAULT CHARSET = utf8;";
	if (0 != mysql_query(&mydata, sqlInstructions.c_str())){
		LOG(ERROR) <<"Error: " <<sqlInstructions;
		_lastError = "create table failed";
		exit(-1);
		return false;
	}
	string testt = "ENGINE = InnoDB DEFAULT CHARSET = utf8;";

	return true;

}

std::string MySQLInterFace::DateTypeToString(const Column& column)
{
	map<DataType, string> IndexName = {
		{ _eUNDEFTYPE, "" },
		{ _eTINYINT, "TINYINT" },
		{ _eSMALLINT, "SMALLINT" },
		{ _eMEDIUMINT, "MEDIUMINT" },
		{ _eINT, "INT" },
		{ _eBIGINT, "BIGINT" },
		{ _eFLOAT, "FLOAT" },
		{ _eDOUBLE, "DOUBLE" },
		{ _eDECIMAL, "DECIMAL" },
		{ _eDATE, "DATE" },
		{ _eTIME, "TIME" },
		{ _eYEAR, "YEAR" },
		{ _eDATETIME, "DATETIME" },
		{ _eTIMESTAMP, "TIMESTAMP" },
		{ _eCHAR, "CHAR" },
		{ _eVARCHAR, "VARCHAR" },
		{ _eTINYBLOB, "TINYBLOB" },
		{ _eTINYTEXT, "TINYTEXT" },
		{ _eBLOB, "BLOB" },
		{ _eTEXT, "TEXT" },
		{ _eMEDIUMBLOB, "MEDIUMBLOB" },
		{ _eMEDIUMTEXT, "MEDIUMTEXT" },
		{ _eLONGBLOB, "LONGBLOB" },
		{ _eLONGTEXT, "LONGTEXT" }
	};
	string strDataType = IndexName[column.datatype] + GetDateSize(column);
	int temp = column.attribute & _eUNSIGNED;
	if (temp != 0)
		strDataType += " UNSIGNED";
	temp = column.attribute & _eAUTO_INCREMENT;
	if (temp != 0)
		strDataType += " AUTO_INCREMENT";

	transform(strDataType.begin(), strDataType.end(), strDataType.begin(), ::tolower);
	return strDataType;
}

DataType MySQLInterFace::StringToDateType(const string& column)
{
	map<string,DataType> IndexName = {
		{ "", _eUNDEFTYPE},
		{ "tinyint", _eTINYINT },
		{ "smallint", _eSMALLINT },
		{ "mediumint", _eMEDIUMINT },
		{ "int", _eINT },
		{ "bigint", _eBIGINT },
		{ "float", _eFLOAT },
		{ "double", _eDOUBLE },
		{ "decimal", _eDECIMAL },
		{ "date", _eDATE },
		{ "time", _eTIME },
		{ "year", _eYEAR },
		{ "datetime", _eDATETIME },
		{ "timestamp", _eTIMESTAMP },
		{ "char", _eCHAR },
		{ "varchar", _eVARCHAR },
		{ "tinyblob", _eTINYBLOB },
		{ "tinytext", _eTINYTEXT },
		{ "bolb", _eBLOB },
		{ "text", _eTEXT },
		{ "mediumblob", _eMEDIUMBLOB },
		{ "mediumtext", _eMEDIUMTEXT },
		{ "longblob", _eLONGBLOB },
		{ "longtext", _eLONGTEXT }
	}; 
	string strType = column;
	if (strType.find("(") > 0 && strType.find("(") < strType.size())
	{
		strType = strType.substr(0, strType.find("("));
	}
	DataType temp;
	temp = IndexName[strType];
	return temp;
}



std::string MySQLInterFace::GetDateAttribute(DataAttribute Attribute)
{
	map<DataAttribute, string> IndexName = {
		{ _eEMPTY, "" },
		{ _eNULL, "" },
		{ _eNOT_NULL, "NOT NULL" },
		{ _eDEFAULT, "DEFAULT"},
		{ _ePRIMARY_KEY, "PRIMARY_KEY"},
		{ _eCHARACTER_SET, "CHARACTER_SET"}
	};
	
	return
		IndexName[DataAttribute(Attribute & _eNULL)] + " " +
		IndexName[DataAttribute(Attribute & _eNOT_NULL)] + " " +
		IndexName[DataAttribute(Attribute & _eDEFAULT)] + " " + 
		IndexName[DataAttribute(Attribute & _eCHARACTER_SET)] ;
}

bool MySQLInterFace::DeleteTable(string _TableName)
{
	string sql = "drop table ";
	sql = sql + _TableName + ";";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "drop table failed";
		return false;
	}
	return true;
}

bool MySQLInterFace::DeleteTableData(string _TableName)
{
	string sql = "delete from ";
	sql = sql + _TableName + ";";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "delete from failed";
		return false;
	}
	return true;

}

bool MySQLInterFace::GetTableList(vector<string>& _Tables)
{
	_Tables.clear();
	string sql = "show tables;";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "show databases  failed";
		return false;
	}

	MYSQL_RES *result = NULL;
	result = mysql_store_result(&mydata);//获取结果
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数

	//取得并打印各字段的名称
	unsigned int fieldcount = mysql_num_fields(result);
	//打印各行
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while (NULL != row) {
		for (unsigned int i = 0; i < fieldcount; i++) {
			_Tables.push_back(row[i]);
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果
	return true;
}

std::string MySQLInterFace::GetDateSize(const Column& column)
{
	string strdateSize = "";
	ostringstream  dataoss;
	//只有在char和varchar的情况下，长度才有意义
	if (column.datasize >= 0 && (column.datatype == _eCHAR || column.datatype == _eVARCHAR))
	{
		dataoss << "(" << column.datasize << ")";
		strdateSize = dataoss.str();
	}
	return strdateSize;
}

int MySQLInterFace::StringToDateSize(const string& datasize)
{
	string strSize = "";
	if (
		datasize.find("(") > 0 &&
		datasize.find(")") > 0 &&
		datasize.find("(") < datasize.find(")")&&
		datasize.find(")") < datasize.size())
	{
		unsigned int lasterIndex = datasize.find("(");
		unsigned int frontIndex = datasize.find(")");
		strSize = datasize.substr(lasterIndex+1, frontIndex - lasterIndex-1);
		return atoi(strSize.c_str());
	}
	return 0;
}

bool MySQLInterFace::GetTableColumns(string _TableName, vector<Column>& columnTypeList)
{
	columnTypeList.clear();
	string sql = "show columns from " + _TableName;
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "show databases  failed";
		return false;
	}
	MYSQL_RES *result = NULL;
	result = mysql_store_result(&mydata);//获取结果
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数

	unsigned int fieldcount = mysql_num_fields(result);
	if (fieldcount != 6)
	{
		LOG(ERROR) << "fieldcount  is not equal to 6.";
		return false;
	}
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	vector<vector<string>> _Tables;
	vector<string> _oneListTable;
	while( NULL != row ) {
		_oneListTable.clear();
		for (unsigned int i = 0; i < fieldcount; i++){
			if (row[i] == NULL){
				_oneListTable.push_back("NULL");
				continue;
			}
			_oneListTable.push_back(row[i]);
		}
		columnTypeList.push_back(ExplaincolumnType(_oneListTable));
		row = mysql_fetch_row(result);
	}
	return true;
}

bool MySQLInterFace::StringToCloumn(const vector<string>& _columnTables, Column& column)
{
	if (_columnTables.size() != 6)
	{
		return false;
	}
	column.Dataname = _columnTables[0];
	if (_columnTables[2] == "YES")
		column.attribute = (DataAttribute)(column.attribute | _eNULL);
	if (_columnTables[2] == "NO")
		column.attribute = (DataAttribute)(column.attribute | _eNOT_NULL);
	// 	column.datatype = 
	return true;
}

Column MySQLInterFace::ExplaincolumnType(const vector<string>& columnstringList)
{
	Column dataColumns;
	if (columnstringList.size() != 6)
	{
		LOG(ERROR) << "Error: columnstringList size  is not equal to 6.";
		return dataColumns;
	}
	dataColumns.Dataname = columnstringList[0];//获得数据名
	dataColumns.datatype = StringToDateType(columnstringList[1]);//获得数据的类型
	dataColumns.datasize = StringToDateSize(columnstringList[1]);//获得数据的长度
	unsigned int index = columnstringList[1].find("unsigned");
	if (	columnstringList[1].find("unsigned")  > 0&&
		columnstringList[1].find("unsigned") < columnstringList[1].size())//判断是否为unsigned类型
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _eUNSIGNED);
	else
		dataColumns.attribute = DataAttribute(dataColumns.attribute & (~_eUNSIGNED));
	if (columnstringList[2] == "YES")//判断是否可以为NULL
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _eNULL & (~_eNOT_NULL));
	else if (columnstringList[2] == "NO")
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _eNOT_NULL & (~_eNULL));

	if (columnstringList[3] == "PRI" || columnstringList[3] == "pri")//判断是否为 Key
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _ePRIMARY_KEY);
	else
		dataColumns.attribute = DataAttribute(dataColumns.attribute & (~_ePRIMARY_KEY));

	dataColumns.defData = columnstringList[4];//获得默认数据
	if (columnstringList[5].find("auto_increment") > 0 &&
		columnstringList[5].find("auto_increment") < columnstringList[5].size())//判断是否可以自动增长
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _eAUTO_INCREMENT);
	else
		dataColumns.attribute = DataAttribute(dataColumns.attribute & (~_eAUTO_INCREMENT));

	return dataColumns;
}







void Column::InitionData(
	const string& _Dataname,
	const DataType& _datatype,
	const int& _datasize,
	const DataAttribute& _attribute,
	const string& _defData)
{
	this->Dataname = _Dataname;
	this->datatype = _datatype;
	this->datasize = _datasize;
	this->attribute = _attribute;
	this->defData = _defData;
	return;
}
