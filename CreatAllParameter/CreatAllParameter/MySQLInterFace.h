#pragma once

#ifndef MYSQLINTERFACE_H
#define MYSQLINTERFACE_H

#include <vector>
#include <string>
#include "DateTool.h"
#include "D:\MySql\mysql-5.7.25-winx64\include\mysql.h"

using namespace std;


enum DataType
{
	_eTINYINT,
	_eSMALLINT,
	_eMEDIUMINT,
	_eINT,
	_eBIGINT,
	_eFLOAT,
	_eDOUBLE,
	_eDECIMAL,
	_eDATE,
	_eTIME,
	_eYEAR,
	_eDATETIME,
	_eTIMESTAMP,
	_eCHAR,
	_eVARCHAR,
	_eTINYBLOB,
	_eTINYTEXT,
	_eBLOB,
	_eTEXT,
	_eMEDIUMBLOB,
	_eMEDIUMTEXT,
	_eLONGBLOB,
	_eLONGTEXT,
	_eUNDEFTYPE
};
enum DataAttribute
{
	_eEMPTY = 0,
	_eNOT_NULL = 1<<0,
	_eDEFAULT = 1 <<1,
	_ePRIMARY_KEY = 1 <<2,
	_eAUTO_INCREMENT = 1 <<3,
	_eUNSIGNED = 1 <<4,
	_eCHARACTER_SET = 1 <<5,
	_eNULL = 1 << 6,
};

