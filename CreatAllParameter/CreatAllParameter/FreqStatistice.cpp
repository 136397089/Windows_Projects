#include "stdafx.h"
#include <math.h>
#include "FreqStatistice.h"

#define MINSTEPSIZE 0.0001
CFreqStatistice::CFreqStatistice():
_stepsize(0)
{
}


CFreqStatistice::~CFreqStatistice()
{
}

bool CFreqStatistice::Inition()
{
	_vfreqlist.clear();
	return true;
}

bool CFreqStatistice::GetGroupFrqu(const VStockData& vdatalist, const float groupsize, changeRateStatis& vfreqlist)
{

	if (groupsize <= 0 || groupsize > -MINSTEPSIZE && groupsize < MINSTEPSIZE)
		return false;
	_vfreqlist.clear();
	vfreqlist.clear();

	_stepsize = groupsize;

	for (unsigned int i = 0; i < vdatalist.size(); i++)
	{
		//获得数据分组的下界，上界通过_stepsize确定
		float downValus = GetTheGroupDownValue(vdatalist[i], _stepsize);
		vfreqlist[downValus]++;
	}
	_vfreqlist = vfreqlist;
	return true;
}


//计算对应值分组的下界，所有分组都以0为基准进行；
float CFreqStatistice::GetTheGroupDownValue(const float value, const float stepsize)
{
	//限制stepsize的最小值
	if (stepsize <= 0 || stepsize>-MINSTEPSIZE && stepsize < MINSTEPSIZE)
		return 0.0f;
	int groupindex = floor(value / stepsize);
	if (value < 0)
		groupindex--;

	return (stepsize * groupindex);
}


int CFreqStatistice::GetFreqByValue(float _downValue, float _upValue)
{
	float downKey = GetTheGroupDownValue(_downValue, _stepsize);
	float upKey = GetTheGroupDownValue(_upValue, _stepsize);
	int freq = 0;
	for (map<float, int>::iterator ite = _vfreqlist.begin(); ite != _vfreqlist.end(); ite++)
		if (ite->first >= downKey && ite->first <= upKey)
			freq += ite->second;

	return freq;
}




