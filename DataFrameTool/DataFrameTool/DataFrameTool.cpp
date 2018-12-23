// DataFrameTool.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include "GroupByMACD.h"
#include "CommonFunction.h"
#include "CGroupDataByBAR.h"
#include "number/Macd.h"

using namespace std;

extern "C"{
	_declspec(dllexport) int ToolInterface(int select)
	{
		string information;
		ShowDebugInformationInCon("\n\n\n\n------------------------------------------------------------------");
		TO_STRING(<< "select="<< select ,information);
		ShowDebugInformationInCon(information);
		if (1 == select)
		{
			CGroupDataByBAR tempTool;//从剪切板读数据，返回BAR正负变化的分组值
			ShowDebugInformationInCon("GetGroupData will run.");
			tempTool.GetGroupData();
			return 1;
		}
		else if (2 == select)
		{
			CMacdManager macd;//生成MACD指标，保存到文件中
			ShowDebugInformationInCon("MACD will run.");
			macd.UpdatePathFilesRanks("d:\\stockdata");
			return 2;
		}
		else if (3 == select)
		{
			
		}
		else
		{
			return 299;
		}
		return 300;
	}
}


