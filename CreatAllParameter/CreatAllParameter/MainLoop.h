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
	//����Ŀ¼������CSV�ļ����ֵ
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
	//ͳ���ļ������ļ��ĸ���
	//////////////////////////////////////////////////////////////////////////
	bool StatisticalFileQuantity(string strPath,int & fileNumber);
	//����ʵʱ����
	bool AnaCurrentRealTimeData(const string& strFolderPath,const string& _filename);
	//////////////////////////////////////////////////////////////////////////
	//������������ݱ��浽�ļ�����
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
