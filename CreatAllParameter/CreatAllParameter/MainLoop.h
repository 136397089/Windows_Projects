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
		CStatisticeInter& statisticeInter) const ;
	//////////////////////////////////////////////////////////////////////////
	//统计文件夹下文件的个数
	//////////////////////////////////////////////////////////////////////////
	bool StatisticalFileQuantity(string strPath,int & fileNumber);
	//分析实时数据
	bool AnaCurrentRealTimeData(const string& strFolderPath,const string& _filename);
	//////////////////////////////////////////////////////////////////////////
	//将计算完的数据保存到文件当中
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToFile(const string& strFilePath,const StockDataTable & allData);


private:
	bool GetCloselyNumber_Current(
		const map<string, SinDayData>& CurrStockData,
		map<string, MACDCombin>& YesTerdayMacdMap,
		map<string, KDJCombin>& YesTerdayKDJMap,
		map<string, vector<SinDayPriceData>>& FrontPrice);
	bool GetNextNumber_Current(
		const map<string, SinDayData>& CurrStockData,
		map<string, MACDCombin>& YesTerdayMacdMap,
		map<string, KDJCombin>& YesTerdayKDJMap,
		map<string, vector<SinDayPriceData>>& FrontPrice);

};

typedef struct
{
	int nIndex;
	string fileName;
	string Filepath;
	CNumberInterface* shnumber;
	CStatisticeInter* statisticeInter;
// 	CMainLoop* mainloop;
	HANDLE hThread;
}ThreadParam;
