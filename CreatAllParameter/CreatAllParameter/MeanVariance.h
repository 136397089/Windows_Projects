#pragma once
#include "number/commonfunction.h"
#include "StockDataTable.h"

struct MeanVar
{
	friend class CMeanVariance;
	StockDataType var;
	StockDataType mean;
	unsigned int n;
	void clear(){ mean = 0; var = 0; Fn = 0; }
	MeanVar():mean(0), var(0),Fn(0),n(0){}
private:
	StockDataType Fn;//�������㷽����м�仯
};

class CMeanVariance
{
public:
	CMeanVariance();
	~CMeanVariance();

	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(
		const VStockData& datalist,
		MeanVar& meanVar) const;
	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(
		const StockDataTable& datalist,
		MeanVar& meanVar) const;
	//ͳ��datalist�еľ�ֵ�ͷ���
	bool GetChangeRateMeanVar(
		const VStockData::iterator beginIte, 
		const VStockData::iterator endIte,
		MeanVar& meanVar) const;
	//�����������֮���ƽ��ֵ�ͷ���
	bool GetNextMeanVar(StockDataType newData, MeanVar& meanVar) const;
	//���ȥ������֮���ƽ��ֵ�ͷ���
	bool GetMeanVarRemoveData(StockDataType removeData, MeanVar& meanVar) const;
	bool MeanVarDebugFunction(
		const StockDataTable& datalist,
		MeanVar& meanVar) const;
	//int nNumber;
};

