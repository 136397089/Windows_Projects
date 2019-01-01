// CreatAllParameter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "number/NumberAll.h"
#include "Log.h"
#include "StockAccountNum.h"
#include "MainLoop.h"



int _tmain(int argc, _TCHAR* argv[])
{
	CLog initionLog;
	CMainLoop mainloop;
	mainloop.RunLoop("D:\\StockFile\\test");
	return 0;
}

