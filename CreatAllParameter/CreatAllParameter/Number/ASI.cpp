#include "stdafx.h"
#include <math.h>
#include <numeric>
#include "ASI.h"


CAsi::CAsi()
{
	_asiDayPara = 26;
	_asitDayPara = 10;
}


CAsi::~CAsi()
{
}


bool CAsi::GetNextASI(const DatePriceData& OneDayData, ASI& mFrontASI)
{
	ASI tempasi = mFrontASI;
	float A = abs(OneDayData._High - _FrontDayData._Close);
	float B = abs(OneDayData._Low - _FrontDayData._Close);
	float C = abs(OneDayData._High - _FrontDayData._Low);
	float D = abs(_FrontDayData._Close - _FrontDayData._Open);

	float R = 0;
	if (A > B && A > C)
		R = A + B / 2 + D / 4;
	else if (B > A && B > C)
		R = B + A / 2 + D / 4;
	else
		R = C + D / 4;

	float E = OneDayData._Close - _FrontDayData._Close;
	float F = OneDayData._Close - OneDayData._Open;
	float G = _FrontDayData._Close - _FrontDayData._Open;
	float X = E + F / 2 + G;
	float K = max(A, B);
	float L = 3;
	float SI = 50 * X / R * K / L;

	_vSIList.push_back(SI);
	if (_vSIList.size() > _asiDayPara)
		_vSIList.pop_front();
	mFrontASI._asi = accumulate(_vSIList.begin(), _vSIList.end(), 0.0f);

	_vASIList.push_back(mFrontASI._asi);
	if (_vASIList.size() > _asitDayPara)
		_vASIList.pop_front();
	mFrontASI._asit = accumulate(_vASIList.begin(), _vASIList.end(), 0.0f)/_asitDayPara;

	_FrontDayData = OneDayData;

	return true;
}
