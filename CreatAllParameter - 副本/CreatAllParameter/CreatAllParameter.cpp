// CreatAllParameter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "number/NumberInter.h"
#include "Log.h"
#include "StockAccountNum.h"
#include "MainLoop.h"



int _tmain(int argc, _TCHAR* argv[])
{
	CLog initionLog;
	CMainLoop mainloop;
	mainloop.AnaHistory("D:\\StockFile\\test");
	//mainloop.AnaHistory("D:\\StockFile\\StockData_D_Current");
	//mainloop.AnaCurrentRealTimeData("D:\\StockFile\\StockData_30");

	return 0;
}

