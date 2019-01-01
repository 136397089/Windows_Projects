#include "stdafx.h"
#include "MainLoop.h"


CMainLoop::CMainLoop()
{
}


CMainLoop::~CMainLoop()
{
}


bool CMainLoop::RunLoop(string strFolderPath)
{
	CNumberAll tempna;

	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;

	//ͳ���ļ����µ��ļ���
	StatisticalFileQuantity(strFolderPath);
	//
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return false;
	}

	//���������ļ�
	do
	{
		tempna.AnalyseTheFile(p.cFileName, strFolderPath);
		SaveDataToFile(strFolderPath + "//" + p.cFileName, tempna.GetAllValue());
	} while (FindNextFile(h, &p));

	return true;

}

//
//
//
//ͳ���ļ�����
bool CMainLoop::StatisticalFileQuantity(string strPath)
{
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;

	mFilePath = strPath + "//*.csv";

	h = FindFirstFile(mFilePath.c_str(), &p);
	FileNumber = 0;
	FileIndex = 0;
	while (FindNextFile(h, &p))
	{
		FileNumber++;
	}
	return true;
}

bool CMainLoop::SaveDataToFile(const string& strFilePath, const AllStockData & allData)
{
	FileTool.ReSavefileRanks(strFilePath, allData._vAsi, "ASI");
	FileTool.ReSavefileRanks(strFilePath, allData._vAsi, "ASI");
	FileTool.ReSavefileRanks(strFilePath, allData._vAsit, "ASIT");
	FileTool.ReSavefileRanks(strFilePath, allData._vDEA, MACD_DEA);
	FileTool.ReSavefileRanks(strFilePath, allData._vDiff, MACD_DIFF);
	FileTool.ReSavefileRanks(strFilePath, allData._vMACDValue, MACD_BAR);
	FileTool.ReSavefileRanks(strFilePath, allData._vDMAValue, DMA_D);
	FileTool.ReSavefileRanks(strFilePath, allData._vAMAValue, DMA_A);
	FileTool.ReSavefileRanks(strFilePath, allData._vK, KDJ_K);
	FileTool.ReSavefileRanks(strFilePath, allData._vD, KDJ_D);
	FileTool.ReSavefileRanks(strFilePath, allData._vJ, KDJ_J);
	FileTool.ReSavefileRanks(strFilePath, allData._vTRIX, TRIX_TRIX);
	FileTool.ReSavefileRanks(strFilePath, allData._vTRMA, TRIX_MA);
	FileTool.ReSavefileRanks(strFilePath, allData._vtr, TRIX_VTR);
	FileTool.ReSavefileRanks(strFilePath, allData._vtb, TRIX_VTB);
	FileTool.ReSavefileRanks(strFilePath, allData._vta, TRIX_VTA);
	return true;
}


