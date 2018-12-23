#ifndef _MACD_H
#define _MACD_H

#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include "IndexNumber.h"
#include "../CommonFunction.h"


using namespace std;
class CMacdManager;

//////////////////////////////////////////////////////////////////////////
//MACD
//
//
//////////////////////////////////////////////////////////////////////////
struct Macd
{
public:
	Macd():m12(0.0f),m26(0.0f),diff(0.0f),dea(0.0f),bar(0.0f){}
	float m12;
	float m26;
	float diff;
	float dea;
	float bar;
};



//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CMacdManager : public CNumberManager
{
public:
	CMacdManager(void);
	~CMacdManager(void);

// 	vector<string> CutString(string inputstring,string CutIndex);


	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的MACD值(数据按列排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的MACD值(天数按行排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath) ;

	//////////////////////////////////////////////////////////////////////////
	//更新CSV文件文件里的MACD值(天数按行排)
	//strPath:文件路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
private:
	//////////////////////////////////////////////////////////////////////////
	//通过前日的MACD获得今日的MACD值
	//closeData:收盘价
	//mMacd:前日的MACD
	//////////////////////////////////////////////////////////////////////////
	Macd GetNextMacd(float closeData, Macd mMacd);
};


#endif
