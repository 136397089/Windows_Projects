#include "stdafx.h"
#include <string>
#include <map>
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
	vector<TableData> dataTypeList;
	TableData temp;
	temp.Dataname = "2TT1";
	temp.datatype = _eINT;
	temp.attribute = DataAttribute(_eNOT_NULL|_ePRIMARY_KEY);
	temp.datasize = 0;
	dataTypeList.push_back(temp);
	temp.Dataname = "2BB2";
	temp.datatype = _eINT;
	temp.attribute = _eNOT_NULL;
	temp.datasize = 0;
	dataTypeList.push_back(temp);
	UseDatabases("db_name");
	GetTableList(Datalist);
	CreatTable("testtable",dataTypeList);
	GetTableList(Datalist);

	CreatDataBases("heweichong");
	DeleteDataBases("heweichong");
	mysql_close(&mydata);//关闭mysql数据库
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
	int rowcount = mysql_num_rows(result);//获得结果行数

	//取得并打印各字段的名称
	unsigned int fieldcount = mysql_num_fields(result);
	//打印各行
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while (NULL != row) {
		for (int i = 0; i < fieldcount; i++) {
			_DatabasesList.push_back(row[i]);
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果

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

bool MySQLInterFace::CreatTable(string _TableName, const vector<TableData>& dataTypeList)
{
	if (dataTypeList.size() == 0)
	{
		_lastError = "dataTypeList is empty.";
		return false;
	}
	string sql = "create table ";
	sql = sql + _TableName + "(";
	string keyName = "";
	for (unsigned int i = 0; i < dataTypeList.size();i++)
	{
		sql = sql + dataTypeList[i].Dataname + " "
			+ GetDateType(dataTypeList[i].datatype)
			+ GetDateAttribute(dataTypeList[i].attribute)+",";
		if ((dataTypeList[i].attribute & _ePRIMARY_KEY) == _ePRIMARY_KEY){
			if (keyName.size() != 0){
				_lastError == "there is too many Key.";
				return false;
			}
			keyName = dataTypeList[i].Dataname;
		}
	}
	if (keyName.size() == 0)
	{
		_lastError == "No Key Here.";
		return false;
	}
	sql = sql + "PRIMARY KEY(" + keyName + "))ENGINE = InnoDB DEFAULT CHARSET = utf8;";
	if (0 != mysql_query(&mydata, sql.c_str())){
		_lastError = "create table failed";
		return false;
	}
	string testt = "ENGINE = InnoDB DEFAULT CHARSET = utf8;";

	return true;

}

std::string MySQLInterFace::GetDateType(DataType datatype)
{
	map<DataType, string> IndexName = {
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
	return IndexName[datatype];
}

std::string MySQLInterFace::GetDateAttribute(DataAttribute Attribute)
{
	map<DataAttribute, string> IndexName = {
		{ _eEMPTY, "" },
		{ _eNULL, "NULL" },
		{ _eNOT_NULL, "NOT NULL" },
		{ _eDEFAULT, "DEFAULT"},
		{ _ePRIMARY_KEY, "PRIMARY_KEY"},
		{ _eAUTO_INCREMENT, "AUTO_INCREMENT"},
		{ _eUNSIGNED, "UNSIGNED"},
		{ _eCHARACTER_SET, "CHARACTER_SET"}
	};
	
	return
		IndexName[DataAttribute(Attribute & _eNULL)] + " " +
		IndexName[DataAttribute(Attribute & _eNOT_NULL)] + " " +
		IndexName[DataAttribute(Attribute & _eDEFAULT)] + " " +
		IndexName[DataAttribute(Attribute & _eAUTO_INCREMENT)] + " " +
		IndexName[DataAttribute(Attribute & _eUNSIGNED)] + " " +
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
	int rowcount = mysql_num_rows(result);//获得结果行数

	//取得并打印各字段的名称
	unsigned int fieldcount = mysql_num_fields(result);
	//打印各行
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while (NULL != row) {
		for (int i = 0; i < fieldcount; i++) {
			_Tables.push_back(row[i]);
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果

}




