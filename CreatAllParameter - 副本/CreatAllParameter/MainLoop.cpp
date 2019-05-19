#include "stdafx.h"
#include "MainLoop.h"
#include "Matrix.h"
#include "glog/logging.h"
#include "StockAccountNum.h"
#include "PythonInterface.h"
#include "BullMarkBearMark.h"
#include "DataSizeSwich.h"
#include "SearchStock.h"
#include "MeanVariance.h"
#include "MySQLInterFace.h"

CRITICAL_SECTION threadParamCS;
string mfilepath = "";
int threadNumber = 0;
int WINAPI TheadFunc(LPVOID lpParam);

CMainLoop::CMainLoop()
{


}


CMainLoop::~CMainLoop()
{
}
//成交量发生巨大变化是很能体现资金心态发生改变的

bool CMainLoop::AnaHistory(string strFolderPath)
{
// 	MySQLInterFace tools;
// 	tools.Inition();

#ifdef _MYDEBUG
	CPythonInterface tempinterface;
	tempinterface.Inition();
#endif
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;
	int filenumber;
	//统计文件夹下的文件数
	StatisticalFileQuantity(strFolderPath, filenumber);
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return false;
	}
	StockDataTable stockdata;
	CNumberInterface shnumber;
	//CNumberInterface shnumber;
	map<float, int> vfreqlist;
	CSearchStock SearchTool;
	CMeanVariance meanTool;
	MeanVar meanData;
	string outPath = "D:\\StockFile\\OutPutFile\\";
	shnumber.GetDataAndNumber("sh.csv", "D:\\StockFile\\whole");
	map<string,FreqListType> allFreqlist;
	CStatisticeInter statisticeInter;
	statisticeInter.Inition();
	InitializeCriticalSection(&threadParamCS);
	mfilepath = strFolderPath;
	ThreadParam* threadParam;
	//遍历所有文件
	do
	{
		EnterCriticalSection(&threadParamCS);
		threadParam = new ThreadParam;
		threadParam->fileName = p.cFileName;
		threadParam->Filepath = strFolderPath;
		threadParam->shnumber = &shnumber;
		threadParam->statisticeInter = &statisticeInter;
		threadParam->mainloop = this;
		LeaveCriticalSection(&threadParamCS);
		while (threadNumber > 6)
			Sleep(10);
		threadNumber ++;
		threadParam->hThread = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE)TheadFunc,
			threadParam, NULL, NULL);
		Sleep(10);
	} while (FindNextFile(h, &p));

	while (threadNumber != 0)
	{
		LOG(INFO) << "Waiting for all thread exit.";
		Sleep(100);
	}
	DeleteCriticalSection(&threadParamCS);

	CNumberBase FileTool;
	FileTool.ReSavefileRanksBegin("D:\\StockFile\\returnrate\\returerate.csv");
//	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataVarList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataMeanList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataVarList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataMeanList);
	FileTool.ReSavefileRanksEnd("D:\\StockFile\\returnrate\\returerate.csv");
	SaveFreqToFile("D:\\StockFile\\returnrate\\freq.csv", allFreqlist);
	return true;
}

//
//
//
//统计文件个数


bool CMainLoop::StatisticalFileQuantity(string strPath, int & fileNumber)
{
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;

	mFilePath = strPath + "//*.csv";

	h = FindFirstFile(mFilePath.c_str(), &p);
	fileNumber = 0;
	int FileIndex = 0;
	while (FindNextFile(h, &p))
	{
		fileNumber++;
	}
	return true;
}

bool CMainLoop::SaveDataToFile(const string& strFullFilePath, const StockDataTable & allData)
{
	CNumberBase FileTool;
	FileTool.ReSavefileRanksBegin(strFullFilePath);
	FileTool.ReSavefileRanks(strFullFilePath, allData._vTimeDay, GetIndexNameByIndexType(_eFile_Date_INDEX));
	StockDataPointer pointers = allData.GetAllPointerToSave();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.ReSavefileRanks(strFullFilePath, *(ite->second), GetIndexNameByIndexType(ite->first));
	}
	FileTool.ReSavefileRanksEnd(strFullFilePath);
	return true;
}

bool CMainLoop::SaveFreqToFile(const string& strFilePath, map<string,FreqListType>& allFreqlost)
{
	CNumberBase FileTool;
	FileTool.ReSavefileRanksBegin(strFilePath);
	for (map<string, FreqListType>::const_iterator ite = allFreqlost.begin(); ite != allFreqlost.cend(); ite++)
	{
		FileTool.ReSavefileRanks(strFilePath, ite->second);
	}
	FileTool.ReSavefileRanksEnd(strFilePath);
	return true;
}

bool CMainLoop::AnaCurrentRealTimeData(string strFolderPath)
{
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return false;
	}
	CRealTimeAna RealTimeTool;
	do
	{
		CNumberInterface numberTool;
		numberTool.GetDataAndNumber(p.cFileName, strFolderPath);
		RealTimeTool.AnalysisRealTimeData(numberTool.GetDayValue(), p.cFileName);
		cout << "*******" << p.cFileName << "Finished." << "*******" << endl;
		LOG(INFO) << "*******" << p.cFileName << "Finished." << "*******" << endl;

	} while (FindNextFile(h, &p));

	return true;
}

bool CMainLoop::AnaHistory_File(
	const string& fileName,
	const string& strFolderPath,
	const CNumberInterface& shnumber,
	CStatisticeInter& statisticeInter
	)
{
	CStateInter daystate;
	CStateInter weekstate;
	CStateInter monthstate;
	CNumberInterface numberTool;
	numberTool.GetDataAndNumber(fileName, strFolderPath);
// 	SaveDataToFile(outPath + p.cFileName, numberTool.GetDayValue());
	daystate.Inter(numberTool.GetDayValue(), fileName);
// 	weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
// 	monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
	vector<MeanVarPoint> moveMeanVarList;
	statisticeInter.StatisticeHistoryData(
		numberTool.GetDayValue(),
		numberTool.GetWeekValue(),
		numberTool.GetMonthValue(),
		shnumber.GetDayValue(),
		daystate,
		weekstate,
		monthstate,
		fileName);
// 	SaveDataToFile("D:\\StockFile\\OutPutFile\\" + fileName, numberTool.GetDayValue());
//	allFreqlist[p.cFileName] = (statisticeInter._FreqList);
	LOG(INFO) << fileName << " Finished";
//	continue;
	return true;
}



int WINAPI TheadFunc(LPVOID lpParam)
{
	EnterCriticalSection(&threadParamCS);
	ThreadParam*  threadParam = (ThreadParam *)lpParam;
	int dwRtn = threadParam->nIndex;
	string _filename = threadParam->fileName;
	string _filepath = threadParam->Filepath;
	CNumberInterface* _shnumber = threadParam->shnumber;
	CStatisticeInter* _statisticeInter = threadParam->statisticeInter;
	CMainLoop* _mainloop = threadParam->mainloop;
	LeaveCriticalSection(&threadParamCS);

	_mainloop->AnaHistory_File(
		_filename,
		_filepath,
		*_shnumber,
		*_statisticeInter);
	threadNumber --;
	delete threadParam;
	return dwRtn;
}

