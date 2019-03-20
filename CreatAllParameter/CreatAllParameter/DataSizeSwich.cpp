#include "stdafx.h"
#include "DataSizeSwich.h"


CDataSizeSwich::CDataSizeSwich()
{
}


CDataSizeSwich::~CDataSizeSwich()
{
}

bool CDataSizeSwich::DayToWeek(const StockDataTable& allstockdata, StockDataTable& returnstockdata)
{
	if (allstockdata._vTimeDay.size() == 0)
	{
		_LastError = "Date is empty.";
		return false;
	}
	if (!allstockdata.ChackDataSize())
	{
		_LastError = "data size error.";
		return false;
	}
	returnstockdata.clear();

	CDate FrontDate,BackDate;
	FrontDate.SetDay(allstockdata._vTimeDay[0]);
	BackDate.SetDay(allstockdata._vTimeDay[0]);

	tyStockData WeekOpen = allstockdata._vOpen[0];
	tyStockData WeekClose = allstockdata._vClose[0];
	tyStockData WeekHigh = allstockdata._vHigh[0];
	tyStockData WeekLow = allstockdata._vLow[0];
	tyStockData WeekVolume = allstockdata._vVolume[0];
	for (unsigned int i = 1; i < allstockdata._vTimeDay.size(); i++)
	{
		FrontDate.SetDay(allstockdata._vTimeDay[i]);
		if (FrontDate.GetFrontWeekDay(5,1)!= BackDate.GetFrontWeekDay(5,1))//如果不相等表示不是同一个星期
		{
			returnstockdata._vTimeDay.push_back((BackDate+7).GetFrontWeekDay(5, 1).GetDay());
			returnstockdata._vOpen.push_back(WeekOpen);
			returnstockdata._vHigh.push_back(WeekHigh);
			returnstockdata._vLow.push_back(WeekLow);
			returnstockdata._vClose.push_back(WeekClose);
			returnstockdata._vVolume.push_back(WeekVolume);

			WeekOpen = allstockdata._vOpen[i];
			WeekClose = allstockdata._vClose[i];
			WeekHigh = allstockdata._vHigh[i];
			WeekLow = allstockdata._vLow[i];
			WeekVolume = allstockdata._vVolume[i];

			BackDate = FrontDate;
			continue;
		}

		if (allstockdata._vHigh[i] > WeekHigh)
			WeekHigh = allstockdata._vHigh[i];
		if (allstockdata._vLow[i] < WeekLow)
			WeekLow = allstockdata._vLow[i];
		WeekVolume += allstockdata._vVolume[i];
		WeekClose = allstockdata._vClose[i];
	}

	return true;
}
//
bool CDataSizeSwich::DayToMonth(const StockDataTable& allstockdata, StockDataTable& returnstockdata)
{
	if (allstockdata._vTimeDay.size() == 0)
	{
		_LastError = "Date is empty.";
		return false;
	}
	if (allstockdata._vTimeDay.size() != allstockdata._vClose.size() ||
		allstockdata._vOpen.size() != allstockdata._vClose.size() ||
		allstockdata._vHigh.size() != allstockdata._vClose.size() ||
		allstockdata._vLow.size() != allstockdata._vClose.size())
	{
		_LastError = "data size error.";
		return false;
	}
	returnstockdata.clear();

	CDate FrontDate, BackDate,Tempdate;
	FrontDate.SetDay(allstockdata._vTimeDay[0]);
	BackDate.SetDay(allstockdata._vTimeDay[0]);

	tyStockData MonthOpen = allstockdata._vOpen[0];
	tyStockData MonthClose = allstockdata._vClose[0];
	tyStockData MonthHigh = allstockdata._vHigh[0];
	tyStockData MonthLow = allstockdata._vLow[0];
	tyStockData MonthVolume = allstockdata._vVolume[0];
	for (unsigned int i = 1; i < allstockdata._vTimeDay.size(); i++)
	{
		FrontDate.SetDay(allstockdata._vTimeDay[i]);
		if (FrontDate.GetMonth() != BackDate.GetMonth())//如果不相等表示不是同一个月份
		{
			returnstockdata._vTimeDay.push_back(Tempdate.GetDay());
			returnstockdata._vOpen.push_back(MonthOpen);
			returnstockdata._vHigh.push_back(MonthHigh);
			returnstockdata._vLow.push_back(MonthLow);
			returnstockdata._vClose.push_back(MonthClose);
			returnstockdata._vVolume.push_back(MonthVolume);

			MonthOpen = allstockdata._vOpen[i];
			MonthClose = allstockdata._vClose[i];
			MonthHigh = allstockdata._vHigh[i];
			MonthLow = allstockdata._vLow[i];
			MonthVolume = allstockdata._vVolume[i];

			BackDate = FrontDate;
			continue;
		}
		Tempdate = FrontDate;
		if (allstockdata._vHigh[i] > MonthHigh)
			MonthHigh = allstockdata._vHigh[i];
		if (allstockdata._vLow[i] < MonthLow)
			MonthLow = allstockdata._vLow[i];
		MonthVolume += allstockdata._vVolume[i];
		MonthClose = allstockdata._vClose[i];
	}

	return true;
}
