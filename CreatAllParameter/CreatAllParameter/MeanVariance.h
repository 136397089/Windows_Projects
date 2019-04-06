#pragma once
#include "number/commonfunction.h"
#include "StockDataTable.h"
struct MeanVar
{
	friend class CMeanVariance;
	tyStockData mean;
	tyStockData var;
	void clear(){ mean = 0; var = 0; Fn = 0; };
	MeanVar() :mean(0), var(0),Fn(0){}
private:
	tyStockData Fn;//�������㷽����м�仯
};

class CMeanVariance
{
public:
	CMeanVariance();
	~CMeanVariance();

	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(const VStockData& datalist, MeanVar& meanVar);
	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(const StockDataTable& datalist, MeanVar& meanVar);
	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(const VStockData::iterator beginIte, const VStockData::iterator endIte, MeanVar& meanVar);
	//
	bool GetNextMeanVar(tyStockData newData, MeanVar& meanVar);
	int nNumber;
};

