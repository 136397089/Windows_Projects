#include "stdafx.h"
#include <numeric>
#include "TRIX.h"

CTRIXCal::CTRIXCal():
TRMA(20)//
{
	_EMAParameter = 12;
}


CTRIXCal::~CTRIXCal()
{
}

void CTRIXCal::GetNextTRIX(const SinCyclePriceData& OneDayData, TRIX& mTrix)
{
	TRIX FrontTrix;
	FrontTrix = mTrix;
	mTrix._AX = GetEMA(FrontTrix._AX, OneDayData._Close, _EMAParameter);
	mTrix._BX = GetEMA(FrontTrix._BX, mTrix._AX, _EMAParameter);
	mTrix._TR = GetEMA(FrontTrix._TR, mTrix._BX, _EMAParameter);
	if (FrontTrix._TR != 0)
	{
		mTrix._TRIX = (mTrix._TR - FrontTrix._TR) * 100.0f / FrontTrix._TR;
		mTrix._TRMA = TRMA.GetNextMA(mTrix._TRIX);
	}

}



void CTRIXCal::Inition()
{
	TRMA.Inition();
}
