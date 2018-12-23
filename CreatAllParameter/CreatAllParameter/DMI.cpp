#include "stdafx.h"
#include <cmath>
#include "DMI.h"

CDMI::CDMI()
{
	M1 = 12;//DI计算周期
	M2 = 2;//ADXR计算周期 
	_FrontDay = 0.0f;
	_FrontADXDay = 0.0f;
}


CDMI::~CDMI()
{
}

void CDMI::UpdateFileColumn(string strFolderPath)
{

}

void CDMI::UpdatePathFilesRanks(string strPath)
{

}

void CDMI::UpdateFileRanks(string strfilename)
{

}

bool CDMI::GetNextDMI(const DatePriceData& TodayDayData, Dmi& mDmi)
{
	mDmi._PDI = 0;
	mDmi._NDI = 0;
	mDmi._ADX = 0;
	mDmi._ADXR = 0;

	if (_FrontDay != 0)
	{
		DMTR currentDMTR;
		//1.1计算上升动向（+DM）
		if (TodayDayData._High - _FrontPriceData._Low > abs(TodayDayData._Low - _FrontPriceData._Low))
			currentDMTR.PDM = TodayDayData._High - _FrontPriceData._Low;
		else
			currentDMTR.PDM = 0;
		//1.2计算下降动向（DM）
		if (TodayDayData._Low - _FrontPriceData._Low > abs(TodayDayData._High - _FrontPriceData._Low))
			currentDMTR.NDM = TodayDayData._Low - _FrontPriceData._Low;
		else
			currentDMTR.NDM = 0;
		//1.3计算真实波幅（TR）
		tySData TR; 
		tySData A,B,C;
		A = TodayDayData._High - TodayDayData._Low;
		B = TodayDayData._High - _FrontPriceData._Close;
		C = TodayDayData._Low - _FrontPriceData._Close;
		TR = max(A, B);
		currentDMTR.TR = max(TR, C);
		//1.4计算平均DM 平均TR
		DMTR currentAvgDMTR;
		if (_FrontDay <= M1)
		{
			currentAvgDMTR.NDM = (_FrontAvgDMTR.NDM*_FrontDay + currentDMTR.NDM) / (_FrontDay + 1);
			currentAvgDMTR.PDM = (_FrontAvgDMTR.PDM*_FrontDay + currentDMTR.PDM) / (_FrontDay + 1);
			currentAvgDMTR.TR = (_FrontAvgDMTR.TR*_FrontDay + currentDMTR.TR) / (_FrontDay + 1);
		}
		else
		{
			currentAvgDMTR.NDM = (_FrontAvgDMTR.NDM* (M1-1) + currentDMTR.NDM) / M1;
			currentAvgDMTR.PDM = (_FrontAvgDMTR.PDM* (M1 - 1) + currentDMTR.PDM) / M1;
			currentAvgDMTR.TR = (_FrontAvgDMTR.TR* (M1 - 1) + currentDMTR.TR) / M1;
		}
		mDmi._PDI = (currentAvgDMTR.PDM / currentDMTR.TR) * 100;
		mDmi._NDI = (currentAvgDMTR.NDM / currentDMTR.TR) * 100;
		mDmi._ADX = abs(mDmi._PDI - mDmi._NDI) / (mDmi._PDI + mDmi._NDI) * 100;
		if (_FrontADXDay <= M2)
			mDmi._ADXR = (_FrontAvgDMTR.ADX*_FrontADXDay + currentDMTR.ADX) / (_FrontADXDay + 1);
		else
			mDmi._ADXR = (_FrontAvgDMTR.ADX*(_FrontADXDay + 1) + currentDMTR.ADX) * M2;
		++_FrontADXDay;
	}
	++_FrontDay;
	_FrontPriceData = TodayDayData;
// 	if (_vFrontPriceDataList.size() > M1)
// 	{
// 		_vFrontPriceDataList.pop_front();
// 	}
	return true;
}
