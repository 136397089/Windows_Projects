

#ifndef _COMMONFUNSTION_H
#define _COMMONFUNSTION_H

// #include "stdafx.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;
//////////////////////////////////////////////////////////////////////////
//�и��ַ���
//inputstring:���и���ַ���
//CutIndex:�и���ַ�
//
//////////////////////////////////////////////////////////////////////////
vector<string> CutString(string inputstring,string CutIndex);

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
string AssembleString(vector<string>inputstring,string Index);





//////////////////////////////////////////////////////////////////////////
//�ڿ���̨��ʾ������Ϣ
//showInformation -- string :Ҫ��ʾ����Ϣ�ַ���
//////////////////////////////////////////////////////////////////////////
void ShowDebugInformationInCon(string showInformation);

//////////////////////////////////////////////////////////////////////////
//�����������ת��Ϊ�ַ������
//
//////////////////////////////////////////////////////////////////////////
template <typename T, typename...Args> 
string PrintToString(const T &t, const Args&...rest);

template <typename T> 
string PrintToString(const T &t);
#define Return_IF_FALSE(_bool_)\
if (!_bool_)\
{\
	return false;\
}

#define  RETURN_IFNULL(VAL) if (NULL == VAL) return;

#define TO_STRING(n,str){str="";stringstream ss;ss  n;	ss >> str;}


typedef  float tySData;
typedef vector < tySData > VStockData;
typedef vector<string> StringList;
typedef vector<StringList> StringBlock;



#endif // _COMMONFUNSTION_H

