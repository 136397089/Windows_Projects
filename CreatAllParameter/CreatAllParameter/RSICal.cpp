#include "stdafx.h"
#include "RSICal.h"
#include <cmath>

CRSICal::CRSICal():
N1(6),
N2(12),
N3(24),
_FrontMax1(0.0f),
_FrontMax2(0.0f),
_FrontMax3(0.0f),
_FrontABS1(0.0f),
_FrontABS2(0.0f),
_FrontABS3(0.0f)
{
}


CRSICal::~CRSICal()
{
}

StockDataType CRSICal::GetSMA(StockDataType _FrontMa, StockDataType _currentData, unsigned int _Count, unsigned int _ParaM)
{
	if (_ParaM > _Count)
		return 0.0;
	return (_ParaM*_currentData + (_Count - _ParaM)*_FrontMa) / _Count;
}

void CRSICal::GetNextRSI(const SinCyclePriceData& OneDayData, RSI& _FrontRSI)
{
	StockDataType LC = 0.0f;
	if (REFPrice._Close != 0)
		LC = OneDayData._Close - REFPrice._Close;

	_FrontMax1 = GetSMA(_FrontMax1, max(LC, 0), N1, 1);
	_FrontMax2 = GetSMA(_FrontMax2, max(LC, 0), N2, 1);
	_FrontMax3 = GetSMA(_FrontMax3, max(LC, 0), N3, 1);

	_FrontABS1 = GetSMA(_FrontABS1, abs(LC), N1, 1);
	_FrontABS2 = GetSMA(_FrontABS2, abs(LC), N2, 1);
	_FrontABS3 = GetSMA(_FrontABS3, abs(LC), N3, 1);
	if (_FrontABS1 != 0 && _FrontABS2 != 0 && _FrontABS3 != 0)
	{
		_FrontRSI.RSI1 = _FrontMax1 / _FrontABS1 * 100;
		_FrontRSI.RSI2 = _FrontMax2 / _FrontABS2 * 100;
		_FrontRSI.RSI3 = _FrontMax3 / _FrontABS3 * 100;
	}
	REFPrice = OneDayData;

}

void CRSICal::Inition()
{
	REFPrice.Inition();
	_FrontMax1 = (0.0f);
	_FrontMax2 = (0.0f);
	_FrontMax3 = (0.0f);
	_FrontABS1 = (0.0f);
	_FrontABS2 = (0.0f);
	_FrontABS3 = (0.0f);

}
