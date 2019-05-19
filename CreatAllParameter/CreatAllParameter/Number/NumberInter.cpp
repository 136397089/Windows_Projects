#include "stdafx.h"
#include "NumberInter.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "..\glog\logging.h"
#include "../PriChag.h"
#include "../Numbers.h"
#include "../DataSizeSwich.h"
CNumberInterface::CNumberInterface()
{
	//FileNumber = 0;
}
//所有分析开始的时间
CDate BeginDate(1997,1,1);

CNumberInterface::~CNumberInterface()
{
}


////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CNumberInterface::RaedDateFromFile(const string& strFilePath)
{
	bool IsLongitudinal  = false;//标志数据是否横向排列
	CNumberBase mCSVFileTool;
	//清除上次读入的所有指标数据
	mDayValue.clear();

	StringList lineString;
	mCSVFileTool.ReadDataFromCSVFileRanks(strFilePath, lineString);
	StringBlock AllString;
	if (lineString.size() < 3)
	{
		return;
	}
	//第一列当中有open的话则认为打开文件数据日期在第一行
	if (lineString.begin()->find("open") < 0)
	{
		IsLongitudinal  = true;
	}
	StringList vSigLinedata;
	for (StringList::iterator ite = lineString.begin(); ite != lineString.end(); ite++)
	{
		CutString(*ite, ",", vSigLinedata);
		AllString.push_back(vSigLinedata);
		vSigLinedata.clear();
	}
	//只处理日期是横向排列的文件
	if (!IsLongitudinal )
		ProcessingTransverseData(AllString);
	else
	{
		cout << "Error:Can not do.";
	}

}


bool CNumberInterface::GetDataAndNumbers(const string& filename, const string& FilePath)
{
	//从文件中读出需要的数据
	LOG(ERROR) << "Read Data begin." ;
	RaedDateFromFile(FilePath + "\\" + filename);
	LOG(ERROR) << "Read Data finish.";
	if (!mDayValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	LOG(INFO)
		<< "************************************"
		<< filename
		<< "************************************" << endl;
	if (filename.find(".") > 0 && filename.find(".") < 100)
		mDayValue._strStockCode = filename.substr(0, filename.find("."));
	mDayValue.SetDate();
	ResizeData(BeginDate);
	CNumbersCalculator numbercal;
	CDataSizeSwich datasizetool;
	LOG(INFO) << "mDayValue Data begin.";
	numbercal.GetAllNumbers(mDayValue, "day");
	LOG(INFO) << "mDayValue Data finish.";

	LOG(INFO) << "mWeekValue Data begin.";
	mWeekValue.clear();
	mWeekValue._strStockCode = mDayValue._strStockCode;
	LOG(INFO) << "mWeekValue Data change begin.";
	datasizetool.DayToWeek(mDayValue, mWeekValue);
	LOG(INFO) << "mWeekValue Data number begin.";
	numbercal.GetAllNumbers(mWeekValue,"week");
	LOG(INFO) << "mWeekValue Data finish.";

	LOG(INFO) << "mMonthValue Data begin.";
	mMonthValue.clear();
	mMonthValue._strStockCode = mDayValue._strStockCode;
	LOG(INFO) << "mMonthValue Data change begin.";
	datasizetool.DayToMonth(mDayValue, mMonthValue);
	LOG(INFO) << "mMonthValue Data number begin.";
	numbercal.GetAllNumbers(mMonthValue, "month");
	LOG(INFO) << "mMonthValue Data finish.";

	return true;
}




void CNumberInterface::ProcessingTransverseData(const StringBlock& AllString)
{
	VStockData* plist = nullptr;
	for (StringBlock::const_iterator iteB = AllString.begin(); iteB != AllString.end(); iteB++)
	{
		if (iteB->size() < 10)
			continue;
		plist = nullptr;
		if (*(iteB->begin()) == File_Open_INDEX)
			plist = &mDayValue._vOpen;
		if (*(iteB->begin()) == File_Close_INDEX)
			plist = &mDayValue._vClose;
		if (*(iteB->begin()) == File_High_INDEX)
			plist = &mDayValue._vHigh;
		if (*(iteB->begin()) == File_Low_INDEX)
			plist = &mDayValue._vLow;
		if (*(iteB->begin()) == File_Volume_INDEX)
			plist = &mDayValue._vVolume;
// 		if (*(iteB->begin()) == "v_ma5")
// 			plist = &mAll._vMa5;

		if (*(iteB->begin()) == "date")//开头是空字符串的为日期行
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				mDayValue._vTimeDay.push_back(*iteL);
			}
			//reverse(_vTimeDay.begin(), _vTimeDay.end());//反转顺序
			continue;
		}

		if (nullptr != plist)
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				plist->push_back((float)atof(iteL->c_str()));
			}
		}
		//if (plist != nullptr)
			//reverse(plist->begin(), plist->end());//反转顺序
	}
}


const StockDataTable& CNumberInterface::GetDayValue() const
{
	return mDayValue;
}

const StockDataTable& CNumberInterface::GetWeekValue() const
{
	return mWeekValue;
}

const StockDataTable& CNumberInterface::GetMonthValue() const
{
	return mMonthValue;
}

bool CNumberInterface::ResizeData(CDate beginData)
{
	unsigned int i = 0;
	if (mDayValue._vDate[0] >= beginData)
	{
		return true;
	}
	for (i = 0; i < mDayValue._vDate.size(); i++)
	{
		if (mDayValue._vDate[i] >= beginData)
			break;
	}
	mDayValue = mDayValue.NewDataByIndex(i, mDayValue._vDate.size());
	return true;
}

bool CNumberInterface::GetTodayData(const string& fileFullPath, map<string, SinDayData>& TodayData)
{
	TodayData.clear();
	CNumberBase mCSVFileTool;
	StringList linestring;
	mCSVFileTool.ReadDataFromCSVFileRanks(fileFullPath, linestring);
	if (linestring.size() <= 1)
	{
		LOG(ERROR) << "Today's data is empty.";
		return false;
	}
	StringList vSigColumns;
	CutString(linestring[0], ",", vSigColumns);
	if (vSigColumns.size() != 15)
	{
		LOG(ERROR) << "Columns size error.";
		return false;
	}
	StringList vSigLinedata;
	string stockCode = "";
	SinDayData tempDayData;
	for (unsigned int index = 1; index < linestring.size();index++)
	{
		vSigLinedata.clear();
		CutString(linestring[index], ",", vSigLinedata);
		stockCode = vSigLinedata[0];
		tempDayData.changepercent = (StockDataType)atof(vSigLinedata[2].c_str());
		tempDayData._Close = (StockDataType)atof(vSigLinedata[3].c_str());
		tempDayData._Open = (StockDataType)atof(vSigLinedata[4].c_str());
		tempDayData._High = (StockDataType)atof(vSigLinedata[5].c_str());
		tempDayData._Low = (StockDataType)atof(vSigLinedata[6].c_str());
		tempDayData._Volume = (StockDataType)atof(vSigLinedata[8].c_str());
		tempDayData.turnoverratio = (StockDataType)atof(vSigLinedata[9].c_str());
		tempDayData.per = (StockDataType)atof(vSigLinedata[11].c_str());
		tempDayData.pb = (StockDataType)atof(vSigLinedata[12].c_str());
		tempDayData.mktcap = (StockDataType)atof(vSigLinedata[13].c_str());
		tempDayData.nmc = (StockDataType)atof(vSigLinedata[14].c_str());
		TodayData[stockCode] = tempDayData;
	}
	return true;
}

