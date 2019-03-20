
#pragma once

#include "number/NumberBase.h"
struct MA
{
	MA() :Ma1(0), Ma2(0), Ma3(0), Ma4(0) {}
	tyStockData Ma1;
	tyStockData Ma2;
	tyStockData Ma3;
	tyStockData Ma4;
};

class CMa
{
public:
	CMa(int M1, int M2, int M3, int M4);
	~CMa();
	bool GetNextMa(const DatePriceData& OneDayData, MA& mFrontKDJ);

	bool Inition();
private:
	list<tyStockData> closedatas;
	int M1_Par, M2_Par, M3_Par, M4_Par;
	int day;
	CMa();

};

