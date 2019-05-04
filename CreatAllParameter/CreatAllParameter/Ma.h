
#pragma once

#include "number/NumberBase.h"
struct MA
{
	MA() :Ma1(0), Ma2(0), Ma3(0), Ma4(0) {}
	StockDataType Ma1;
	StockDataType Ma2;
	StockDataType Ma3;
	StockDataType Ma4;
};

class CMaCal
{
public:
	CMaCal(int M1, int M2, int M3, int M4);
	~CMaCal();
	bool GetNextMa(const DatePriceData& OneDayData, MA& mFrontKDJ);

	bool Inition();
private:
	list<StockDataType> closedatas;
	int M1_Par, M2_Par, M3_Par, M4_Par;
	int day;
	CMaCal();

};

