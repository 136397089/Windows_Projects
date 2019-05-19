#include "stdafx.h"
#include <string>
#include <map>
#include <algorithm>
#include "MySQLInterFace.h"
#include "glog/logging.h"
#pragma comment(lib, "libmysql.lib")


#define CheckSqlBase(a)if (NULL == a){_lastError = "mysql_init()failed";return false;}

MySQLInterFace::MySQLInterFace()
	:dataSql(NULL)
{
}
MySQLInterFace::~MySQLInterFace()
{
	if (dataSql != NULL)
	{
		mysql_close(&sqlDataBase);//关闭mysql数据库
		dataSql = NULL;
	}
}

bool MySQLInterFace::Inition()
{
	if (NULL == dataSql){
		dataSql = mysql_init(&sqlDataBase);
	}
	if (NULL == dataSql){
		_lastError = "mysql_init()failed";
		return false;
	}
	if (0 != mysql_library_init(0, NULL, NULL)){
		_lastError = "mysql_library_init()failed";
		return false;
	}
	if (0 != mysql_options(&sqlDataBase, MYSQL_SET_CHARSET_NAME, "gbk")){
		_lastError = "mysql_options() failed";
		return false;
	}
	//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
	if (NULL == mysql_real_connect(&sqlDataBase, "localhost", "root", "", "", 3306, NULL, 0)){
		_lastError = "mysql_real_connect() failed" ;
		return false;
	}

	return true;
}

bool MySQLInterFace::CreatDataBases(const string& _Databases)
{
	string sqlInstru = "CREATE DATABASE ";
	sqlInstru = sqlInstru + _Databases + " DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "mysql_query() create table failed";
		return false;
	}
	return true;
}
bool MySQLInterFace::DeleteDataBases(const string& _Databases)
{
	string sqlInstru = "drop database ";
	sqlInstru = sqlInstru + _Databases + ";";

	if (!RunCommand(sqlInstru))
	{
		_lastError = "DeleteDataBases failed";
		return false;
	}
	return true;

}
bool MySQLInterFace::GetDatabasesList(vector<string>& _DatabasesList)
{
	_DatabasesList.clear();
	string sqlInstru = "show databases;";
	
	if (!RunCommand(sqlInstru))
	{
		_lastError = "show databases  failed";
		return false;
	}
	GetListResult(_DatabasesList);
	return true;
}
bool MySQLInterFace::UseDatabases(const string& _Databases)
{
	string sqlInstru = "use ";
	sqlInstru = sqlInstru + _Databases + ";";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "UseDatabases failed";
		return false;
	}

	return true;
}

bool MySQLInterFace::CreatTable(const string& _TableName, const vector<Column>& columnTypeList)
{
	if (columnTypeList.size() == 0)
	{
		_lastError = "dataTypeList is empty.";
		return false;
	}
	string sqlInstru = "create table ";
	sqlInstru = sqlInstru + _TableName + "(";
	string keyName = "";
	for (unsigned int i = 0; i < columnTypeList.size();i++)
	{
		sqlInstru = sqlInstru + columnTypeList[i].Dataname + " "
			+ DateTypeToString(columnTypeList[i])
			//+ GetDateSize(columnTypeList[i].datasize)
			+ DateAttributeToString(columnTypeList[i].attribute) + columnTypeList[i].remarkData+",";
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
	sqlInstru = sqlInstru + "PRIMARY KEY(" + keyName + "))ENGINE = InnoDB DEFAULT CHARSET = utf8;";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) <<"Error: " <<sqlInstru;
		_lastError = "create table failed";
		return false;
	}
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
	//转为小写字母
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

std::string MySQLInterFace::DateAttributeToString(DataAttribute Attribute)
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

bool MySQLInterFace::DeleteTable(const string& _TableName)
{
	CheckSqlBase(dataSql);
	string sqlInstru = "drop table ";
	sqlInstru = sqlInstru + _TableName + ";";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "drop table failed";
		return false;
	}
	DeleteTableMap(_TableName);
	return true;
}

bool MySQLInterFace::DeleteTableData(const string& _TableName)
{
	string sql = "delete from ";
	sql = sql + _TableName + ";";
	if (!RunCommand(sql))
	{
		_lastError = "delete from failed";
		return false;
	}
	return true;
}

