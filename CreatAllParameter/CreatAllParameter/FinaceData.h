#pragma once
#ifndef FINACEDATA_H
#define FINACEDATA_H
//#include "MySQLInterFace.h"
#include <sstream>

enum FinaceType
{
	_ebasics,
	_eReport,
	_eProfit,
	_eOperation,
	_eGrowth,
	_eDebtpaying,
	_eCashflow
};


class CFinaceData
{
public:
	CFinaceData();
	~CFinaceData();

	//inline string GetTablesName(FinaceType tabletype, int year, int quarter);
};

#endif