// CreatAllParameter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "number/NumberInter.h"
#include "Log.h"
#include "StockAccountNum.h"
#include "MainLoop.h"
#include "NumbersToSql.h"


int _tmain(int argc, _TCHAR* argv[])
{
// 	CNumbersToSql tool;
// 	tool.testFunstion();
	CLog initionLog;
	CMainLoop mainloop;
	//mainloop.AnaHistory("D:\\StockFile\\test");
	mainloop.AnaHistory("D:\\StockFile\\StockData_D_Current");
	//mainloop.AnaCurrentRealTimeData("D:\\StockFile\\StockData_30");

	return 0;
}

