#include "stdafx.h"
#include <numeric>
#include "TRIX.h"


CTRIX::CTRIX()
{
	_ParameterN = 18;
	_ParameterM = 28;
	_EMADenominator = 0;
	for (int i = 1; i <= _ParameterN;i++)
	{
		_EMADenominator += i;
	}
// 	_vTrix.clear();
}


CTRIX::~CTRIX()
{
}

void CTRIX::GetNextTRIX(const DatePriceData& OneDayData, TRIX& mTrix)
{
// 	_vTrix.push_back(mTrix);
	TRIX FrontTrix;
	FrontTrix = mTrix;
	StaticSizePush(OneDayData._Close, _ParameterN, frontClose);
	mTrix._AX = GetEMA(frontClose);
	StaticSizePush(mTrix._AX, _ParameterN, frontTA);
	mTrix._BX = GetEMA(frontTA);
	StaticSizePush(mTrix._BX, _ParameterN, frontTB);
	mTrix._TR = GetEMA(frontTB);
	if (FrontTrix._TR != 0)
	{
		mTrix._TRIX = (mTrix._TR - FrontTrix._TR) * 100.0f / FrontTrix._TR;
		mTrix._TRMA = 0;
		StaticSizePush(mTrix._TRIX, _ParameterM, TrixData);
		if (TrixData.size() >= _ParameterM)
		{
			float addin10 = accumulate(TrixData.begin(), TrixData.end(), 0.0f);
			mTrix._TRMA = addin10 / _ParameterM;
		}
	}

}

float CTRIX::GetEMA(const list<tySData>& dataList)
{
	float addIn = 0;
	list<tySData>::const_iterator ite = dataList.begin();
	float i = 1;
	float iAdd = 0;
	for (list<tySData>::const_iterator ite = dataList.begin(); ite != dataList.end(); ite++)
	{
		addIn = addIn + i*(*ite);
		iAdd = iAdd + i;
		++i;
	}
	return addIn / iAdd;
}

void CTRIX::StaticSizePush(tySData mdata, float msize, list<tySData>& mList)
{
	mList.push_back(mdata);
	if (mList.size() > msize)
		mList.pop_front();
}
