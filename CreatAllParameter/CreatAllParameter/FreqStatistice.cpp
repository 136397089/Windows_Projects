#include "stdafx.h"
#include <math.h>
#include "FreqStatistice.h"

#define MINSTEPSIZE 0.00001f
CFreqStatistice::CFreqStatistice():
_stepsize(0.05f),
beginGroup(-220),
endGroup(220)
{
}

CFreqStatistice::CFreqStatistice(float step, int begin, int end):
_stepsize(step),
beginGroup(begin),
endGroup(end)
{
}


CFreqStatistice::~CFreqStatistice()
{
}

bool CFreqStatistice::Inition()
{
	return true;
}

bool CFreqStatistice::GetGroupFrqu(const VStockData& vdatalist, FreqListType& vfreqlist)const
{

	if (_stepsize <= 0 || _stepsize > -MINSTEPSIZE && _stepsize < MINSTEPSIZE)
		return false;
	vfreqlist.clear();


	for (unsigned int i = 0; i < vdatalist.size(); i++)
	{
		//获得数据分组的下界，上界通过_stepsize确定
		float downValus = GetTheGroupDownValue(vdatalist[i], _stepsize);
		vfreqlist[downValus]++;
	}
	return true;
}


//计算对应值分组的下界，所有分组都以0为基准进行；
float CFreqStatistice::GetTheGroupDownValue(const float value, const float stepsize)const
{
	return (stepsize * GetTheGroupIndex(value, stepsize));
}


int CFreqStatistice::GetTheGroupIndex(const float value, const float stepsize) const
{
	//限制stepsize的最小值
	if (stepsize <= 0 || stepsize > -MINSTEPSIZE && stepsize < MINSTEPSIZE)
		return 0.0f;
	int groupindex = floor(value / stepsize);
	if (groupindex < beginGroup)
		return beginGroup;
	if (groupindex > endGroup)
		return endGroup;

	return groupindex;
}

int CFreqStatistice::GetFreqByValue(float _downValue, float _upValue, FreqListType _vfreqlist) const
{
	float downKey = GetTheGroupDownValue(_downValue, _stepsize);
	float upKey = GetTheGroupDownValue(_upValue, _stepsize);
	int freq = 0;
	for (map<float, int>::iterator ite = _vfreqlist.begin(); ite != _vfreqlist.end(); ite++)
		if (ite->first >= downKey && ite->first <= upKey)
			freq += ite->second;

	return freq;
}

bool CFreqStatistice::StaticFreqData(float _value, FreqListType& vfreqlist)const
{
	if (_stepsize <= 0 || _stepsize > -MINSTEPSIZE && _stepsize < MINSTEPSIZE)
		return false;
	float downValus = GetTheGroupDownValue(_value, _stepsize);
	vfreqlist[downValus]++;
	return true;
}

void CFreqStatistice::SetEmptydate(FreqListType& vfreqlist) const
{
	for (int i = beginGroup; i <= endGroup;i++)
	{
		if (vfreqlist.count(i*_stepsize) == 0)
			vfreqlist[i*_stepsize] = 0;
	}
}




