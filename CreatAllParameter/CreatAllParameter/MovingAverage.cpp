#include "stdafx.h"
#include "MovingAverage.h"


CMovingAverage::CMovingAverage()
{
}

CMovingAverage::CMovingAverage(unsigned int MAP)
{
	dataSum = 0.0f;
	MAPara = MAP;
}


CMovingAverage::~CMovingAverage()
{
}

StockDataType CMovingAverage::GetNextMA(const StockDataType newData)
{
	_HistoryData.push_back(newData);
	if (_HistoryData.size() > MAPara)
	{
		dataSum -= _HistoryData.front();
		_HistoryData.pop_front();
	}
	dataSum += newData;
	return (dataSum / MAPara);
}

void CMovingAverage::Inition()
{
	_HistoryData.clear();
	dataSum = 0.0f;
}

StockDataType CMovingAverage::GetDataMovingSum() const
{
	return dataSum;
}


CREFMA::CREFMA(unsigned int inputPara)
{
	MAPara = inputPara;
}
CREFMA::CREFMA()
{
}
CREFMA::~CREFMA()
{
}

StockDataType CREFMA::GetRefMA(const StockDataType newData)
{
	_HistoryData.push_back(newData);
	StockDataType frontData = _HistoryData.front();
	if (_HistoryData.size() > MAPara)
		_HistoryData.pop_front();
	return (frontData + newData) / 2;
}

void CREFMA::Inition()
{
	_HistoryData.clear();
}


StockDataType GetSMA(StockDataType _FrontSMA, StockDataType _currentData, unsigned int _Count, unsigned int _CurPara)
{
	if (_CurPara > _Count)
		return 0.0;
	return (_CurPara * _currentData + (_Count - _CurPara) * _FrontSMA) / _Count;
}

StockDataType GetEMA(StockDataType _FrontSMA, StockDataType _currentData, unsigned int _Count)
{
	return ((2 * _currentData  + (_Count - 1) * _FrontSMA) / (_Count + 1));
}

CRefData::CRefData()
{
}

CRefData::~CRefData()
{
}

StockDataType CRefData::GetRefData(StockDataType newData)
{
	_HistoryData.push_back(newData);
	StockDataType returnData = 0.0;
	if (_HistoryData.size() > Para)
	{
		returnData = _HistoryData.front();
		_HistoryData.pop_front();
	}
	return returnData;
}

void CRefData::Inition()
{
	_HistoryData.clear();
}
