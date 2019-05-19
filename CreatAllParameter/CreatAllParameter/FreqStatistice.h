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
	//��ʼ��
	bool Inition();
	//ͳ��vdatalist�ڲ�ͬ�����г��ֵ�Ƶ��,�̶����
	bool GetGroupFrqu(const VStockData& vdatalist, FreqListType& vfreqlist) const;
	//��÷����ͳ��Ƶ��
	int GetFreqByValue(float _downValue, float _upValue, FreqListType _vfreqlist) const;
	//ͳ��Ƶ�����ݣ�����һ�����ݵ�
	bool StaticFreqData(float _downValue, FreqListType& vfreqlist) const ;
	//
	void SetEmptydate(FreqListType& vfreqlist) const;

	//�����Ӧֵ������½磬���з��鶼��0Ϊ��׼���У�
	float GetTheGroupDownValue(const float value, const float stepsize) const;
	//ͨ��ֵ�Ͳ�������ֵ���ڵķ���
	int GetTheGroupIndex(const float value, const float stepsize) const;
	
	
	float _stepsize;
	int beginGroup;
	int endGroup;
};

#endif
