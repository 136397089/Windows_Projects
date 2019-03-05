#pragma once
#ifndef STATISTICEINTERFACE_H
#define STATISTICEINTERFACE_H
#include <map>
#include "FreqStat.h"
#include "number/NumberInter.h"
#include "StatusInter.h"

//��ָ֤��������MACD����Ϊ36 78��diff�ϵ�0�����������Ǹ��ʼӴ�


struct StaticResult
{
	float RiseRate;//����������ռ��
	float MaxRise;//���ǵ�������
};

class CStatisticeInter
{
public:
	CStatisticeInter();
	~CStatisticeInter();
	//
	bool Inition();
	//
	bool Inter(const AllStockData& allnumber, CStatusInter& statusinter);
	//
	bool GroupFreqStatistice(const AllStockData& allnumber, CStatusInter& statusinter);
	//
	bool MACD_EDA_Statistice(const AllStockData& allnumber, const AllStatus& StatusList);
	//
	bool Ma5_Ma30_Statistice(const AllStockData& allnumber, const StatusPointsList& StatusList);
	//unsigned int GetIndexFromTimeList(vector<string> _vTimeDay, string time1);
	//
	bool GetMaxChangeRates(const VStockData PriceChangeResult);
	string _LastError;
};




#endif
