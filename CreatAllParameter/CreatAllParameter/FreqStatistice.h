#pragma once
#ifndef FREQSTAT_H
#define FREQSTAT_H
#include <vector>
#include <map>
#include "Number/NumberBase.h"


class CFreqStatistice
{
public:
	CFreqStatistice();
	CFreqStatistice(float step,int begin,int end);
	~CFreqStatistice();
	//初始化
	bool Inition();
	//统计vdatalist在不同分组中出现的频次,固定组距
	bool GetGroupFrqu(const VStockData& vdatalist, FreqListType& vfreqlist) const;
	//获得分组的统计频次
	int GetFreqByValue(float _downValue, float _upValue, FreqListType _vfreqlist) const;
	//统计频次数据，增加一个数据点
	bool StaticFreqData(float _downValue, FreqListType& vfreqlist) const ;
	//
	void SetEmptydate(FreqListType& vfreqlist) const;

	//计算对应值分组的下界，所有分组都以0为基准进行；
	float GetTheGroupDownValue(const float value, const float stepsize) const;
	//通过值和步长计算值所在的分组
	int GetTheGroupIndex(const float value, const float stepsize) const;
	
	
	float _stepsize;
	int beginGroup;
	int endGroup;
};

#endif
