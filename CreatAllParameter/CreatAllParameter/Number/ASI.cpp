#include "stdafx.h"
#include <math.h>
#include <numeric>
#include "ASI.h"


CAsiCal::CAsiCal():
MovingSI(26),//M1
MovingASIT(10)//M2
{
}


CAsiCal::~CAsiCal()
{
}


bool CAsiCal::GetNextASI(const SinCyclePriceData& TodayData, ASI& mFrontASI)
{
	ASI tempasi = mFrontASI;
	float A = abs(TodayData._High - _YesterdayData._Close);
	float B = abs(TodayData._Low - _YesterdayData._Close);
	float C = abs(TodayData._High - _YesterdayData._Low);
	float D = abs(_YesterdayData._Close - _YesterdayData._Open);

	float R = 0;
	if (A > B && A > C)
		R = A + B / 2 + D / 4;
	else if (B > A && B > C)
		R = B + A / 2 + D / 4;
	else
		R = C + D / 4;

	float E = (TodayData._Close - _YesterdayData._Close);
	float F = (TodayData._Close - TodayData._Open);
	float G = (_YesterdayData._Close - _YesterdayData._Open);
	float X = E + F / 2 + G;//从概念上来看－－指近两日的真实涨幅加权求和
	float K = max(A, B);
	float SI = 16 * X * K / R;

	MovingSI.GetNextMA(SI);
	mFrontASI._asi = MovingSI.GetDataMovingSum();
	mFrontASI._asit = MovingASIT.GetNextMA(mFrontASI._asi);

	_YesterdayData = TodayData;
	return true;
}

bool CAsiCal::Inition()
{
	MovingSI.Inition();
	MovingASIT.Inition();
	return true;
}
