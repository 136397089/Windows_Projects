

#ifndef _COMMONFUNSTION_H
#define _COMMONFUNSTION_H

// #include "stdafx.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;
//////////////////////////////////////////////////////////////////////////
//切割字符串
//inputstring:被切割的字符串
//CutIndex:切割的字符
//
//////////////////////////////////////////////////////////////////////////
vector<string> CutString(string inputstring,string CutIndex);

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
string AssembleString(vector<string>inputstring,string Index);





//////////////////////////////////////////////////////////////////////////
//在控制台显示调试信息
//showInformation -- string :要显示的信息字符串
//////////////////////////////////////////////////////////////////////////
void ShowDebugInformationInCon(string showInformation);

//////////////////////////////////////////////////////////////////////////
//将输入参数都转化为字符串输出
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

