#include "stdafx.h"
#include <time.h>  
#include<Windows.h>
#include "log.h"
#include "glog/logging.h"



static unsigned int ClassCLogCreateNumber = 0;

#pragma comment(lib,"libglog.lib")

CLog::CLog()
{
	++ClassCLogCreateNumber;
	try{
		if (1 == ClassCLogCreateNumber)
		{
			CreateDirectory("D:\\StockFile\\Log\\", NULL);
			CreateDirectory("D:\\StockFile\\Log\\stockLog\\", NULL);
			CreateDirectory("D:\\StockFile\\Log\\WarningLog\\", NULL);
			CreateDirectory("D:\\StockFile\\Log\\ErrorLog\\", NULL);
			CreateDirectory("D:\\StockFile\\Log\\FatalLog\\", NULL);
			google::InitGoogleLogging("CreatAllParameter.exe");
			google::SetLogDestination(google::GLOG_INFO, "D:\\StockFile\\Log\\stockLog\\");
			google::SetLogDestination(google::GLOG_WARNING, "D:\\StockFile\\Log\\WarningLog\\");
			google::SetLogDestination(google::GLOG_ERROR, "D:\\StockFile\\Log\\ErrorLog\\");
			google::SetLogDestination(google::GLOG_FATAL, "D:\\StockFile\\Log\\FatalLog\\");
		}
	}
	catch (exception)
	{
		exit(99);
	}
}
CLog::CLog(string filename)
	:logfilePath("D:\\StockFile\\log")
{
	logfilename = filename + ".csv";
}

CLog::~CLog()
{
	--ClassCLogCreateNumber;
	if (0 == ClassCLogCreateNumber)
		google::ShutdownGoogleLogging();
}




// fstream& CLog::get()
// {
// 	if (!LogFile)
// 	{
// 		try
// 		{
// 			LogFile.open("D:\\StockFile\\LogFileResult.txt", fstream::out);
// 		}
// 		catch (exception)
// 		{
// 			LOG(FATAL) << "Open log file error.";
// 		}
// 	}
// 	return LogFile;
// }
