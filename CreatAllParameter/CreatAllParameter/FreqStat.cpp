#include "stdafx.h"
#include <math.h>
#include "FreqStat.h"

#define MINSTEPSIZE 0.0001
CFreqStat::CFreqStat():
_stepsize(0)
{
}


CFreqStat::~CFreqStat()
{
}

bool CFreqStat::Inition()
{
	_vfreqlist.clear();
	return true;
}

bool CFreqStat::GetGroupFrqu(const VStockData& vdatalist, const float groupsize, changeRateStatis& vfreqlist)
{
	if (groupsize <= 0 || groupsize > -MINSTEPSIZE && groupsize < MINSTEPSIZE)
		return false;
	_vfreqlist.clear();
	vfreqlist.clear();

	_stepsize = groupsize;

	for (unsigned int i = 0; i < vdatalist.size(); i++)
	{
		//������ݷ�����½磬�Ͻ�ͨ��_stepsizeȷ��
		float downValus = GetTheGroupDownValue(vdatalist[i], _stepsize);
		vfreqlist[downValus]++;
	}
	_vfreqlist = vfreqlist;
	return true;
}

// bool CFreqStat::GetGroupFrqu(const VStockData& vdatalist, map<float, int>& vfreqlist)
// {
// 	if (groupsize <= 0 || groupsize > -MINSTEPSIZE && groupsize < MINSTEPSIZE)
// 		return false;
// 	_stepsize = groupsize;
// 
// 	_vfreqlist.clear();
// 	vfreqlist.clear();
// 
// 	_minGroupValus = vdatalist[0];
// 	_maxGroupValus = vdatalist[0];
// 	for (unsigned int i = 0; i < vdatalist.size(); i++)
// 	{
// 		//������ݷ�����½磬�Ͻ�ͨ��_stepsizeȷ��
// 		float downValus = GetTheGroupDownValue(vdatalist[i], _stepsize);
// 		if (_minGroupValus > downValus)
// 			_minGroupValus = downValus;
// 		if (_maxGroupValus < downValus)
// 			_maxGroupValus = downValus;
// 		vfreqlist[downValus]++;
// 	}
// 	_vfreqlist = vfreqlist;
// 	return true;
// 
// }

//�����Ӧֵ������½磬���з��鶼��0Ϊ��׼���У�
float CFreqStat::GetTheGroupDownValue(const float value, const float stepsize)
{
	//����stepsize����Сֵ
	if (stepsize <= 0 || stepsize>-MINSTEPSIZE && stepsize < MINSTEPSIZE)
		return 0.0f;
	int groupindex = floor(value / stepsize);
	if (value < 0)
		groupindex--;

	return (stepsize * groupindex);
}


int CFreqStat::GetFreqByValue(float _downValue, float _upValue)
{
	float downKey = GetTheGroupDownValue(_downValue, _stepsize);
	float upKey = GetTheGroupDownValue(_upValue, _stepsize);
	int freq = 0;
	for (map<float, int>::iterator ite = _vfreqlist.begin(); ite != _vfreqlist.end(); ite++)
		if (ite->first >= downKey && ite->first <= upKey)
			freq += ite->second;

	return freq;
}




