#include "stdafx.h"
#include "StatisticeTool.h"
#include <cmath>
using namespace std;
CStatisticeTool::CStatisticeTool()
{
	_statisDaySize1 = 5;
	_statisDaySize2 = 10;
	_statisDaySize3 = 30;
}


CStatisticeTool::~CStatisticeTool()
{
}

bool CStatisticeTool::GetMaxChangeRates(const VStockData& priceChangeRate, StaticResults& maxChangeRate)
{
	if (priceChangeRate.size() < _statisDaySize3)
	{
		_LastError = "priceChangeResult size error.In GetMaxChangeRates";
		return false;
	}

	maxChangeRate.RiseRate1 = priceChangeRate[0];
	maxChangeRate.RiseRate2 = priceChangeRate[0];
	maxChangeRate.RiseRate3 = priceChangeRate[0];
	maxChangeRate.FallRate1 = priceChangeRate[0];
	maxChangeRate.FallRate2 = priceChangeRate[0];
	maxChangeRate.FallRate3 = priceChangeRate[0];

	StockDataType _tempdata = 0;
	for (unsigned int i = 0; i < priceChangeRate.size(); i++)
	{
		_tempdata = priceChangeRate[i];
		if (_tempdata > maxChangeRate.RiseRate1 && i < _statisDaySize1)
			maxChangeRate.RiseRate1 = _tempdata;
		if (_tempdata > maxChangeRate.RiseRate2 && i < _statisDaySize2)
			maxChangeRate.RiseRate2 = _tempdata;
		if (_tempdata > maxChangeRate.RiseRate3 && i < _statisDaySize3)
			maxChangeRate.RiseRate3 = _tempdata;
		if (_tempdata < maxChangeRate.FallRate1 && i < _statisDaySize1)
			maxChangeRate.FallRate1 = _tempdata;
		if (_tempdata < maxChangeRate.FallRate2 && i < _statisDaySize2)
			maxChangeRate.FallRate2 = _tempdata;
		if (_tempdata < maxChangeRate.FallRate3 && i < _statisDaySize3)
			maxChangeRate.FallRate3 = _tempdata;
	}

	return true;

}

bool CStatisticeTool::GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate)
{
	//�������Ĳ��������Ƿ�̫��
	if (vdatalist.size() < 3)
	{
		return false;
	}
	//��һ�����������0���޷����к��������
	StockDataType begindata = vdatalist[0];
	if (begindata < 0.001 && begindata > -0.001)
		return false;
	chanRate.clear();
	//����ÿ������ڵ�һ��ı仯��
	for (VStockData::const_iterator ite = vdatalist.cbegin(); ite != vdatalist.cend(); ite++)
	{
		chanRate.push_back((*ite - begindata) * 100 / begindata);
	}
	return true;

}

StockDataType CStatisticeTool::GetPricePosition(const StockDataTable& _inputdata)
{
	//������
	if (!_inputdata.ChackDataSize())
		return -1;
	//�����ڼ�������Сֵ
	VStockData::const_iterator maxHigh = max_element(_inputdata._vHigh.begin(), _inputdata._vHigh.end());
	VStockData::const_iterator minLow = min_element(_inputdata._vLow.begin(), _inputdata._vLow.end());

	if (_inputdata._vHigh.end() == maxHigh ||
		_inputdata._vLow.end() == minLow ||
		*maxHigh - *minLow <= 0.00001f){
		return -1.0f;
	}
	//�����һ�����̼��ں��漸���еı���
	StockDataType positionRate = (1.0f - (_inputdata._vClose[0] - *minLow) / (*maxHigh - *minLow));
	return positionRate;
}


// float CStatisticeTool::FristDayPricePosition(const AllStockData& newdata)
// {
// 	return 0.0f;
// }




