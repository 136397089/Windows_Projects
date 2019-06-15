#include "stdafx.h"
// #include <windows.h>
#include "MainLoop.h"
#include "Matrix.h"
#include "glog/logging.h"
#include "StockAccountNum.h"
#include "PythonInterface.h"
#include "BullMarkBearMark.h"
#include "DataSizeSwich.h"
#include "MeanVariance.h"
#include "numberstosql.h"
#include "ComTime.h"
CRITICAL_SECTION threadParamCS;
static bool threadParamCSIsInition = false;
int threadNumber = 0;
int WINAPI TheadFunc(LPVOID lpParam);
int WINAPI TheadFunc_Mintue(LPVOID lpParam);

CMainLoop::CMainLoop(){}


CMainLoop::~CMainLoop(){}
//成交量发生巨大变化是很能体现资金心态发生改变的

bool CMainLoop::CreateThreadToAnaHistory_Daily(string strFolderPath)
{
	//统计文件夹下的文件数
	int filenumber;
	filenumber = StatisticalFileQuantity(strFolderPath);
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h){
		LOG(ERROR) << "Get File handle from " << mFilePath << "error.";
		return false;
	}
	//读取上证指数的数据
	CIndicatorsInterface ShanghaiStockExchangeIndex;
	ShanghaiStockExchangeIndex.GetDataAndIndicators_History("sh.csv", "D:\\StockFile\\whole");
	string outPath = "D:\\StockFile\\OutPutFile\\";
	if (!threadParamCSIsInition)
	{
		InitializeCriticalSection(&threadParamCS);
		threadParamCSIsInition = true;
	}
	ThreadParam* threadParam;
	//遍历所有文件
	do
	{
		while (threadNumber > 0) Sleep(10);//限制总的线程数量
		threadNumber++;
		EnterCriticalSection(&threadParamCS);
		threadParam = new ThreadParam;
		threadParam->fileName = p.cFileName;
		threadParam->Filepath = strFolderPath;
		threadParam->shnumber = &ShanghaiStockExchangeIndex;
		LeaveCriticalSection(&threadParamCS);
		threadParam->hThread = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)TheadFunc,threadParam, NULL, NULL);
		Sleep(10);
	} while (FindNextFile(h, &p));

	//如何线程还没有全部退出则等待
	while (threadNumber != 0)
	{
		LOG(INFO) << "Waiting for all thread exit.";
		Sleep(100);
	}
	if (threadParamCSIsInition)
	{
		DeleteCriticalSection(&threadParamCS);
		threadParamCSIsInition = false;
	}

	return true;
}



int CMainLoop::StatisticalFileQuantity(string strPath)
{
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;

	mFilePath = strPath + "//*.csv";

	h = FindFirstFile(mFilePath.c_str(), &p);
	int fileNumber = 0;
	int FileIndex = 0;
	while (FindNextFile(h, &p))
	{
		fileNumber++;
	}
	return fileNumber;
}

bool CMainLoop::SaveDataToFile(const string& strFullFilePath, StockDataTable & allData) 
{
	CNumberBase FileTool;
	FileTool.AddRanksToCsvFileBegin(strFullFilePath);
	FileTool.AddRanksToCsvFile(strFullFilePath, allData._vTimeDay, GetNumberNameByIndexType(_eFile_Date_INDEX));
	StockDataPointer const pointers = allData.GetAllPointerToSave();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.AddRanksToCsvFile(strFullFilePath, *(ite->second), GetNumberNameByIndexType(ite->first));
	}
	FileTool.AddRanksToCsvFileEnd(strFullFilePath);
	return true;
}

bool CMainLoop::ReadDailyFileToAnaHistory(	const string& fileName,
	const string& strFolderPath,	CIndicatorsInterface& shnumber	)const
{
	CStateInter daystate;
	CStateInter weekstate;
	CStateInter monthstate;
	CIndicatorsInterface numberTool;
	numberTool.GetDataAndIndicators_History(fileName, strFolderPath);
// 	SaveDataToFile(outPath + p.cFileName, numberTool.GetDayValue());
//	daystate.Inter(numberTool.GetDayValue(), fileName);
// 	weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
// 	monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
// 	statisticeInter.StatisticeASIData(numberTool.GetDayValue(), daystate, fileName);
	CStatisticeInter statisticeInter;
	statisticeInter.Inition();
	statisticeInter.ReturnStatistics(numberTool,shnumber.GetResourceValue());
// 	SaveDataToFile("D:\\StockFile\\OutPutFile\\" + fileName, numberTool.GetDayValue());
//	allFreqlist[p.cFileName] = (statisticeInter._FreqList);
	LOG(INFO) << fileName << " Finished";
//	continue;
	return true;
}




