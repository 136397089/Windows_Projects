#pragma once
#include "number/commonfunction.h"
#include "StockDataTable.h"

struct MeanVar
{
	friend class CMeanVariance;
	StockDataType var;
	StockDataType mean;
	unsigned int n;
	void clear(){ mean = 0; var = 0; Fn = 0; n = 0; }
	MeanVar():mean(0), var(0),Fn(0),n(0){}
private:
	StockDataType Fn;//迭代计算方差的中间变化
};

class CMeanVariance
{
public:
	CMeanVariance();
	~CMeanVariance();
	//获得增加数据之后的平均值和方差
	bool GetNextMeanVar(StockDataType newData, MeanVar& meanVar) const;
	//获得去除数据之后的平均值和方差
	bool GetMeanVarRemoveData(StockDataType removeData, MeanVar& meanVar) const;
	//调试用的函数
	bool MeanVarDebugFunction(
		const StockDataTable& datalist,
		MeanVar& meanVar) const;
};

