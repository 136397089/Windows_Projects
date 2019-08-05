#include "stdafx.h"
#include "SaveResult.h"
#include <vector>
#include <fstream>
#include <time.h>
#include <windows.h>
using namespace std;

static vector<string> codeList;
static string codefilepath = "D:/MyProjects/python/stockcrawl/ÏÂÔØÊý¾Ý/SellBuy/codefile.csv";
CRITICAL_SECTION threadParamCL;
static bool threadParamCSIsInition = false;

CSaveCodeList::CSaveCodeList()
{
	if (!threadParamCSIsInition)
	{
 		InitializeCriticalSection(&threadParamCL);
		threadParamCSIsInition = true;
	}
}


CSaveCodeList::~CSaveCodeList()
{
}

void CSaveCodeList::ClearCodeList()
{
 	EnterCriticalSection(&threadParamCL);
	codeList.clear();
 	LeaveCriticalSection(&threadParamCL);
}

void CSaveCodeList::AddCode(const string& code)
{
 	EnterCriticalSection(&threadParamCL);
	codeList.push_back(code);
 	LeaveCriticalSection(&threadParamCL);
}

void CSaveCodeList::SaveCode()
{
	if (codeList.size() <= 1)
		return;
 	EnterCriticalSection(&threadParamCL);
	fstream codefile(codefilepath.c_str(), ios::in | ios::out | ios::app);
	codefile << endl;
	time_t tt = time(NULL);
	tm t;
	localtime_s(&t,&tt);
	int _year = t.tm_year + 1900;
	int _month = t.tm_mon + 1;
	int _day = t.tm_mday;
	codefile << _year << "-" << _month << "-" << _day;
	for (unsigned int index = 0; index < codeList.size(); index++)
	{
		codefile << "," << codeList[index];
	}
	codefile.close();
 	LeaveCriticalSection(&threadParamCL);
}

bool CSaveCodeList::CheckToday(const CDate& today)
{
	time_t tt = time(NULL);
	tm t;
	localtime_s(&t, &tt);
	int _year = t.tm_year + 1900;
	int _month = t.tm_mon + 1;
	int _day = t.tm_mday;


	return true;
}

