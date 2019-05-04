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
		CStatisticeInter& statisticeInter);
	//////////////////////////////////////////////////////////////////////////
	//ͳ���ļ������ļ��ĸ���
	//////////////////////////////////////////////////////////////////////////
	bool StatisticalFileQuantity(string strPath,int & fileNumber);
	//����ʵʱ����
	bool AnaCurrentRealTimeData(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//������������ݱ��浽�ļ�����
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
