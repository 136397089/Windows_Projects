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

CRITICAL_SECTION MYSQLCS;
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
#ifdef _MYDEBUG
	CPythonInterface tempinterface;
	tempinterface.Inition();
#endif
	//统计文件夹下的文件数
	int filenumber;
	StatisticalFileQuantity(strFolderPath, filenumber);
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
	CNumberInterface shnumber;
	shnumber.GetDataAndNumbers("sh.csv", "D:\\StockFile\\whole");
	string outPath = "D:\\StockFile\\OutPutFile\\";

	CStatisticeInter statisticeInter;
	statisticeInter.Inition();
	InitializeCriticalSection(&MYSQLCS);
	ThreadParam* threadParam;
	//遍历所有文件
	do
	{
		while (threadNumber > 5) Sleep(10);//限制总的线程数量
		threadNumber++;
		EnterCriticalSection(&MYSQLCS);
		threadParam = new ThreadParam;
		threadParam->fileName = p.cFileName;
		threadParam->Filepath = strFolderPath;
		threadParam->shnumber = &shnumber;
		threadParam->statisticeInter = &statisticeInter;
		LeaveCriticalSection(&MYSQLCS);
		threadParam->hThread = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)TheadFunc,threadParam, NULL, NULL);
		Sleep(10);
	} while (FindNextFile(h, &p));

	//如何线程还没有全部退出则等待
	while (threadNumber != 0)
	{
		LOG(INFO) << "Waiting for all thread exit.";
		Sleep(100);
	}
	DeleteCriticalSection(&MYSQLCS);

	CNumberBase FileTool;
	FileTool.AddRanksToCsvFileBegin("D:\\StockFile\\returnrate\\returerate.csv");
//	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataVarList);
//	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataMeanList);
	FileTool.AddRanksToCsvFile("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataVarList);
	FileTool.AddRanksToCsvFile("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataMeanList);
	FileTool.AddRanksToCsvFileEnd("D:\\StockFile\\returnrate\\returerate.csv");

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
	FileTool.AddRanksToCsvFileBegin(strFullFilePath);
	FileTool.AddRanksToCsvFile(strFullFilePath, allData._vTimeDay, GetIndexNameByIndexType(_eFile_Date_INDEX));
	StockDataPointer pointers = allData.GetAllPointerToSave();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.AddRanksToCsvFile(strFullFilePath, *(ite->second), GetIndexNameByIndexType(ite->first));
	}
	FileTool.AddRanksToCsvFileEnd(strFullFilePath);
	return true;
}


bool CMainLoop::AnaCurrentRealTimeData(const string& strFolderPath, const string& strfilename)
{
	//从CSV读取今日的数据
	CNumberInterface numtool;
	map<string, SinDayData> CurrStockData;
	//从MySQL读取最近的MACD KDJ Price
	map<string, MACDCombin> YesTerdayMacdMap;
	map<string, KDJCombin> YesTerdayKDJMap;
	map<string, vector<SinDayPriceData>> FrontPrice;
	numtool.GetTodayData(strFolderPath + "//" + strfilename, CurrStockData);
	GetCloselyNumber_Current(CurrStockData, YesTerdayMacdMap, YesTerdayKDJMap, FrontPrice);
	CRealTimeAna RealTimeTool;
	while (true)
	{
		numtool.GetTodayData(strFolderPath + "//" + strfilename, CurrStockData);
		GetNextNumber_Current(CurrStockData, YesTerdayMacdMap, YesTerdayKDJMap, FrontPrice);
		RealTimeTool.AnalysisRealTimeData(YesTerdayMacdMap, YesTerdayKDJMap);
		cout << endl;
		system("pause");
	}
	
	exit(1);

	return true;
}

