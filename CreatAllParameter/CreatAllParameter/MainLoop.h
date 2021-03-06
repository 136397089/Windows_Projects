#pragma once
#include "stdafx.h"
#include <string>
#include "Number/NumberInter.h"
#include "Locker.h"
#include "Number/NumberBase.h"
#include "FreqStatistice.h"
#include "StatisticeInter.h"
#include "StateInter.h"

using namespace std;


class CMainLoop
{
public:
	CMainLoop();
	~CMainLoop();
	//////////////////////////////////////////////////////////////////////////
	//新建多个线程，分析目标文件夹下的股票数据，用于日数据的分析
	//////////////////////////////////////////////////////////////////////////
	bool CreateThreadToAnaHistory_Daily(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//分析目标文件夹下的股票数据，用于日数据的分析，间线程运行函数
	//////////////////////////////////////////////////////////////////////////
	bool ReadFileToAnaHistory_Daily(const string& fileName, const string& strFolderPath,	CIndicatorsInterface& shnumber) const;
	//////////////////////////////////////////////////////////////////////////
	//新建多个线程，分析目标文件夹下的股票数据
	//////////////////////////////////////////////////////////////////////////
	bool CreateThreadToAnaHistory_Mintue(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//分析目标文件夹下的股票数据，用于分钟数据的分析，间线程运行函数
	//////////////////////////////////////////////////////////////////////////
	bool ReadFileToAnaHistory_Minute(const string& fileName, const string& strFolderPath,CIndicatorsInterface& shnumber) const;
private:
	//////////////////////////////////////////////////////////////////////////
	//将计算完的数据保存到文件当中
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToFile(const string& strFilePath,StockDataTable & allData);
	//分析实时数据
	bool AnaCurrentRealTimeData(const string& strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//统计文件夹下文件的个数
	//////////////////////////////////////////////////////////////////////////
	int StatisticalFileQuantity(string strPath);

};

typedef struct
{
	int nIndex;
	string fileName;
	string Filepath;
	CIndicatorsInterface* shnumber;
// 	CMainLoop* mainloop;
	HANDLE hThread;
}ThreadParam;
