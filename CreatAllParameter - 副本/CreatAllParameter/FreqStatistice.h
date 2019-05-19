#pragma once
#ifndef FREQSTAT_H
#define FREQSTAT_H
#include <vector>
#include <map>
#include "Number/NumberBase.h"

//ͳ�Ƶ�һ�����飬�Ͻ硢�½硢���ֵ�Ƶ��
// struct Freq_OneGroup
// {
// 	float _downValue;
// 	float _upValue;
// 	unsigned int _freqcy;
// 	Freq_OneGroup() :_freqcy(0), _downValue(0), _upValue(0){}
// };

class CFreqStatistice
{
public:
	CFreqStatistice();
	~CFreqStatistice();


	bool Inition();

	//ͳ��vdatalist�ڲ�ͬ�����г��ֵ�Ƶ��,�̶����
	bool GetGroupFrqu(const VStockData& vdatalist, FreqListType& vfreqlist) const;
// 	//ͳ��vdatalist�ڲ�ͬ�����г��ֵ�Ƶ��,�̶����
// 	bool GetGroupFrqu(const VStockData& vdatalist, map<float, int>& vfreqlist);
	//�����Ӧֵ������½磬���з��鶼��0Ϊ��׼���У�
	float GetTheGroupDownValue(const float value, const float stepsize) const;
	//
	int GetTheGroupIndex(const float value, const float stepsize) const;
	//
	int GetFreqByValue(float _downValue, float _upValue, FreqListType _vfreqlist) const;
	//
	bool PushFreqData(float _downValue, FreqListType& vfreqlist) const ;
	//
	void SetEmptydate(FreqListType& vfreqlist) const;
	float _stepsize;

	int beginGroup;
	int endGroup;

	//const VStockData _vdatalist;

	//changeRateStatis _vfreqlist;
	//vector<Freq_OneGroup> vfreqlist;

	
};

#endif
