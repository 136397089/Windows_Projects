#pragma once
#include <vector>
#include <string>
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
	string defData;
	Column() :
		attribute(_eEMPTY), 
		Dataname(""), 
		datatype(_eINT), 
		datasize(0), 
		defData(""){}
	void InitionData(
		const string& _Dataname,
		const DataType& _datatype,
		const int& _datasize, 
		const DataAttribute& _attribute,
		const string& _defData);
};
class MySQLInterFace
{
public:
	MySQLInterFace();
	~MySQLInterFace();

	bool Inition();
	//创建一个数据库
	bool CreatDataBases(string _Databases);
	//删除一个数据库
	bool DeleteDataBases(string _Databases);
	//获得所有数据库名称
	bool GetDatabasesList(vector<string>& _Databases);
	//进入数据库
	bool UseDatabases(string _Databases);
	//创建一个表
	bool CreatTable(string _TableName,const vector<Column>& columnTypeList);
	//删除一个表
	bool DeleteTable(string _TableName);
	//删除表里面的数据
	bool DeleteTableData(string _TableName);
	//获得所有表名称
	bool GetTableList(vector<string>& _Tables);
	//获得对应表的表头
	bool GetTableColumns(string _TableName, vector<Column>& columnTypeList);
	//
	bool InsertData(
		string _TableName,
		const vector<Column>& columnTypeList,
		const vector<string> data);
	//
	bool StringToCloumn(const vector<string>& _Tables, Column& column);
	//将DataType类型的数据类型转为字符串形式
	string DateTypeToString(const Column& column);
	//将查询到的字符串形式的数据类型转为DataType
	DataType StringToDateType(const string& column);
	//将数据属性转为命令字符串
	string GetDateAttribute(DataAttribute Attribute);
	//获得数据长度的字符串
	string GetDateSize(const Column& column);
	//
	int StringToDateSize(const string& datasize);
	MYSQL mydata;
	//将字符串转为数据类型
	Column ExplaincolumnType(const vector<string>& columnstringList);
	string _lastError;
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