bool MySQLInterFace::GetTableList(vector<string>& _Tables)
{
	_Tables.clear();
	string sqlInstru = "show tables;";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "show databases  failed";
		return false;
	}

	GetListResult(_Tables);

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

bool MySQLInterFace::GetTableColumns(const string& _TableName, vector<Column>& columnTypeList)
{
	columnTypeList.clear();
	string sqlInstru = "show columns from " + _TableName + ";";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "show databases  failed";
		return false;
	}
// 	vector<vector<string>> _TablesResult;
// 	GetListResult(_TablesResult);
	MYSQL_RES *result = NULL;
	result = mysql_store_result(&sqlDataBase);//获取结果
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数
	unsigned int fieldcount = mysql_num_fields(result);//获得结果列数
	MYSQL_ROW row = mysql_fetch_row(result);

	if (fieldcount != 6)
	{
		LOG(ERROR) << "fieldcount  is not equal to 6.";
		mysql_free_result(result);//释放结果
		return false;
	}

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
	mysql_free_result(result);//释放结果
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

	dataColumns.remarkData = columnstringList[4];//获得默认数据
	if (columnstringList[5].find("auto_increment") > 0 &&
		columnstringList[5].find("auto_increment") < columnstringList[5].size())//判断是否可以自动增长
		dataColumns.attribute = DataAttribute(dataColumns.attribute | _eAUTO_INCREMENT);
	else
		dataColumns.attribute = DataAttribute(dataColumns.attribute & (~_eAUTO_INCREMENT));

	return dataColumns;
}
//保存数据到数据库之前调用，将特定类型的数据转为字符串
string MySQLInterFace::DataConversion(const Column& columnName, const string& _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eCHAR
		|| columnName.datatype == _eVARCHAR
		|| columnName.datatype == _eTEXT
		|| columnName.datatype == _eLONGTEXT
		|| columnName.datatype == _eMEDIUMTEXT
		)
	{
		vdataList.push_back(_data);
		return _data;
	}
	return "";
}
string MySQLInterFace::DataConversion(const Column& columnName, const int _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eINT
		|| columnName.datatype == _eTINYINT
		|| columnName.datatype == _eSMALLINT
		|| columnName.datatype == _eMEDIUMINT
		){
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";
}
string MySQLInterFace::DataConversion(const Column& columnName, const unsigned int _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eINT
		|| columnName.datatype == _eTINYINT
		|| columnName.datatype == _eSMALLINT
		|| columnName.datatype == _eMEDIUMINT
		){
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";
}
string MySQLInterFace::DataConversion(const Column& columnName, const long _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eBIGINT
		){
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";
}
string MySQLInterFace::DataConversion(const Column& columnName, const short _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eSMALLINT
		|| columnName.datatype == _eTINYINT
		){
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";

}
string MySQLInterFace::DataConversion(const Column& columnName, const CDate& _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eDATE)
	{
		ostringstream  oss;
		oss << "'" <<_data._year << "-" << _data._month << "-" << _data._day<<"'";
		string tempdate = oss.str();
		vdataList.push_back(tempdate);
		return oss.str();
	}
	if (columnName.datatype == _eTIME)
	{
		ostringstream  oss;
		oss << "'" << _data._hour << ":" << _data._minute << ":" << _data._second << "'";
		vdataList.push_back(oss.str());
		return oss.str();
	}
	if (columnName.datatype == _eDATETIME || columnName.datatype == _eTIMESTAMP)
	{
		ostringstream  oss;
		oss << "'" 
			<< _data._year << "-"
			<< _data._month << "-" << _data._day 
			<< " "<< _data._hour 
			<< ":" << _data._minute 
			<< ":" << _data._second
			<< "'";
		vdataList.push_back(oss.str());
		return oss.str();
	}
	if (columnName.datatype == _eYEAR)
	{
		ostringstream  oss;
		oss << "'" << _data._year << "'";
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";

}
string MySQLInterFace::DataConversion(const Column& columnName, const float _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eFLOAT)
	{
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";
}
string MySQLInterFace::DataConversion(const Column& columnName, const double _data, vector<string>& vdataList) const
{
	if (columnName.datatype == _eDOUBLE
		|| columnName.datatype == _eDECIMAL
		)
	{
		ostringstream  oss;
		oss << _data;
		vdataList.push_back(oss.str());
		return oss.str();
	}
	return "";
}

