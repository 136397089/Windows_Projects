#include "stdafx.h"
#include "NumberInter.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "../glog/logging.h"
#include "../PriChag.h"
#include "../Numbers.h"
#include "../DataSizeSwich.h"
#include "../ComTime.h"

//所有的开盘日期，从上证指数读出的日期
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
	LOG(INFO) << filename << " Read Data begin.";
	cout  << filename << " Read Data begin."<<endl;
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString,mResourceValue);
	else
		cout << "Error:Can not do.";

	LOG(INFO) << " Read Data finish.";
	cout << " Read Data finish." << endl;
	if (!mResourceValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	mResourceValue.SetDate();
	if (filename.find(".") > 0 && filename.find(".") < 100)
	{
		mResourceValue._strStockCode = filename.substr(0, filename.find("."));
		mWeekValue._strStockCode = mResourceValue._strStockCode;
		mMonthValue._strStockCode = mResourceValue._strStockCode;
		mRealTimeValue._strStockCode = mResourceValue._strStockCode;

	}
	if (mResourceValue._strStockCode.size() != 6)
	{
		LOG(ERROR) << "Error file: " << filename;
	}

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
	LOG(ERROR) << "SH Read Data begin.";
	cout << "SH Read Data begin.";
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString,mResourceValue);
	else
		cout << "Error:Can not do.";

	LOG(INFO) << "SH Read Data finish.";
	cout << "SH Read Data finish.";
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
void CIndicatorsInterface::ProcessingTransverseData(const StringBlock& AllString, StockDataTable& mTargetValue)
{
	VStockData* plist = nullptr;
	for (StringBlock::const_iterator iteB = AllString.begin(); iteB != AllString.end(); iteB++)
	{
		if (iteB->size() < 10)
			continue;
		plist = nullptr;
		if (*(iteB->begin()) == File_Open_INDEX)
			plist = &mTargetValue._vOpen;
		if (*(iteB->begin()) == File_Close_INDEX)
			plist = &mTargetValue._vClose;
		if (*(iteB->begin()) == File_High_INDEX)
			plist = &mTargetValue._vHigh;
		if (*(iteB->begin()) == File_Low_INDEX)
			plist = &mTargetValue._vLow;
		if (*(iteB->begin()) == File_Volume_INDEX)
			plist = &mTargetValue._vVolume;

		if (*(iteB->begin()) == "date")//开头是空字符串的为日期行
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
				mTargetValue._vTimeDay.push_back(*iteL);
			continue;
		}

		if (nullptr != plist)
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
				plist->push_back((float)atof(iteL->c_str()));
		}
	}
	mTargetValue.SetDate();
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
	ResizeData(BeginDate);
	IndicatorsCal.GetAllNumbers(mResourceValue);
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
	LOG(INFO) << filename << " Read Data begin.";
	cout << filename << " Read Data begin." << endl;
	StringBlock AllString;
	RaedDateFromFile(FilePath + "\\" + filename, AllString);
	if (IsLongitudinal)
		ProcessingTransverseData(AllString,mRealTimeValue);
	else
		cout << "Error:Can not do.";

	LOG(INFO) << filename << " Read Data finish.";
	cout << filename << " Read Data finish." << endl;
	if (!mRealTimeValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	if (filename.find(".") > 0 && filename.find(".") < 100)
		mRealTimeValue._strStockCode = filename.substr(0, filename.find("."));
	//处理30分钟数据，计算指标
	if (!Cal30MinuteIndicators())
		return false;
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
	//ResizeData(BeginDate);
	if (mRealTimeValue._vDate.size() == 0)
	{
		LOG(ERROR) << "mRealTimeValue size error.";
		return false;
	}
	Indicatorscal.GetAllNumbers(mRealTimeValue);
	//保存指标当前的临时变量
	Indicatorscal.SaveTempIndicators(mRealTimeValue._strStockCode, eMinute30, mRealTimeValue._vDate.back());
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
bool CIndicatorsInterface::GetStockPriceData_RealTime(const string& stockData, CycleType dataCycle,map<RealDataIndex, SinCyclePriceData>& returnData)
{
	returnData.clear();
	RealDataIndex tempIndex;
	for (unsigned int i = 0; i < NoUpdatesTableType.size();i++)
	{
		tempIndex = NoUpdatesTableType[i];
		tempIndex.SetStockCode(stockData);
		if (CurrentData.find(tempIndex) != CurrentData.end() && dataCycle == tempIndex._cycletype)
			returnData[tempIndex] = CurrentData[tempIndex];
		else
		{
			LOG(INFO) << "Can not get data." << tempIndex._stockCode<<" " << tempIndex._date.GetDateTime();
		}
		tempIndex._date = tempIndex._date + GetCycleTime(dataCycle);;
		if (CurrentData.find(tempIndex) != CurrentData.end() && dataCycle == tempIndex._cycletype)
		{
			LOG(INFO) << "Found next time data.";
			returnData[tempIndex] = CurrentData[tempIndex];
		}
	}
	LOG(INFO) << "returnData size." << returnData.size();
	LOG(INFO) << "NoUpdatesTableType." << NoUpdatesTableType.size();
	return true;
}

bool CIndicatorsInterface::GetIndicators_Realtime(const string& _stockName, CycleType dataCycle)
{
	CComTime Timeer;
	map<RealDataIndex, SinCyclePriceData> returnData;
	mRealTimeValue.clear();
	mRealTimeValue._strStockCode = _stockName;
	GetStockPriceData_RealTime(_stockName, dataCycle, returnData);//读取当前最新数据
	if (returnData.size() <= 1)
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

CDate CIndicatorsInterface::GetCycleTime(CycleType dataCycle)
{
	CDate retDate;
	if (eMinute5 == dataCycle)
		retDate._minute = 5;
	else if (eMinute15 == dataCycle)
		retDate._minute = 15;
	else if (eMinute30 == dataCycle)
		retDate._minute = 30;
	else if (eMinute60 == dataCycle)
		retDate._hour = 1;
	else if (eMinute120 == dataCycle)
		retDate._hour = 2;
	else if (eDay == dataCycle)
		retDate._day = 1;
	else if (eWeek == dataCycle)
		retDate._day = 7;
	else if (eMonth == dataCycle)
		retDate._month = 1;


	return retDate;
}

bool CIndicatorsInterface::MappingToSH(const StockDataTable& shdata)
{
	if (mResourceValue._vCumulativeChangerate.size() == 0)
	{
		LOG(ERROR) << mResourceValue._strStockCode << " mResourceValue._vCumulativeChangerate is empty.";
		return false;
	}
	mResourceValue._vMapToMarket.clear();
	mResourceValue.GetIndexMap(shdata._vDate,IndexMap);
	StockDataType shBase = shdata._vCumulativeChangerate[IndexMap[0]];
	StockDataType selfBase = mResourceValue._vCumulativeChangerate.front();
	StockDataType shChangeRate = 0;
	StockDataType selfChangeRate = 0;
	for (unsigned int i = 0; i != mResourceValue._vTimeDay.size(); i++)
	{
		selfChangeRate = mResourceValue._vCumulativeChangerate[i];
		shChangeRate = shdata._vCumulativeChangerate[IndexMap[i]];
		mResourceValue._vMapToMarket.push_back((selfChangeRate / shChangeRate / selfBase * shBase));
	} 
// 	if (mResourceValue._vMapToMarket.back() < 1.0)
// 	{
// 		LOG(INFO) << mResourceValue._strStockCode << " bottom income stock.";
// 	}
	return true;
}


