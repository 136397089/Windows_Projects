#include "stdafx.h"
#include "SearchStock.h"
#include "glog/logging.h"

CSearchStock::CSearchStock()
{
}


CSearchStock::~CSearchStock()
{
}

bool CSearchStock::Inter(
	const StockDataTable& daydata,
	const StockDataTable& weekdata,
	const StockDataTable& mounthdata,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	LOG(ERROR)<< "weekMacd:" << weekdata._vMACDValue.back() << " "
		<< "monthMacd:" << mounthdata._vMACDValue.back() << " "
		<< "weekDif:" << weekdata._vDiff.back() << " "
		<< "monthDif:" << mounthdata._vDiff.back() << " ";
	if (daydata._vD.back() < 20 &&
		daydata._vJ.back() <20 &&
		daydata._vJ.back() > daydata._vD.back() &&
		weekdata._vMACDValue.back() > 0 &&
		mounthdata._vMACDValue.back() > 0&&
		weekdata._vDiff.back() > 0 &&
		mounthdata._vDiff.back() < 0)
	{
		return true;
	}
	return false;
}
