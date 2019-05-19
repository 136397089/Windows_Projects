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
	int datasize;//������ַ�����ͨ��datasizeָ�����ݵĳ���
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
	//����һ�����ݿ�
	bool CreatDataBases(string _Databases);
	//ɾ��һ�����ݿ�
	bool DeleteDataBases(string _Databases);
	//����������ݿ�����
	bool GetDatabasesList(vector<string>& _Databases);
	//�������ݿ�
	bool UseDatabases(string _Databases);
	//����һ����
	bool CreatTable(string _TableName,const vector<Column>& columnTypeList);
	//ɾ��һ����
	bool DeleteTable(string _TableName);
	//ɾ�������������
	bool DeleteTableData(string _TableName);
	//������б�����
	bool GetTableList(vector<string>& _Tables);
	//��ö�Ӧ��ı�ͷ
	bool GetTableColumns(string _TableName, vector<Column>& columnTypeList);
	//
	bool InsertData(
		string _TableName,
		const vector<Column>& columnTypeList,
		const vector<string> data);
	//
	bool StringToCloumn(const vector<string>& _Tables, Column& column);
	//��DataType���͵���������תΪ�ַ�����ʽ
	string DateTypeToString(const Column& column);
	//����ѯ�����ַ�����ʽ����������תΪDataType
	DataType StringToDateType(const string& column);
	//����������תΪ�����ַ���
	string GetDateAttribute(DataAttribute Attribute);
	//������ݳ��ȵ��ַ���
	string GetDateSize(const Column& column);
	//
	int StringToDateSize(const string& datasize);
	MYSQL mydata;
	//���ַ���תΪ��������
	Column ExplaincolumnType(const vector<string>& columnstringList);
	string _lastError;
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