bool CMainLoop::AnaHistory_File(
	const string& fileName,
	const string& strFolderPath,
	const CNumberInterface& shnumber,
	CStatisticeInter& statisticeInter
	)const
{
	CStateInter daystate;
	CStateInter weekstate;
	CStateInter monthstate;
	CNumberInterface numberTool;
	numberTool.GetDataAndNumbers(fileName, strFolderPath);
// 	SaveDataToFile(outPath + p.cFileName, numberTool.GetDayValue());
//	daystate.Inter(numberTool.GetDayValue(), fileName);
// 	weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
// 	monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
// 	statisticeInter.StatisticeASIData(numberTool.GetDayValue(), daystate, fileName);
	statisticeInter.GroupDabug(
		numberTool.GetDayValue(),
		numberTool.GetWeekValue(),
		numberTool.GetMonthValue(),
		shnumber.GetDayValue(),
		daystate,
		weekstate,
		monthstate);
// 	SaveDataToFile("D:\\StockFile\\OutPutFile\\" + fileName, numberTool.GetDayValue());
//	allFreqlist[p.cFileName] = (statisticeInter._FreqList);
	LOG(INFO) << fileName << " Finished";
//	continue;
	return true;
}

bool CMainLoop::GetCloselyNumber_Current(
	const map<string, SinDayData>& CurrStockData,
	map<string, MACDCombin>& YesTerdayMacdMap,
	map<string, KDJCombin>& YesTerdayKDJMap,
	map<string, vector<SinDayPriceData>>& FrontPrice)
{
	CNumbersToSql SQlTool;
	MACDCombin _Todaymacd;
	for (map<string, SinDayData>::const_iterator ite = CurrStockData.begin(); ite != CurrStockData.end(); ite++)
	{
		if (!SQlTool.RecoveryMACDData(ite->first, "day", _Todaymacd))
			continue;
		FrontPrice[ite->first].resize(8);
		YesTerdayMacdMap[ite->first] = _Todaymacd;
		SQlTool.RecoveryPriceData(ite->first, "day", FrontPrice[ite->first]);
		SQlTool.RecoveryKDJData(ite->first, "day", YesTerdayKDJMap[ite->first]);
	}
	GetNextNumber_Current(CurrStockData, YesTerdayMacdMap, YesTerdayKDJMap, FrontPrice);
	return true;
}

bool CMainLoop::GetNextNumber_Current(
	const map<string, SinDayData>& CurrStockData,
	map<string, MACDCombin>& YesTerdayMacdMap,
	map<string, KDJCombin>& YesTerdayKDJMap,
	map<string, vector<SinDayPriceData>>& FrontPrice)
{
	CMacdCal macdCalTool;
	CKDJCal KDJTool;
	for (map<string, SinDayData>::const_iterator ite = CurrStockData.begin(); ite != CurrStockData.end(); ite++)
	{
		if (YesTerdayMacdMap.count(ite->first) == 0)
			continue;
		const SinDayData& TodayData = ite->second;
		MACDCombin& CloselyMacd = YesTerdayMacdMap[ite->first];
		KDJCombin& CloselyKDJ = YesTerdayKDJMap[ite->first];
		vector<SinDayPriceData>& CloselyPrice = FrontPrice[ite->first];
		CloselyMacd.CurrentMacd = CloselyMacd.TodayMacd;
		macdCalTool.GetNextMacd(CloselyPrice[0], CloselyMacd.CurrentMacd);
		CloselyKDJ.CurrentKDJ = CloselyKDJ.TodayKDJ;
		KDJTool.StaticGetNextKDJ(CloselyPrice,TodayData, CloselyKDJ.CurrentKDJ);
	}
	return true;
}



int WINAPI TheadFunc(LPVOID lpParam)
{
	EnterCriticalSection(&MYSQLCS);
	ThreadParam*  threadParam = (ThreadParam *)lpParam;
	int dwRtn = threadParam->nIndex;
	string _filename = threadParam->fileName;
	string _filepath = threadParam->Filepath;
	CNumberInterface* _shnumber = threadParam->shnumber;
	CStatisticeInter* _statisticeInter = threadParam->statisticeInter;
	CMainLoop _mainloop;
	LeaveCriticalSection(&MYSQLCS);

	_mainloop.AnaHistory_File(
		_filename,
		_filepath,
		*_shnumber,
		*_statisticeInter);
	threadNumber --;
	delete threadParam;
	return dwRtn;
}

