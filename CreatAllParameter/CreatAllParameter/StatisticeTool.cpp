#include "stdafx.h"
#include "StatisticeTool.h"

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

	tyStockData _data = 0;
	for (unsigned int i = 0; i < priceChangeRate.size(); i++)
	{
		_data = priceChangeRate[i];
		if (_data > maxChangeRate.RiseRate1 && i < _statisDaySize1)
			maxChangeRate.RiseRate1 = _data;
		if (_data > maxChangeRate.RiseRate2 && i < _statisDaySize2)
			maxChangeRate.RiseRate2 = _data;
		if (_data > maxChangeRate.RiseRate3 && i < _statisDaySize3)
			maxChangeRate.RiseRate3 = _data;
		if (_data < maxChangeRate.FallRate1 && i < _statisDaySize1)
			maxChangeRate.FallRate1 = _data;
		if (_data < maxChangeRate.FallRate2 && i < _statisDaySize2)
			maxChangeRate.FallRate2 = _data;
		if (_data < maxChangeRate.FallRate3 && i < _statisDaySize3)
			maxChangeRate.FallRate3 = _data;
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
	tyStockData begindata = vdatalist[0];
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

tyStockData CStatisticeTool::GetPricePosition(const StockDataTable& _data)
{
	//������
	if (!_data.ChackDataSize())
		return -1;
	//�����ڼ�������Сֵ
	VStockData::const_iterator maxHigh = max_element(_data._vHigh.begin(), _data._vHigh.end());
	VStockData::const_iterator minLow = min_element(_data._vLow.begin(), _data._vLow.end());

	if (_data._vHigh.end() == maxHigh ||
		_data._vLow.end() == minLow ||
		*maxHigh - *minLow <= 0.00001f){
		return -1.0f;
	}
	//�����һ�����̼��ں��漸���еı���
	tyStockData positionRate = (1.0f - (_data._vClose[0] - *minLow) / (*maxHigh - *minLow));
	return positionRate;
}


// float CStatisticeTool::FristDayPricePosition(const AllStockData& newdata)
// {
// 	return 0.0f;
// }



