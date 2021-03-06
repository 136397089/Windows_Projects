#pragma once
#ifndef FREQSTAT_H
#define FREQSTAT_H
#include <vector>
#include <map>
#include "Number/NumberBase.h"

//统计的一个分组，上界、下界、出现的频次
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

	//统计vdatalist在不同分组中出现的频次,固定组距
	bool GetGroupFrqu(const VStockData& vdatalist, FreqListType& vfreqlist) const;
// 	//统计vdatalist在不同分组中出现的频次,固定组距
// 	bool GetGroupFrqu(const VStockData& vdatalist, map<float, int>& vfreqlist);
	//计算对应值分组的下界，所有分组都以0为基准进行；
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
