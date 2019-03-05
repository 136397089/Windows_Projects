#pragma once
#ifndef STATUSINTER_H
#define STATUSINTER_H

#include "StaticRecord.h"
#include "StaticIntegral.h"
#include "CLocalRecordTool.h"
#include "TrandRecord.h"
#include "number/NumberInter.h"


class CStatusInter
{
public:
	CStatusInter();
	~CStatusInter();

	void Inter(const AllStockData& allnumber, const string filename);

	AllStatus GetOneNumberStatus(const vector<string>& date, const VStockData& _data);

	void GetStatusByIndex(unsigned int index);
	IndexStatusMap allIndexStatus;

	StatusPointsList _priceLocalStatus;
// 	AllStatus MACDStatus;
// 	AllStatus DEAStatus;

};

#endif
