#include "stdafx.h"
#include "MainLoop.h"
#include "StatusInter.h"
#include "Matrix.h"
#include "StatisticeInter.h"
#include "glog/logging.h"
#include "StockAccountNum.h"
#include "PythonInterface.h"
#include "BullMarkBearMark.h"

CMainLoop::CMainLoop()
{


}


CMainLoop::~CMainLoop()
{
}


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

	CNumberInterface tempna;
	CStatusInter statusinter;
	CStatisticeInter statisticeInter;
	map<float, int> vfreqlist;
	//遍历所有文件
	do
	{
		tempna.AnalyseTheFile(p.cFileName, strFolderPath);
// 		SaveDataToFile(strFolderPath + "//" + p.cFileName, tempna.GetAllValue());
		AllStockData allstockdata = tempna.GetAllValue();
		statusinter.Inter(allstockdata, p.cFileName);
		statisticeInter.Inter(allstockdata, statusinter);
#ifdef _MYDEBUG
		CBullMarkBearMark tempmatket;
		MarketTypeList templist = tempmatket.GetMarketTypes(allstockdata._vCloseData, allstockdata._vTimeDay);
		string logfailepath = "D:\\StockFile\\Log\\";
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, statusinter.allIndexStatus[_eMACD_DEA].GetLocalDay(), "Day");
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, statusinter.allIndexStatus[_eMACD_DEA].GetLocalStatusPointsValue(), "Localstatus");
		FileTool.ReSavefileRanks(logfailepath + p.cFileName, statusinter.allIndexStatus[_eMACD_DEA].GetTrendStatusPointsValue(), "LowTrendstatus");
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

bool CMainLoop::SaveDataToFile(const string& strFilePath, const AllStockData & allData)
{
	StockDataPointer pointers = allData.GetAllPointer();
	for (StockDataPointer::const_iterator ite = pointers.cbegin(); ite != pointers.cend(); ite++)
	{
		FileTool.ReSavefileRanks(strFilePath, *(ite->second), GetIndexNameByIndexType(ite->first));
	}

	return true;
}