bool MySQLInterFace::InsertData(
	string _TableName,
	const vector<Column>& columnTypeList,
	const vector<string>& datalist)
{
	if (datalist.size() == 0 || columnTypeList.size() != datalist.size())
	{
		_lastError = "Data size error.";
		return false;
	}
	string sqlInstru = "INSERT INTO ";
	sqlInstru = sqlInstru + _TableName + "(" ;
	sqlInstru += columnTypeList[0].Dataname;
	for (unsigned int i = 1; i < columnTypeList.size(); i++)
	{
		sqlInstru = sqlInstru + "," + columnTypeList[i].Dataname ;
	}
	sqlInstru += ") VALUES(" + datalist[0];
	for (unsigned int i = 1; i < datalist.size(); i++)
	{
		sqlInstru = sqlInstru + "," + datalist[i];
	}
	sqlInstru += ");";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Insert data failed";
		return false;
	}

	return true;
}

bool MySQLInterFace::SearchDataFromTable(
	const string _TableName,
	const vector<Column>& columnTypeList,
	vector<vector<string>>& _returnDatas)
{
	string sqlInstru = "";
	if (columnTypeList.size() == 0)
	{
		sqlInstru = "select * from " + _TableName + ";";
	}
	else
	{
		sqlInstru = "select " + columnTypeList[0].Dataname;
		for (unsigned int i = 1; i < columnTypeList.size();i++)
		{
			sqlInstru = sqlInstru + "," + columnTypeList[i].Dataname;
		}
		sqlInstru = sqlInstru + " from " + _TableName + ";";
	}
	if (!RunCommand(sqlInstru))
	{
		_lastError = "GetDataFromTable failed";
		return false;
	}
	GetTableResult(_returnDatas);
	return true;
}
bool MySQLInterFace::SearchDataFromTable(
	const string _TableName,
	const vector<Column>& columnTypeList,
	const vector<WhereCommand>& whereList,
	vector<vector<string>>& _ReturnDatas)
{
	_ReturnDatas.clear();
	string sqlInstru = "";
	if (columnTypeList.size() == 0)
	{
		sqlInstru = "select * from " + _TableName;
	}
	else
	{
		sqlInstru = "select " + columnTypeList[0].Dataname;
		for (unsigned int i = 1; i < columnTypeList.size(); i++)
		{
			sqlInstru = sqlInstru + "," + columnTypeList[i].Dataname;
		}
		sqlInstru = sqlInstru + " from " + _TableName;
	}
	for (unsigned int i = 0; i < whereList.size();i++)
	{
		if (i == 0)
			sqlInstru = sqlInstru +" where " +whereList[i].GetCommand(_eWhereEmpty);
		else
			sqlInstru = sqlInstru + whereList[i].GetCommand(whereList[i].relation);
	}
	sqlInstru += ";";
	if (!RunCommand(sqlInstru))
	{
		_lastError = "GetDataFromTable failed";
		return false;
	}

	GetTableResult(_ReturnDatas);
	return true;
}



void Column::InitionData(
	const string& _Dataname,
	const DataType& _datatype,
	const int& _datasize,
	const DataAttribute& _attribute,
	const string& _remarkData)
{
	this->Dataname = _Dataname;
	this->datatype = _datatype;
	this->datasize = _datasize;
	this->attribute = _attribute;
	this->remarkData = _remarkData;
	return;
}


bool MySQLInterFace::UpDateData(
	string _TableName,
	const vector<Column>& columnTypeList,
	const vector<string>& dataList,
	const vector<WhereCommand>& commandList
	)
{
	if (dataList.size() == 0 || columnTypeList.size() != dataList.size())
	{
		_lastError = "Data size error.";
		return false;
	}
	string sqlInstru = "UPDATE " + _TableName + " SET " + columnTypeList[0].Dataname + "=" + dataList[0];
	for (unsigned int i = 1; i < columnTypeList.size();i++)
		sqlInstru = sqlInstru + "," + columnTypeList[i].Dataname + "=" +dataList[i];
	if (!commandList.empty())
	{
		sqlInstru = sqlInstru + " where " + commandList[0].GetCommand(_eWhereEmpty);
		for (unsigned int i = 1; i < commandList.size();i++)
		{
			sqlInstru = sqlInstru + commandList[i].GetCommand(commandList[i].relation)+" ";
		}
	}
	sqlInstru += ";";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Insert data failed";
		return false;
	}

	return true;
}

