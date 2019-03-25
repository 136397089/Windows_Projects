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


CMainLoop::CMainLoop()
{


}


CMainLoop::~CMainLoop()
{
}
//成交量发生巨大变化是很能体现资金心态发生改变的

bool CMainLoop::RunLoop(string strFolderPath)
{

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
	//遍历所有文件
	do
	{
		numberTool.GetDataAndNumber(p.cFileName, strFolderPath);
		//stockdata = numberTool.GetMonthValue();
		//SaveDataToFile(strFolderPath + "//" + p.cFileName, numberTool.GetDayValue());
		//SaveDataToFile(strFolderPath + "//" + "W_" + p.cFileName, numberTool.GetDayValue());
		daystate.Inter(numberTool.GetDayValue(), p.cFileName);
		weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
		monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
		statisticeInter.Inter(
			numberTool.GetDayValue(),
			numberTool.GetWeekValue(),
			numberTool.GetMonthValue(),
			daystate,weekstate,monthstate);
		return true;
#ifdef _MYDEBUG
		CBullMarkBearMark tempmatket;
		MarketTypeList templist = tempmatket.GetMarketTypes(stockdata._vClose, stockdata._vTimeDay);
		string logfailepath = "D:\\StockFile\\Log\\";
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, daystate.allIndexStates[_eMACD_DEA].GetLocalDay(), "Day");
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, daystate.allIndexStates[_eMACD_DEA].GetLocalStatePointsValue(), "Localstate");
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, daystate.allIndexStates[_eMACD_DEA].GetTrendStatePointsValue(), "LowTrendstate");
#endif
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
	//FileTool.ReSavefileRanks(strFilePath, allData._vTimeDay, GetIndexNameByIndexType(_eFile_Date_INDEX));
	StockDataPointer pointers = allData.GetAllPointerToSave();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.ReSavefileRanks(strFilePath, *(ite->second), GetIndexNameByIndexType(ite->first));
	}

	return true;
}


