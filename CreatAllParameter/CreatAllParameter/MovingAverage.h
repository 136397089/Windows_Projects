#pragma once
#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H
#include <list>
#include "Number/NumberBase.h"

class CMovingAverage
{
public:
	CMovingAverage(unsigned int MAP);
	~CMovingAverage();

	StockDataType GetNextMA(const StockDataType newData);
	StockDataType GetDataMovingSum() const;
	void Inition();
private:
	CMovingAverage();
	unsigned int MAPara;
	StockDataType dataSum;
	list<StockDataType> _HistoryData;
};



class CREFMA
{
public:
	CREFMA(unsigned int inputPara);
	~CREFMA();
	StockDataType GetRefMA(const StockDataType newData);
	void Inition();
private:
	CREFMA();
	unsigned int MAPara;
	list<StockDataType> _HistoryData;

};



// class SMA
// {
// public:
// 	SMA(unsigned int _Count);
// 	~SMA();
// 	StockDataType GetSMA(	StockDataType _currentData,
// 		unsigned int _Count,
// 		unsigned int _ParaM);
// 
// private:
// 	SMA();
// 	StockDataType FrontData;
// };

StockDataType GetSMA(StockDataType _FrontSMA,
	StockDataType _currentData,
	unsigned int _Count,
	unsigned int _CurPara);


StockDataType GetEMA(StockDataType _FrontSMA,
	StockDataType _currentData,
	unsigned int _Count);


class CRefData
{
public:
	CRefData(unsigned int inputPara) :Para(inputPara){};
	~CRefData();
	StockDataType GetRefData(StockDataType newData);
	void Inition();
private:
	CRefData();
	unsigned int Para;
	list<StockDataType> _HistoryData;
};




#endif
