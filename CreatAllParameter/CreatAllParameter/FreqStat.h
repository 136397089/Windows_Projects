#pragma once
#ifndef FREQSTAT_H
#define FREQSTAT_H
#include <vector>
#include <map>
#include "Number/IndexNumber.h"

//ͳ�Ƶ�һ�����飬�Ͻ硢�½硢���ֵ�Ƶ��
// struct Freq_OneGroup
// {
// 	float _downValue;
// 	float _upValue;
// 	unsigned int _freqcy;
// 	Freq_OneGroup() :_freqcy(0), _downValue(0), _upValue(0){}
// };
typedef map<float, int> changeRateStatis;

class CFreqStat
{
public:
	CFreqStat();
	~CFreqStat();


	bool Inition();

	//ͳ��vdatalist�ڲ�ͬ�����г��ֵ�Ƶ��
	bool GetGroupFrqu(const VStockData& vdatalist, const float stepsize, changeRateStatis& vfreqlist);
	//ͳ��vdatalist�ڲ�ͬ�����г��ֵ�Ƶ��,�̶����
	bool GetGroupFrqu(const VStockData& vdatalist, map<float, int>& vfreqlist);
	//�����Ӧֵ������½磬���з��鶼��0Ϊ��׼���У�
	float GetTheGroupDownValue(const float value,const float stepsize);
	//
	int GetFreqByValue(float _downValue, float _upValue);


	float _stepsize;

	//const VStockData _vdatalist;

	changeRateStatis _vfreqlist;
	//vector<Freq_OneGroup> vfreqlist;

	
};

#endif
