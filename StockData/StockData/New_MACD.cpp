// New_MACD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <tchar.h> 
#include "CommonFunction.h"
#include "CMACDProg1.h"
#include "CMACDProg2.h"

using namespace std;


namespace 
{
	static const string  StockForderPath = "D:\\StockData";
}//namespace


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{

	CMACDProg2 tool;
	tool.RunInterface();
	system("pause");
	return 0;
}

