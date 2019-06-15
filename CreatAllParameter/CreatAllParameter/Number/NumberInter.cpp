#include "stdafx.h"
#include "NumberInter.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "..\glog\logging.h"
#include "../PriChag.h"
#include "../Numbers.h"
#include "../DataSizeSwich.h"
#include "../ComTime.h"

static vector<CDate> DateOfTransaction;
CIndicatorsInterface::CIndicatorsInterface()
{
	IsLongitudinal = true;//标志数据是否横向排列
}
//所有分析开始的时间
CDate BeginDate(1997,1,1);
CIndicatorsInterface::~CIndicatorsInterface()
{
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CIndicatorsInterface::RaedDateFromFile(const string& strFilePath, StringBlock& AllString)
{
	CNumberBase mCSVFileTool;
	//清除上次读入的所有指标数据
	mResourceValue.clear();

	StringList lineString;
	mCSVFileTool.ReadDataFromCSVFileRanks(strFilePath, lineString);
	AllString.clear();
	if (lineString.size() < 3)
	{
		return;
	}
	//第一列当中有open的话则认为打开文件数据日期在第一行
// 	if (lineString.begin()->find("open") < 0)
// 	{
// 		IsLongitudinal  = true;
// 	}
	StringList vSigLinedata;
	for (StringList::iterator ite = lineString.begin(); ite != lineString.end(); ite++)
	{
		CutString(*ite, ",", vSigLinedata);
		AllString.push_back(vSigLinedata);
		vSigLinedata.clear();
	}
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::GetDataAndIndicators_History(const string& filename, const string& FilePath)
{
	LOG(INFO) << "************************************" << filename << "************************************" << endl;
	//从文件中读出需要的数据
	LOG(ERROR) << "Read Data begin." ;
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString);
	else
		cout << "Error:Can not do.";

	LOG(ERROR) << "Read Data finish.";
	if (!mResourceValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	mResourceValue.SetDate();
	if (filename.find(".") > 0 && filename.find(".") < 100)
		mResourceValue._strStockCode = filename.substr(0, filename.find("."));
	//处理日数据,计算日指标
		CalResourceIndicators();
	//处理周数据，计算指标
		CalWeekIndicators();
	//处理月数据，计算指标
		CalMonthIndicators();
	return true;
}

bool CIndicatorsInterface::GetDataAndIndicators_SH(const string& filename, const string& FilePath)
{
	LOG(INFO) << "************************************" << filename << "************************************" << endl;
	//从文件中读出需要的数据
	LOG(ERROR) << "Read Data begin.";
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString);
	else
		cout << "Error:Can not do.";

	LOG(ERROR) << "Read Data finish.";
	if (!mResourceValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	mResourceValue.SetDate();
	if (filename.find(".") > 0 && filename.find(".") < 100)
		mResourceValue._strStockCode = filename.substr(0, filename.find("."));
	//处理日数据,计算日指标
	CalResourceIndicators();
	//处理周数据，计算指标
	CalWeekIndicators();
	//处理月数据，计算指标
	CalMonthIndicators();
	if (DateOfTransaction.empty())
		DateOfTransaction = mResourceValue._vDate;
	return true;
}

////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CIndicatorsInterface::ProcessingTransverseData(const StringBlock& AllString)
{
	VStockData* plist = nullptr;
	for (StringBlock::const_iterator iteB = AllString.begin(); iteB != AllString.end(); iteB++)
	{
		if (iteB->size() < 10)
			continue;
		plist = nullptr;
		if (*(iteB->begin()) == File_Open_INDEX)
			plist = &mResourceValue._vOpen;
		if (*(iteB->begin()) == File_Close_INDEX)
			plist = &mResourceValue._vClose;
		if (*(iteB->begin()) == File_High_INDEX)
			plist = &mResourceValue._vHigh;
		if (*(iteB->begin()) == File_Low_INDEX)
			plist = &mResourceValue._vLow;
		if (*(iteB->begin()) == File_Volume_INDEX)
			plist = &mResourceValue._vVolume;

		if (*(iteB->begin()) == "date")//开头是空字符串的为日期行
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
				mResourceValue._vTimeDay.push_back(*iteL);
			continue;
		}

		if (nullptr != plist)
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
				plist->push_back((float)atof(iteL->c_str()));
		}
	}
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
StockDataTable& CIndicatorsInterface::GetResourceValue()
{
	return mResourceValue;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
StockDataTable& CIndicatorsInterface::GetWeekValue()
{
	return mWeekValue;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
StockDataTable& CIndicatorsInterface::GetMonthValue()
{
	return mMonthValue;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::ResizeData(CDate beginData)
{
	unsigned int i = 0;
	if (mResourceValue._vDate.empty())
		return false;
	if (mResourceValue._vDate[0] >= beginData)
		return true;
	for (i = 0; i < mResourceValue._vDate.size(); i++)
		if (mResourceValue._vDate[i] >= beginData)
			break;

	mResourceValue = mResourceValue.NewDataByIndex(i, mResourceValue._vDate.size());
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::CalResourceIndicators()
{
	CIndicatorsCalculator IndicatorsCal;
	CDataSizeSwich datasizetool;
	LOG(INFO) << "mDayValue Data begin.";
	IndicatorsCal.GetAllNumbers(mResourceValue);
	ResizeData(BeginDate);
	IndicatorsCal.SaveTempIndicators(mResourceValue._strStockCode, eDay, mResourceValue._vDate.back());
	LOG(INFO) << "mDayValue Data finish.";
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::CalWeekIndicators()
{
	CIndicatorsCalculator Indicatorscal;
	CDataSizeSwich datasizetool;
	LOG(INFO) << "mWeekValue Data begin.";
	mWeekValue.clear();
	mWeekValue._strStockCode = mResourceValue._strStockCode;
	datasizetool.DayToWeek(mResourceValue, mWeekValue);
	Indicatorscal.Inition();
	Indicatorscal.GetAllNumbers(mWeekValue);
	LOG(INFO) << "mWeekValue Data finish.";
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::CalMonthIndicators()
{
	CIndicatorsCalculator Indicatorscal;
	CDataSizeSwich datasizetool;
	LOG(INFO) << "mMonthValue Data begin.";
	mMonthValue.clear();
	mMonthValue._strStockCode = mResourceValue._strStockCode;
	datasizetool.DayToMonth(mResourceValue, mMonthValue);
	Indicatorscal.Inition();
	Indicatorscal.GetAllNumbers(mMonthValue);
	LOG(INFO) << "mMonthValue Data finish.";
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::GetDataAndIndicatorMintue_History(const string& filename, const string& FilePath)
{
	LOG(INFO) << "************************************" << filename << "************************************" << endl;
	//从文件中读出需要的数据
	LOG(INFO) << "Read Data begin.";
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString);
	else
		cout << "Error:Can not do.";

	LOG(INFO) << "Read Data finish.";
	if (!mResourceValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	if (filename.find(".") > 0 && filename.find(".") < 100)
		mResourceValue._strStockCode = filename.substr(0, filename.find("."));
	//处理30分钟数据，计算指标
	Cal30MinuteIndicators();
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
bool CIndicatorsInterface::Cal30MinuteIndicators()
{
	CIndicatorsCalculator Indicatorscal;
	CDataSizeSwich datasizetool;
	LOG(INFO) << "mDayValue Data begin.";
	Indicatorscal.GetAllNumbers(mResourceValue);
	ResizeData(BeginDate);
	//保存指标当前的临时变量
	Indicatorscal.SaveTempIndicators(mResourceValue._strStockCode, eMinute30, mResourceValue._vDate.back());
	LOG(INFO) << "mDayValue Data finish.";
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CIndicatorsInterface::RefreshAllStockDate_RealTime()
{
	CNumbersToSql SQLTool;
	SQLTool.IniMysqlTool();
	SQLTool.GetCurrentData(CurrentData, NoUpdatesTableType);
	LOG(INFO) << "CurrentData size:" << CurrentData.size();
	LOG(INFO) << "NoUpdatesTableType size:" << NoUpdatesTableType.size();
	return;
}
//
bool CIndicatorsInterface::GetStockPriceData_RealTime(const string& stockData, map<RealDataIndex, SinCyclePriceData>& returnData)
{
	returnData.clear();
	RealDataIndex tempIndex;
	for (unsigned int i = 0; i < NoUpdatesTableType.size();i++)
	{
		tempIndex = NoUpdatesTableType[i];
		tempIndex.SetStockCode(stockData);
		if (CurrentData.find(tempIndex) != CurrentData.end())
			returnData[tempIndex] = CurrentData[tempIndex];
		else
		{
			LOG(INFO) << "Can not get data." << tempIndex._stockCode<<" " << tempIndex._date.GetDateTime();
		}
	}
	LOG(INFO) << "returnData size." << returnData.size();
	LOG(INFO) << "NoUpdatesTableType." << NoUpdatesTableType.size();
	return true;
}

bool CIndicatorsInterface::GetIndicators_Realtime(const string& _stockName, BasisCycleType dataCycle)
{
	CComTime Timeer;
	map<RealDataIndex, SinCyclePriceData> returnData;
	mRealTimeValue.clear();
	mRealTimeValue._strStockCode = _stockName;
	GetStockPriceData_RealTime(_stockName, returnData);//读取当前最新数据
	if (returnData.size() <= 2)
	{
		LOG(INFO) << "returnData size is too small. " << _stockName;
		return false;
	}

	for (map<RealDataIndex, SinCyclePriceData>::iterator ite = returnData.begin(); ite != returnData.end(); ite++)
	{
		mRealTimeValue._vOpen.push_back(ite->second._Open);
		mRealTimeValue._vClose.push_back(ite->second._Close);
		mRealTimeValue._vHigh.push_back(ite->second._High);
		mRealTimeValue._vLow.push_back(ite->second._Low);
		mRealTimeValue._vVolume.push_back(ite->second._Volume);
		mRealTimeValue._vDate.push_back(ite->first._date);
		mRealTimeValue._vTimeDay.push_back(ite->first._date.GetDateTime());
	}
	CIndicatorsCalculator IndicatorsCal;
	IndicatorsCal.Inition();
	if (DateOfTransaction.empty())
	{
		LOG(ERROR) << "DateOfTransaction is empty.";
		return false;
	}
	CDate dateTime = DateOfTransaction.back();
	dateTime._hour = 15;
	Timeer.TimeBegin();
	if (!IndicatorsCal.GetTempIndicators(mRealTimeValue, _stockName, dataCycle, dateTime))//指标计算工具，返回最后一次计算状态
	{
		LOG(ERROR) << "GetTempIndicators error.";
		return false;
	}
	Timeer.TimeEnd("GetTempIndicators:");
	IndicatorsCal.GetAllNumbers(mRealTimeValue);
	return true;
}

StockDataTable& CIndicatorsInterface::GetRealtimeValue()
{
	return mRealTimeValue;
}

CDate CIndicatorsInterface::GetLastDate()
{
	return DateOfTransaction.back();
}


