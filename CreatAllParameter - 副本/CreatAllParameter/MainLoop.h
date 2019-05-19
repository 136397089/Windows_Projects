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

	HANDLE _StockCSVFileMutex;

	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的值
	//////////////////////////////////////////////////////////////////////////
	bool AnaHistory(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool AnaHistory_File(
		const string& fileName,
		const string& strFolderPath,
		const CNumberInterface& shnumber,
		CStatisticeInter& statisticeInter);
	//////////////////////////////////////////////////////////////////////////
	//统计文件夹下文件的个数
	//////////////////////////////////////////////////////////////////////////
	bool StatisticalFileQuantity(string strPath,int & fileNumber);
	//分析实时数据
	bool AnaCurrentRealTimeData(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//将计算完的数据保存到文件当中
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToFile(const string& strFilePath,const StockDataTable & allData);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool SaveFreqToFile(const string& strFilePath, map<string, FreqListType>& allFreqlost);
};

typedef struct
{
	int nIndex;
	string fileName;
	string Filepath;
	CNumberInterface* shnumber;
	CStatisticeInter* statisticeInter;
	CMainLoop* mainloop;
	HANDLE hThread;
}ThreadParam;