bool MySQLInterFace::CheckTableExists(string _TableName)
{
	if (CheckTableMap(_TableName))
	{
		LOG(INFO) << "Find table:"<<_TableName;
		return true;
	}
	transform(_TableName.begin(), _TableName.end(), _TableName.begin(), ::tolower);
	string sqlIns = "show tables like '" + _TableName + "';";
	if (!RunCommand(sqlIns))
	{
		LOG(ERROR) << "error:CheckTableExists Run command error.";
		return false;
	}
	vector<vector<string>> resultTable;
	if (!GetTableResult(resultTable))
	{
		LOG(ERROR) << "error:CheckTableExists get result error.";
		return false;
	}
	if (resultTable.size() > 1)
	{
		LOG(INFO) << "Find table " << _TableName<<" by SQL commond.";
		SetTableMap(_TableName);
		return true;
	}

	return false;
}

bool MySQLInterFace::CheckTableColumnsExists(const string& _TableName, const vector<Column>& _columnsName)
{
	vector<Column> _Columns;
	if (!GetTableColumns(_TableName, _Columns))
		return false;
	//map<string, int> columnsMapRead;
	for (unsigned int i = 0; i < _Columns.size(); i++)
		SetTableColMap(_TableName, _Columns[i].Dataname);

	for (unsigned int i = 0; i < _columnsName.size(); i++)
	{
		if (!CheckTableColMap(_TableName, _Columns[i].Dataname))
			return false;
	}
	return true;
}

bool MySQLInterFace::AddColumnToTable(const string& _TableName, const vector<Column>& _columnsName)
{
	if (!CheckTableExists(_TableName))
	{
		_lastError = "Table is non-existent.";
		return false;
	}
	vector<Column> _Columns;
	GetTableColumns(_TableName, _Columns);
	map<string, int> columnsMapRead;
	for (unsigned int i = 0; i < _Columns.size(); i++)
	{
		columnsMapRead[_Columns[i].Dataname] = 1;
	}
	string sql = "";
	for (unsigned int i = 0; i < _columnsName.size(); i++)
	{
		if (columnsMapRead[_columnsName[i].Dataname] != 1)
		{
			AddOneColumns(_TableName, _columnsName[i]);
		}
	}
	return true;
}

bool MySQLInterFace::AddOneColumns(const string& _TableName, const Column& _columnsName)
{
	string sqlInstru = "";
	sqlInstru = "Alter table "
		+ _TableName
		+ " add column "
		+ _columnsName.Dataname + " "
		+ DateTypeToString(_columnsName) + " "
		+ DateAttributeToString(_columnsName.attribute) + " "
		+ _columnsName.remarkData + ";";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Add columns failed.";
		return false;
	}
	return true;
}

bool MySQLInterFace::DeleteColumn(const string& _TableName, const string& columnName)
{
	string sqlInstru = "";
	sqlInstru = sqlInstru + "alter table " + _TableName + " drop column " + columnName;
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Deleta Column failed.";
		return false;
	}
	DeleteTableColMap(_TableName, columnName);
	return true;
}

bool MySQLInterFace::CheckDataExists(const string _TableName, const vector<WhereCommand>& whereList)
{
	vector<Column> columnTypeList;
	vector<vector<string>> _datas;
	SearchDataFromTable(_TableName, columnTypeList, whereList, _datas);
	if (_datas.size() <= 1)
	{
		return false;
	}
	return true;
}

bool MySQLInterFace::RunCommand(const string& command)
{
	CheckSqlBase(dataSql);
	int returnCode;
// 	returnCode = mysql_query(&sqlDataBase, ";");
// 	if (returnCode != 0)
// 		Inition();
	returnCode= mysql_query(&sqlDataBase, command.c_str());
	if (returnCode != 0)
	{
		LOG(ERROR) <<"Run Commend Error: "<< command << endl;
		return false;
	}
	LOG(INFO) << "Run Commend Success: " << command << endl;
	return true;
}

