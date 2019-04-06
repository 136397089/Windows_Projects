#include "stdafx.h"
#include <time.h>  
#include<Windows.h>
#include "log.h"
#include "glog/logging.h"
static fstream LogFile;
static unsigned int OpenNumber = 0;
static double beginTime = 0;
#pragma comment(lib,"libglog.lib")

CLog::CLog()
{
	++OpenNumber;
	try{
		if (1 == OpenNumber)
		{
			CreateDirectory("D:\\StockFile\\Log", NULL);
			google::InitGoogleLogging("CreatAllParameter.exe");
			google::SetLogDestination(google::GLOG_INFO, "D:\\StockFile\\Log\\stockLog");
			google::SetLogDestination(google::GLOG_WARNING, "D:\\StockFile\\Log\\WarningLog");
			google::SetLogDestination(google::GLOG_ERROR, "D:\\StockFile\\ErrorLog");
			google::SetLogDestination(google::GLOG_FATAL, "D:\\StockFile\\FatalLog");
		}
	}
	catch (exception)
	{
		exit(99);
	}
}

CLog::~CLog()
{
	--OpenNumber;
	if (0 == OpenNumber)
		google::ShutdownGoogleLogging();
}



fstream& CLog::get()
{
	if (!LogFile)
	{
		try
		{
			LogFile.open("D:\\StockFile\\LogFileResult.txt", fstream::out);
		}
		catch (exception)
		{
			LOG(FATAL) << "Open log file error.";
		}
	}
	return LogFile;
}
