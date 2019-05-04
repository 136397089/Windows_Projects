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
//���з�����ʼ��ʱ��
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
	bool IsLongitudinal  = false;//��־�����Ƿ��������
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
	//��һ�е�����open�Ļ�����Ϊ���ļ����������ڵ�һ��
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
	//ֻ���������Ǻ������е��ļ�
	if (!IsLongitudinal )
		ProcessingTransverseData(AllString);
	else
	{
		cout << "Error:Can not do.";
	}

}


bool CNumberInterface::GetDataAndNumber(const string& filename, const string& FilePath)
{
	//����ϴη������ļ�����ָ������
	mDayValue.clear();
	//���ļ��ж�����Ҫ������
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
	LOG(ERROR) << "mDayValue Data begin.";
	numbercal.GetAllNumbers(mDayValue, "day");
	LOG(ERROR) << "mDayValue Data finish.";

	LOG(ERROR) << "mWeekValue Data begin.";
	mWeekValue.clear();
	mWeekValue._strStockCode = mDayValue._strStockCode;
	LOG(ERROR) << "mWeekValue Data change begin.";
	datasizetool.DayToWeek(mDayValue, mWeekValue);
	LOG(ERROR) << "mWeekValue Data number begin.";
	numbercal.GetAllNumbers(mWeekValue,"week");
	LOG(ERROR) << "mWeekValue Data finish.";

	LOG(ERROR) << "mMonthValue Data begin.";
	mMonthValue.clear();
	mMonthValue._strStockCode = mDayValue._strStockCode;
	LOG(ERROR) << "mMonthValue Data change begin.";
	datasizetool.DayToMonth(mDayValue, mMonthValue);
	LOG(ERROR) << "mMonthValue Data number begin.";
	numbercal.GetAllNumbers(mMonthValue, "month");
	LOG(ERROR) << "mMonthValue Data finish.";

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

		if (*(iteB->begin()) == "date")//��ͷ�ǿ��ַ�����Ϊ������
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				mDayValue._vTimeDay.push_back(*iteL);
			}
			//reverse(_vTimeDay.begin(), _vTimeDay.end());//��ת˳��
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
			//reverse(plist->begin(), plist->end());//��ת˳��
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

