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
	int datasize;//������ַ�����ͨ��datasizeָ�����ݵĳ���
	DataAttribute attribute;
	string remarkData;//���ڼ�¼
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
	//����һ�����ݿ�
	bool CreatDataBases(const string& _Databases);
	//ɾ��һ�����ݿ�
	bool DeleteDataBases(const string& _Databases);
	//����������ݿ�����
	bool GetDatabasesList(vector<string>& _Databases);
	//�������ݿ�
	bool UseDatabases(const string& _Databases);
	//����һ����
	bool CreatTable(const string& _TableName,const vector<Column>& columnTypeList);
	//ɾ��һ����
	bool DeleteTable(const string& _TableName);
	//ɾ�������������
	bool DeleteTableData(const string& _TableName);
	//������б�����
	bool GetTableList(vector<string>& _Tables);
	//�����Ƿ����
	bool CheckTableExists(string _TableName);
	//��ñ�ı�ͷ
	bool GetTableColumns(const string& _TableName, vector<Column>& columnTypeList);
	//����ͷ�Ƿ񶼴���
	bool CheckTableColumnsExists(const string& _TableName, const vector<Column>& _columnsName);
	//����ͷ���ӵ���Ӧ�ı���
	bool AddColumnToTable(const string& _TableName,const vector<Column>& _columnsName);
	//
	bool DeleteColumn(const string& _TableName, const string& columnName);
	//�������ݵ����ݿ�
	bool InsertData(string _TableName,const vector<Column>& columnTypeList,const vector<string>& dataList);
	//���±�����
	bool UpDateData(
		string _TableName,
		const vector<Column>& columnTypeList,
		const vector<string>& dataList,
		const vector<WhereCommand>& commandList
/*		const string& whereLimit*/);
	//�ӱ��ж�ȡ����
	bool ReadDataFromTable(
		const string _TableName,
		const vector<Column>& columnTypeList,
		vector<vector<string>>& _returndatas);
	//�ӱ��ж�ȡ����
	bool ReadDataFromTable(
		const string _TableName,
		const vector<Column>& columnTypeList,
		const vector<WhereCommand>& whereList,
		vector<vector<string>>& _returndatas);
	//����Ƿ��д��ڶ�Ӧ������
	bool CheckDataExists(const string _TableName, const vector<WhereCommand>& whereList);
	//
	bool SetColumnPrimaryKey(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnUnique(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnIndex(const string& _TableName, const string& _ColumnName);
	//
	bool SetColumnCombinUnique(const string& _TableName, const vector<string>& _ColumnName);
	//�������ݵ����ݿ�֮ǰ���ã����ض����͵�����תΪ�ַ���
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
	//��DataType���͵���������תΪ�ַ�����ʽ
	string DateTypeToString(const Column& column);
	//����ѯ�����ַ�����ʽ����������תΪDataType
	DataType StringToDateType(const string& column);
	//����������תΪ�����ַ���
	string DateAttributeToString(DataAttribute Attribute);
	//������ݳ��ȵ��ַ���
	string GetDateSize(const Column& column);
	//
	int StringToDateSize(const string& datasize);
	//���ַ���תΪ��������
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
		map<string, int> _TableMap;//���ڼ�¼֮ǰ�Ѿ��鵽�ı����ٷ���ָ��Ĵ���
		map<string, int> _TableColMap;//���ڼ�¼֮ǰ�Ѿ��鵽�ı�ͷ�����ٷ���ָ��Ĵ���
	} indexmap;
};
#define SZCHARSIZE 100
struct _tagMysqlInfo
{
	char host[SZCHARSIZE];    //����
	char user[SZCHARSIZE];			//�û���
	char passwd[SZCHARSIZE];    //����
	char db[SZCHARSIZE];		//���ݿ�
	UINT port;				//�˿ں�
	char unix_socket[SZCHARSIZE];    //���unix_socket����NULL�����ַ���������Ӧʹ�õ��׽��ֻ������ܵ���ע�⣬��host���������������ӵ����͡�
	DWORD client_flag;    // client_flag��ֵͨ��Ϊ0
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