#include "stdafx.h"
#include "Operator.h"


COperator::COperator()
{
	vInpudatas.clear();
}


COperator::~COperator()
{
}

COperatorOutputData COperator::AnalysisInterface(const COperatorInputData& inputdata)
{
	COperatorOutputData ReturnData;
	return ReturnData;
}

