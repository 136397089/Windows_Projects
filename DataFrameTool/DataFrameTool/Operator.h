#pragma once
#include <vector>
#ifndef __OPERTOR_H
#define __OPERTOR_H 1

using namespace std;
struct COperatorInputData
{
	float openData;				//��������	
	float closeData;			//��������
	float nowProportion;		//��ǰ�Ĳ�λ����
	float Relevant;				//����̵������
	COperatorInputData() :openData(0.0), closeData(0.0), nowProportion(0.0), Relevant(0){}
};

struct COperatorOutputData
{
	bool BuyOrSale;					//�ڶ���Ĳ�����ʽ�����������
	float OperatorProportion;		//�����Ĳ�λ����
	COperatorOutputData() :BuyOrSale(false), OperatorProportion(0.0){}
}; 


//��������
class COperator
{
public:
	COperator();
	virtual ~COperator();
	virtual COperatorOutputData AnalysisInterface(const COperatorInputData& inputdata);
private:
	vector<COperatorInputData> vInpudatas;//���ڱ�����ʷ����
};





#endif//__OPERTOR_H


