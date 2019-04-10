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
//�ɽ��������޴�仯�Ǻ��������ʽ���̬�����ı��

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
	//ͳ���ļ����µ��ļ���
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
	MeanVar meanData;
	string outPath = "D:\\StockFile\\OutPutFile\\";
	FileTool.ReSavefileRanksBegin("D:\\StockFile\\returnrate\\returerate.csv");
	//���������ļ�
	do
	{
		numberTool.GetDataAndNumber(p.cFileName, strFolderPath);
		//daystate.Inter(numberTool.GetDayValue(), p.cFileName);
		//weekstate.Inter(numberTool.GetWeekValue(), p.cFileName);
		//monthstate.Inter(numberTool.GetMonthValue(), p.cFileName);
		vector<MeanVarPoint> moveMeanVarList;
		statisticeInter.StatisticeDebugFunstion(
			numberTool.GetDayValue(),
			numberTool.GetWeekValue(),
			numberTool.GetMonthValue(),
			daystate,
			weekstate,
			monthstate,
			p.cFileName);

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
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataVarList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.allDataMeanList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataVarList);
	FileTool.ReSavefileRanks("D:\\StockFile\\returnrate\\returerate.csv", statisticeInter.filterDataMeanList);
	FileTool.ReSavefileRanksEnd("D:\\StockFile\\returnrate\\returerate.csv");
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


