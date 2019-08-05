// CreatAllParameter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "number/NumberInter.h"
#include "Log.h"
#include "StockAccountNum.h"
#include "MainLoop.h"
#include "NumbersToSql.h"
#include "SaveResult.h"

int _tmain(int argc, _TCHAR* argv[])
{
// 	CSaveCodeList ttt;
// 	ttt.SaveCode();
	CLog initionLog;
	CMainLoop mainloop;
// 	mainloop.CreateThreadToAnaHistory_Mintue("D:\\StockFile\\test");
//  	mainloop.CreateThreadToAnaHistory_Mintue("D:\\StockFile\\StockData_D_Current");
//	mainloop.CreateThreadToAnaHistory_Mintue("D:\\StockFile\\StockData_30_Current");
// 	mainloop.CreateThreadToAnaHistory_Mintue("D:\\StockFile\\StockData_30_Current");
   	mainloop.CreateThreadToAnaHistory_Daily("D:\\StockFile\\StockData_D");
// 	mainloop.CreateThreadToAnaHistory_Daily("D:\\StockFile\\test");
// 	mainloop.AnaCurrentRealTimeData("D:\\StockFile\\whole\\", "today.csv");
// 	mainloop.AnaCurrentRealTimeData("D:\\StockFile\\StockData_30");



	return 0;
}

