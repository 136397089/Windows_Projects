#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "StateB.h"
#include "StockDataTable.h"
#include "number/_DMA.h"
#include "number/Macd.h"
#include "number/trix.h"
#include "number/KDJ.h"
#include "number/ASI.h"
#include "CDP.h"
#include "DMI.h"
#include "Ma.h"
#include "PriChag.h"
#include "ARBRCRVRPSY.h"
#include "EMV.h"
class CNumbersCalculator
{
public:
	CNumbersCalculator();
	~CNumbersCalculator();
	//
	bool GetAllNumbers(StockDataTable& datas);
	
private:
	void PushBackIndex(const SigDayTechIndex& AllIndex, StockDataTable& datas);
	//AllStockData mDayValue;

};

#endif



