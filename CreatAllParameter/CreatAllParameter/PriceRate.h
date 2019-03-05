#pragma once
#include "Number/commonfunction.h"

class CPriceRate
{
public:
	CPriceRate();
	~CPriceRate();

	bool GetEveryDayChangeRate(const VStockData& vdatalist,VStockData& chanRate);

};

