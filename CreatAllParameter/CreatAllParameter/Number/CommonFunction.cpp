#include "stdafx.h"
#include <windows.h>
#include "CommonFunction.h"
#include <iostream>
#include <time.h> 
#include <fstream>

using namespace std;

//切割字符串
void CutString(const string& inputstring, string CutIndex, vector<string>& ResultStringList)
{

	int frontIndex = inputstring.find(CutIndex);
	int laterIndex = 0;
	while(frontIndex >= 0)
	{
		ResultStringList.push_back(inputstring.substr(laterIndex, frontIndex - laterIndex));
		laterIndex = frontIndex + 1;
		frontIndex = inputstring.find(CutIndex, laterIndex);
	}
	if (laterIndex < inputstring.size()-1)
	{
		ResultStringList.push_back(inputstring.substr(laterIndex, inputstring.size() - laterIndex));
	}
	return ;
}


std::string AssembleString( vector<string>inputstring,string Index )
{
	string ReturnString="";
	for (vector<string>::iterator ite = inputstring.begin();ite!=inputstring.end();ite++)
	{
		ReturnString = ReturnString+*ite+Index;
	}
	//删除最后一个index
	ReturnString = ReturnString.substr(0,ReturnString.size()-1-Index.size());
	return ReturnString;
}



void ShowDebugInformationInCon(string showInformation)
{
// 	SYSTEMTIME sys;
// 	GetLocalTime(&sys);
// 	cout << "[ " << sys.wYear << "/" << sys.wMonth <<"/" << sys.wDay << " : "\
// 		<< sys.wHour << "-" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds<<" ]:\t";
// 	cout <<showInformation << endl;
// 	fstream outfile("d:\\stockfile\\debugfile.txt", ios::out | ios::app);
// 	outfile << "[ " << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " : "\
// 		<< sys.wHour << "-" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds << " ]:\t";
// 	outfile << showInformation << endl;
// 	outfile.close();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template <typename T>
string PrintToString(const T &t)
{
	string str = "";
	stringstream ss;
	ss << t;
	ss >> str;
	return str;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template <typename T, typename...Args>
string PrintToString(const T &t, const Args&...rest)
{
	string str = "";
	stringstream ss;
	ss << t;
	ss >> str;
	str += PrintToString(rest...);//递归调用，打印其他实参  
	return str;
}