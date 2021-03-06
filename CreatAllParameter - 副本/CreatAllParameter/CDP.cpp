#include "stdafx.h"
#include "CDP.h"


CCDP::CCDP()
{
}


CCDP::~CCDP()
{
}

// void CCDP::UpdateFileColumn(string strFolderPath)
// {
// 
// }
// 
// void CCDP::UpdatePathFilesRanks(string strPath)
// {
// 
// }
// 
// void CCDP::UpdateFileRanks(string strfilename)
// {
// 
// }

bool CCDP::GetNextCDP(const DatePriceData& TodayDayData, CDP& mCdp)
{
	mCdp._CDP = (TodayDayData._High + TodayDayData._Low + 2 * TodayDayData._Close) / 4;
	mCdp._AH_High = mCdp._CDP + (TodayDayData._High - TodayDayData._Low);
	mCdp._NH_NormalHigh = 2 * mCdp._CDP - TodayDayData._Low;
	mCdp._AL_Low = mCdp._CDP - (TodayDayData._High - TodayDayData._Low);
	mCdp._NL_NormalLow = 2 * mCdp._CDP - TodayDayData._High;
	return true;
}
