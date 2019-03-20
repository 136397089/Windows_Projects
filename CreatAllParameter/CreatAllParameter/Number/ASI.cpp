#include "stdafx.h"
#include <math.h>
#include <numeric>
#include "ASI.h"


CAsi::CAsi()
{
	_M1 = 26;
	_M2 = 10;
}


CAsi::~CAsi()
{
}


bool CAsi::GetNextASI(const DatePriceData& TodayData, ASI& mFrontASI)
{
	ASI tempasi = mFrontASI;
	float A = abs(TodayData._High - _YesterdayData._Close);
	float B = abs(TodayData._Low - _YesterdayData._Close);
	float C = abs(TodayData._High - _YesterdayData._Low);
	float D = abs(_YesterdayData._Close - TodayData._Open);

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
	float X = E + F / 2 + G;
	float K = max(A, B);
	float SI = 16 * X * K / R;

	_vSIList.push_back(SI);
	tyStockData fristSI = 0; 
	if (_vSIList.size() > _M1)
	{
		fristSI = *_vSIList.begin();
		_vSIList.pop_front();
	}
//	mFrontASI._asi = accumulate(_vSIList.begin(), _vSIList.end(), 0.0f);
	mFrontASI._asi = mFrontASI._asi - fristSI + SI;

	_vASIList.push_back(mFrontASI._asi);
	tyStockData fristASI = 0;
	if (_vASIList.size() > _M2)
	{
		fristASI = *_vASIList.begin();
		_vASIList.pop_front();
	}
// 	mFrontASI._asit = accumulate(_vASIList.begin(), _vASIList.end(), 0.0f) / _vASIList.size();
	mFrontASI._asit = mFrontASI._asit - (fristASI - mFrontASI._asi) / _M2;

	_YesterdayData = TodayData;
	return true;
}

bool CAsi::Inition()
{
	_vSIList.clear();
	_vASIList.clear();
	return true;
}