struct Column
{
	string Dataname;
	DataType datatype;
	int datasize;//如果是字符串，通过datasize指定数据的长度
	DataAttribute attribute;
	string remarkData;//用于记录
	Column() :
		attribute(_eEMPTY), 
		Dataname(""), 
		datatype(_eINT), 
		datasize(0), 
		remarkData(""){}
	void InitionData(
		const string& _Dataname,
		const DataType& _datatype,
		const int& _datasize, 
		const DataAttribute& _attribute,
		const string& _remarkData);
};
enum WhereType
{
	_eLess,
	_eBigger,
	_eIs,
	_eNot,
	_eLessOrEqual,
	_eBiggerOrEqual,
	_eLike
};
enum RelationalType
{
	_eAnd,
	_eOr,
	_eWhereEmpty
};
struct WhereCommand
{
	string dataName;
	WhereType commandType;
	RelationalType relation;
	string data;
	template<class T>
	WhereCommand IniCommand(const string& _strName, WhereType _tyCommand, const T& _tdata, RelationalType _relation);
	string GetCommand(RelationalType relation) const;
	WhereCommand();
	~WhereCommand();
};
class MySQLInterFace
{
public:
	MySQLInterFace();
	~MySQLInterFace();
	//
	bool Inition();
	//创建一个数据库
	bool CreatDataBases(const string& _Databases);
	//删除一个数据库
	bool DeleteDataBases(const string& _Databases);
	//获得所有数据库名称
	bool GetDatabasesList(vector<string>& _Databases);
	//进入数据库
	bool UseDatabases(const string& _Databases);
	//创建一个表
	bool CreatTable(const string& _TableName,const vector<Column>& columnTypeList);
	//删除一个表
	bool DeleteTable(const string& _TableName);
	//删除表里面的数据
	bool DeleteTableData(const string& _TableName);
	//获得所有表名称
	bool GetTableList(vector<string>& _Tables);
	//检查表是否存在
	bool CheckTableExists(string _TableName);
	//获得表的表头
	bool GetTableColumns(const string& _TableName, vector<Column>& columnTypeList);
	//检查表头是否都存在
	bool CheckTableColumnsExists(const string& _TableName, const vector<Column>& _columnsName);
	//将表头增加到对应的表当中
	bool AddColumnToTable(const string& _TableName,const vector<Column>& _columnsName);
	//
	bool DeleteColumn(const string& _TableName, const string& columnName);
	//保存数据到数据库
	bool InsertData(string _TableName,const vector<Column>& columnTypeList,const vector<string>& dataList);
	//更新表数据
	bool UpDateData(
		string _TableName,
		const vector<Column>& columnTypeList,
		const vector<string>& dataList,
		const vector<WhereCommand>& commandList
/*		const string& whereLimit*/);
	//从表中读取数据
	bool ReadDataFromTable(
		const string _TableName,
		const vector<Column>& columnTypeList,
		vector<vector<string>>& _returndatas);
	//从表中读取数据
	bool ReadDataFromTable(
		const string _TableName,
		const vector<Column>& columnTypeList,
		const vector<WhereCommand>& whereList,
		vector<vector<string>>& _returndatas);
	//检查是否有存在对应的数据
	bool CheckDataExists(const string _TableName, const vector<WhereCommand>& whereList);
	//
	bool SetColumnPrimaryKey(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnUnique(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnIndex(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnCombinUnique(const string& _TableName, const vector<string>& _ColumnName);
	//保存数据到数据库之前调用，将特定类型的数据转为字符串
	string DataConversion(const Column& columnName, const string& _data) const;
	string DataConversion(const Column& columnName, const int _data) const;
	string DataConversion(const Column& columnName, const unsigned int _data) const;
	string DataConversion(const Column& columnName, const long _data) const;
	string DataConversion(const Column& columnName, const short _data) const;
	string DataConversion(const Column& columnName, const CDate& _data) const;
	string DataConversion(const Column& columnName, const float _data) const;
	string DataConversion(const Column& columnName, const double _data) const;

	//
	bool RunCommand(const string& command);
	//
	bool GetTableResult(vector<vector<string>>& _Tables);
	//
private:
	bool GetListResult(vector<string>& _ListData);
// 	bool StringToCloumn(const vector<string>& _Tables, Column& column);
	//将DataType类型的数据类型转为字符串形式
	string DateTypeToString(const Column& column);
	//将查询到的字符串形式的数据类型转为DataType
	DataType StringToDateType(const string& column);
	//将数据属性转为命令字符串
	string DateAttributeToString(DataAttribute Attribute);
	//获得数据长度的字符串
	string GetDateSize(const Column& column);
	//
	int StringToDateSize(const string& datasize);
	//将字符串转为数据类型
	Column ExplaincolumnType(const vector<string>& columnstringList);
	//
	bool AddOneColumns(const string& _TableName, const Column& _columnsName);
	string _lastError;
	MYSQL sqlDataBase;
	MYSQL* dataSql;

	bool CheckTableColMap(const string& table, const string& column);
	void SetTableColMap(const string& table, const string& column);
	void DeleteTableColMap(const string& table, const string& column);
	bool CheckTableMap(const string& table);
	void SetTableMap(const string& table);
	void DeleteTableMap(const string& table);

	struct IndexMaps
	{
		map<string, int> _TableMap;//用于记录之前已经查到的表，减少发送指令的次数
		map<string, int> _TableColMap;//用于记录之前已经查到的表头，减少发送指令的次数
	} indexmap;
};
#define SZCHARSIZE 100
struct _tagMysqlInfo
{
	char host[SZCHARSIZE];    //主机
	char user[SZCHARSIZE];			//用户名
	char passwd[SZCHARSIZE];    //密码
	char db[SZCHARSIZE];		//数据库
	UINT port;				//端口号
	char unix_socket[SZCHARSIZE];    //如果unix_socket不是NULL，该字符串描述了应使用的套接字或命名管道。注意，“host”参数决定了连接的类型。
	DWORD client_flag;    // client_flag的值通常为0
};


// struct LikeCommand
// {
// 
// };

template<class T>
WhereCommand WhereCommand::IniCommand(const string& _strName, WhereType _tyCommand, const T& _tdata, RelationalType _relation)
{
	dataName = _strName;
	commandType = _tyCommand;
	relation = _relation;
	ostringstream  oss;
	oss << _tdata;
	data = oss.str();
	return *this;
}





#endif