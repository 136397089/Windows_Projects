#pragma once
#include <vector>
#ifndef __OPERTOR_H
#define __OPERTOR_H 1

using namespace std;
struct COperatorInputData
{
	float openData;				//开盘数据	
	float closeData;			//收盘数据
	float nowProportion;		//当前的仓位比例
	float Relevant;				//与大盘的相关性
	COperatorInputData() :openData(0.0), closeData(0.0), nowProportion(0.0), Relevant(0){}
};

struct COperatorOutputData
{
	bool BuyOrSale;					//第二天的操作方式买入或者卖出
	float OperatorProportion;		//操作的仓位比例
	COperatorOutputData() :BuyOrSale(false), OperatorProportion(0.0){}
}; 


//操作者类
class COperator
{
public:
	COperator();
	virtual ~COperator();
	virtual COperatorOutputData AnalysisInterface(const COperatorInputData& inputdata);
private:
	vector<COperatorInputData> vInpudatas;//用于保存历史数据
};





#endif//__OPERTOR_H


