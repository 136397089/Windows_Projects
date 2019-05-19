
#pragma once

#include "number/NumberBase.h"

class CMaCal
{
public:
	CMaCal(int M1, int M2, int M3, int M4);
	~CMaCal();
	bool GetNextMa(const SinDayPriceData& OneDayData, MA& mFrontKDJ);

	bool Inition();
private:
	list<StockDataType> closedatas;
	int M1_Par, M2_Par, M3_Par, M4_Par;
	int day;
	CMaCal();

};

