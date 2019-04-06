#include "stdafx.h"
#include "MainLoop.h"
#include "StateInter.h"
#include "Matrix.h"
#include "StatisticeInter.h"
#include "glog/logging.h"
#include "StockAccountNum.h"
#include "PythonInterface.h"
#include "BullMarkBearMark.h"
#include "DataSizeSwich.h"
#include "SearchStock.h"
#include "MeanVariance.h"
#include "MySQLInterFace.h"

CMainLoop::CMainLoop()
{


}


CMainLoop::~CMainLoop()
{
}
//成交量发生巨大变化是很能体现资金心态发生改变的

bool CMainLoop::RunLoop(string strFolderPath)
{
	//MySQLInterFace tools;
	//tools.Inition();

#ifdef _MYDEBUG
	CPythonInterface tempinterface;
	tempinterface.Inition();
#endif
	WIN32_FIND_DATA p;
	HANDLE h;
	string mFilePath;
	//统计文件夹下的文件数
	StatisticalFileQuantity(strFolderPath);
	mFilePath = strFolderPath + "\\*.csv";
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return false;
	}
	StockDataTable stockdata;
	CNumberInterface numberTool;
	CStateInter daystate;
	CStateInter weekstate;
	CStateInter monthstate;
	CStatisticeInter statisticeInter;
	map<float, int> vfreqlist;
	CSearchStock SearchTool;
	CMeanVariance meanTool;
	string outPath = "D:\\StockFile\\OutPutFile\\";
	//遍历所有文件
	do
	{
		numberTool.GetDataAndNumber(p.cFileName, strFolderPath);
		daystate.Inter(numberTool.GetDayValue(), p.cFileName);
		weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
		monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
//		SaveDataToFile(outPath + p.cFileName, numberTool.GetDayValue());
// 		if (SearchTool.Inter(numberTool.GetDayValue(),
// 			numberTool.GetWeekValue(),
// 			numberTool.GetMonthValue(),
// 			daystate, weekstate, monthstate))
// 		{
// 			LOG(INFO) << "Find Stock: " << p.cFileName;
// 			LOG(ERROR) << "Find Stock: " << p.cFileName;
// 		}
		LOG(ERROR) << p.cFileName << "Finished";
		continue;
		statisticeInter.Inter(
			numberTool.GetDayValue(),
			numberTool.GetWeekValue(),
			numberTool.GetMonthValue(),
			daystate,weekstate,monthstate);
		return true;
	} while (FindNextFile(h, &p));
	return true;
}

//
//
//
//统计文件个数
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

bool CMainLoop::SaveDataToFile(const string& strFilePath, const StockDataTable & allData)
{
	FileTool.ReSavefileRanksBegin(strFilePath);
	FileTool.ReSavefileRanks(strFilePath, allData._vTimeDay, GetIndexNameByIndexType(_eFile_Date_INDEX));
	StockDataPointer pointers = allData.GetAllPointerToSave();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.ReSavefileRanks(strFilePath, *(ite->second), GetIndexNameByIndexType(ite->first));
	}
	FileTool.ReSavefileRanksEnd(strFilePath);
	return true;
}


