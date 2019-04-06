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

	mDayValue._vClose.clear();
	mDayValue._vHigh.clear();
	mDayValue._vLow.clear();
	mDayValue._vOpen.clear();
	mDayValue._vTimeDay.clear();
	mDayValue._vVolume.clear();

	StringList lineString = mCSVFileTool.ReadDataFromCSVFileRanks(strFilePath);
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
		vSigLinedata = CutString(*ite, ",");
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


bool CNumberInterface::GetDataAndNumber(const string& filename, const string& FilePath)
{
	//清除上次分析的文件所有指标数据
	mDayValue.clear();
	//从文件中读出需要的数据
	RaedDateFromFile(FilePath + "\\" + filename);
	if (!mDayValue.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	LOG(INFO)
		<< "************************************"
		<< filename
		<< "************************************" << endl;
	mDayValue.SetDate();
	ResizeData(BeginDate);
	CNumbersCalculator numbercal;
	CDataSizeSwich datasizetool;
	numbercal.GetAllNumbers(mDayValue);

	mWeekValue.clear();
	datasizetool.DayToWeek(mDayValue, mWeekValue);
	numbercal.GetAllNumbers(mWeekValue);

	mMonthValue.clear();
	datasizetool.DayToMonth(mDayValue, mMonthValue);
	numbercal.GetAllNumbers(mMonthValue);

	return true;
}




void CNumberInterface::CheckAndPrintKDJMin(const string& filename)
{
	VStockData::reverse_iterator iteKDJ1 = mDayValue._vJ.rbegin();
	VStockData::reverse_iterator iteKDJ2 = iteKDJ1 + 1;
	VStockData::reverse_iterator iteMACD1 = mDayValue._vMACDValue.rbegin();
	VStockData::reverse_iterator iteMACD2 = iteMACD1 + 1;
	VStockData::reverse_iterator iteASI1 = mDayValue._vAsi_i.rbegin();
	VStockData::reverse_iterator iteASI2 = iteASI1 + 1;
	if (*iteKDJ1 <= 0 && *iteKDJ2 <= 0)
		resultFile.get() << filename << "  KDJMin" << endl;
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


const StockDataTable& CNumberInterface::GetDayValue()
{
	return mDayValue;
}

const StockDataTable& CNumberInterface::GetWeekValue()
{
	return mWeekValue;
}

const StockDataTable& CNumberInterface::GetMonthValue()
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

