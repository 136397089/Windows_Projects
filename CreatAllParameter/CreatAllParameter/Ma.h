
#pragma once

#include "number/indexnumber.h"
struct MA
{
	MA() :Ma1(0), Ma2(0), Ma3(0), Ma4(0) {}
	tySData Ma1;
	tySData Ma2;
	tySData Ma3;
	tySData Ma4;
};

class CMa
{
public:
	CMa(int M1, int M2, int M3, int M4);
	~CMa();
	bool GetNextMa(const DatePriceData& OneDayData, MA& mFrontKDJ);

	bool Inition();
private:
	list<tySData> closedatas;
	int M1_Par, M2_Par, M3_Par, M4_Par;
	int day;
	CMa();

};