bool CMainLoop::ReadFileToAnaHistory_Minute(const string& fileName, const string& strFolderPath, CIndicatorsInterface& shnumber) const
{
	CStateInter daystate;
	CStateInter weekstate;
	CStateInter monthstate;
	CIndicatorsInterface IndicatorsTool;
	IndicatorsTool.GetDataAndIndicatorMintue_History(fileName, strFolderPath);
	CRealTimeAna DataAnaTool;
	DataAnaTool.Inition();
	DataAnaTool.AnalysisRealTimeData(IndicatorsTool.GetResourceValue());
	LOG(INFO) << fileName << " Finished";
	return true;
}
bool CMainLoop::AnaCurrentRealTimeData(const string& strFolderPath)
{
	CRealTimeAna analTool;
	CIndicatorsInterface IndicatorsTool;
	WIN32_FIND_DATA p;
	string mFileType = strFolderPath + "//*.csv";;
	HANDLE h = nullptr;
	string stockCode;
	int loopTime = 0;
	CNumbersToSql sqlTool;
	sqlTool.IniMysqlTool();
	CDate lastDate = IndicatorsTool.GetLastDate();
	lastDate._hour = 15;
	sqlTool.RefreshTemporaryNumberData(lastDate);
	SYSTEMTIME starttime = { 0 };
	SYSTEMTIME endtime = { 0 };
	CComTime timeer;
	IndicatorsTool.RefreshAllStockDate_RealTime();
	while (true)
	{
		cout << loopTime << "  ";
		if (loopTime == 0)
		{
			cout << endl << "请输入循环次数：";
			cin >> loopTime;
		}
		if (loopTime < 0) exit(1);
		loopTime--;
		IndicatorsTool.RefreshAllStockDate_RealTime();
		h = FindFirstFile(mFileType.c_str(), &p);
		do{
			stockCode = p.cFileName;
			if (stockCode.find(".") > 0 && stockCode.find(".") < 100)
				stockCode = stockCode.substr(0, stockCode.find("."));
			timeer.TimeBegin();
			IndicatorsTool.GetIndicators_Realtime(stockCode,eMinute30);
			timeer.TimeEnd("GetIndicators_Realtime:");
			timeer.TimeBegin();
			analTool.AnalysisRealTimeData(IndicatorsTool.GetRealtimeValue());
			timeer.TimeEnd("AnalysisRealTimeData:");
		} while (FindNextFile(h, &p));
	}
	exit(1);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//新建多个线程，分析目标文件夹下的股票数据
bool CMainLoop::CreateThreadToAnaHistory_Mintue(string strFolderPath)
{
	//1.1统计文件夹下的文件数
	int filenumber;
	filenumber = StatisticalFileQuantity(strFolderPath);
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h){
		LOG(ERROR) << "Get File handle from " << mFilePath << "error.";
		return false;
	}
	//1.2读取上证指数的数据
	CIndicatorsInterface ShanghaiStockExchangeIndex;
	ShanghaiStockExchangeIndex.GetDataAndIndicators_SH("sh.csv", "D:\\StockFile\\whole");
	string outPath = "D:\\StockFile\\OutPutFile\\";
	//1.3遍历所有文件
	if (!threadParamCSIsInition)
	{
		InitializeCriticalSection(&threadParamCS);
		threadParamCSIsInition = true;
	}	ThreadParam* threadParam;
	do
	{
		while (threadNumber > 0) Sleep(10);//限制总的线程数量
		threadNumber++;
		EnterCriticalSection(&threadParamCS);
		threadParam = new ThreadParam;
		threadParam->fileName = p.cFileName;
		threadParam->Filepath = strFolderPath;
		threadParam->shnumber = &ShanghaiStockExchangeIndex;
		LeaveCriticalSection(&threadParamCS);
		threadParam->hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TheadFunc_Mintue, threadParam, NULL, NULL);
		Sleep(10);
	} while (FindNextFile(h, &p));
	//1.4如何线程还没有全部退出则等待
	while (threadNumber != 0)
	{
		LOG(INFO) << "Waiting for all thread exit.";
		Sleep(100);
	}
	//1.5
	AnaCurrentRealTimeData(strFolderPath);
	if (threadParamCSIsInition)
	{
		DeleteCriticalSection(&threadParamCS);
		threadParamCSIsInition = false;
	}

	return true;
}



int WINAPI TheadFunc(LPVOID lpParam)
{
	EnterCriticalSection(&threadParamCS);
	ThreadParam*  threadParam = (ThreadParam *)lpParam;
	int dwRtn = threadParam->nIndex;
	string _filename = threadParam->fileName;
	string _filepath = threadParam->Filepath;
	CIndicatorsInterface* _shnumber = threadParam->shnumber;
	CMainLoop _mainloop;
	LeaveCriticalSection(&threadParamCS);

	_mainloop.ReadDailyFileToAnaHistory(_filename,_filepath,*_shnumber);
	threadNumber --;
	delete threadParam;
	return dwRtn;
}


int WINAPI TheadFunc_Mintue(LPVOID lpParam)
{
	EnterCriticalSection(&threadParamCS);
	ThreadParam*  threadParam = (ThreadParam *)lpParam;
	int dwRtn = threadParam->nIndex;
	string _filename = threadParam->fileName;
	string _filepath = threadParam->Filepath;
	CIndicatorsInterface* _shnumber = threadParam->shnumber;
	CMainLoop _mainloop;
	LeaveCriticalSection(&threadParamCS);

	_mainloop.ReadFileToAnaHistory_Minute(_filename, _filepath, *_shnumber);
	threadNumber--;
	delete threadParam;
	return dwRtn;
}