bool MySQLInterFace::GetListResult(vector<string>& _ListData)
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(&sqlDataBase);//获取结果
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数
	//取得并打印各字段的名称
	unsigned int fieldcount = mysql_num_fields(result);
	//打印各行
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while (NULL != row) {
		for (unsigned int i = 0; i < fieldcount; i++) {
			_ListData.push_back(row[i]);
		}
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果
	return true;
}
bool MySQLInterFace::GetTableResult(vector<vector<string>>& _Tables)
{
	_Tables.clear();
	MYSQL_RES *result = NULL;
	result = mysql_store_result(&sqlDataBase);//获取结果
	if (NULL == result)
	{
		mysql_free_result(result);//释放结果
		return false;
	}
	vector<string> _oneListTable; //用于暂存一行数据
	my_ulonglong rowcount = mysql_num_rows(result);//获得结果行数
	unsigned int fieldcount = mysql_num_fields(result);//获得结果列数

	//获得列名
	MYSQL_FIELD *fd = nullptr;
	for (unsigned int i = 0; i < fieldcount; i++)
	{
		fd = mysql_fetch_field(result);//获取列名
		_oneListTable.push_back(fd->name);
	}
	_Tables.push_back(_oneListTable);
	_oneListTable.clear();
	//获得每一行数据
	MYSQL_ROW row = mysql_fetch_row(result);//读取一行结果
	while (NULL != row)
	{
		_oneListTable.clear();
		for (unsigned int i = 0; i < fieldcount; i++)
		{
			if (row[i] == NULL)
			{
				_oneListTable.push_back("NULL");
				continue;
			}
			_oneListTable.push_back(row[i]);
		}
		_Tables.push_back(_oneListTable);
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);//释放结果
	return true;
}

bool MySQLInterFace::SetColumnUnique(const string& _TableName, const string& _ColumnName)
{
	string sqlInstru = "";
	sqlInstru = "Alter table "
		+ _TableName
		+ " add unique ("
		+ _ColumnName + ");";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Set ColumnUnique failed.";
		return false;
	}
	return true;
}

bool MySQLInterFace::SetColumnCombinUnique(const string& _TableName, const vector<string>& _ColumnName)
{
	if (_ColumnName.size() == 0)
	{
		LOG(ERROR) << "Function SetColumnCombinUnique:_Columns size error.";
		return false;
	}
	string sqlInstru = "";
	sqlInstru = "Alter table " + _TableName + " add unique index(" + _ColumnName[0];
	for (unsigned int i = 1; i < _ColumnName.size();i++)
	{
		sqlInstru += "," + _ColumnName[i];
	}
	sqlInstru = sqlInstru + ");";
	if (!RunCommand(sqlInstru))
	{
		LOG(ERROR) << "Error: " << sqlInstru;
		_lastError = "Set Column CombinUnique failed.";
		return false;
	}

	return true;
}

bool MySQLInterFace::CheckTableColMap(const string& table, const string& column)
{
	return indexmap._TableColMap[table + "_" + column] == 1;
}
void MySQLInterFace::SetTableColMap(const string& table, const string& column)
{
	indexmap._TableColMap[table + "_" + column] = 1;
}
void MySQLInterFace::DeleteTableColMap(const string& table, const string& column)
{
	indexmap._TableColMap[table + "_" + column] = 0;
}
bool MySQLInterFace::CheckTableMap(const string& table)
{
	return indexmap._TableMap[table] == 1;
}
void MySQLInterFace::SetTableMap(const string& table)
{
	indexmap._TableMap[table] = 1;
}
void MySQLInterFace::DeleteTableMap(const string& table)
{
	indexmap._TableMap[table] = 0;
}

WhereCommand::WhereCommand()
	:dataName(),
	commandType(_eIs),
	data("")
{
}
WhereCommand::~WhereCommand()
{}
std::string WhereCommand::GetCommand(RelationalType relation) const
{
	map < WhereType, string > commTypeMap=
	{{_eLess, "<"},{_eBigger, ">"},{_eIs, "="},{_eNot, "<>"},
	{_eLessOrEqual, "<="},{_eBiggerOrEqual, ">="},{_eLike," LIKE "}};
	map < RelationalType, string > relaMap = 
	{{_eAnd, " AND "}, { _eOr, " OR " }, { _eWhereEmpty, " " }};

	string  sql = "";
	sql += relaMap[relation];
	sql += dataName + commTypeMap[commandType] + data;
	return sql;
}
