#pragma once
#ifndef TOPBUTRECORD_H
#define TOPBUTRECORD_H
#include "Number/NumberBase.h"
struct IndexPoint
{
	StockDataType Indexdata;
	unsigned int HeadDistance;
	unsigned int _TailDistance;
};
class CTopButRecord
{
public:
	CTopButRecord();
	~CTopButRecord();

	void Inition();

	void SetNewData(StockDataType newData);

	vector<IndexPoint> HighIndexs;
	vector<IndexPoint> LowIndexs;

	IndexPoint CurrentHigh;
	IndexPoint CurrentLow;

	unsigned int _Cycle;
	unsigned int _TailDistance;
};

#endif
