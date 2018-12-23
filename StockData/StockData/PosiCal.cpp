#include "PosiCal.h"
#include <assert.h>
CPosiAnalyzer* _TheMethod = NULL;

/***********************************************************************/
/***********************************************************************/
CPosiAnalyzer::CPosiAnalyzer()
{
}


CPosiAnalyzer::~CPosiAnalyzer()
{
}

std::string CPosiAnalyzer::GetLastError()
{
	return _LastError;
}



/***********************************************************************/
/***********************************************************************/


CPosiMethod1::CPosiMethod1():
CofListSize(16){
}

CPosiMethod1::~CPosiMethod1()
{
}

double CPosiMethod1::AnalysisPosition(list<OneDayVariable> dayVariableList, Coefficients coefData)
{
	if (coefData.size() != 16)
	{
		_LastError = "coefData Size Error.";
		return 0.0;
	}
	if (dayVariableList.size() < 2)
	{
		_LastError = "dayVariableList size is zero.";
		return 0.0;
	}

	list<OneDayVariable>::reverse_iterator lastIte = dayVariableList.rbegin();
	OneDayVariable lastData = *lastIte;
	lastIte++;
	OneDayVariable frontData = *lastIte;
	double diffDiff = lastData.fMACD_Diff - frontData.fMACD_Diff;
	double BARDiff = lastData.fMACD_BAR - frontData.fMACD_BAR;
	int i = 0;
	if (lastData.fMACD_Diff > 0 && lastData.fMACD_BAR > 0)
	{
		i = 0;
	}
	if (lastData.fMACD_Diff > 0 && lastData.fMACD_BAR < 0)
	{
		i = 1;
	}
	if (lastData.fMACD_Diff < 0 && lastData.fMACD_BAR > 0)
	{
		i = 2;
	}
	if (lastData.fMACD_Diff < 0 && lastData.fMACD_BAR < 0)
	{
		i = 3;
	}
// 	if (diffDiff > 0)
// 	{
// 		i = i + 4;
// 	}
// 	else
// 	{
// 		i = i;
// 	}
// 	if (BARDiff > 0)
// 	{
// 		i = i + 8;
// 	}
// 	else
// 	{
// 		i = i;
// 	}

	return coefData[i];
}

void CPosiMethod1::Initmethod()
{

}
//
int CPosiMethod1::GetCofListSize()
{
	return CofListSize;
}


/***********************************************************************/
/***********************************************************************/
CPosiAnalyzerHandle::CPosiAnalyzerHandle(int methodIndex)
{
	if (NULL == _TheMethod)
	{
		switch (methodIndex)
		{
		case 1:
			_TheMethod = new CPosiMethod1;
			break;
		default:
			break;
		}
	}
}

CPosiAnalyzerHandle::~CPosiAnalyzerHandle()
{
// 	if (NULL != _TheMethod)
// 	{
// 		delete _TheMethod;
// 		_TheMethod = NULL;
// 	}
}

void CPosiAnalyzerHandle::Initmethod()
{
	if (NULL != _TheMethod)
	{
		_TheMethod->Initmethod();
	}
	return;
}

double CPosiAnalyzerHandle::AnalysisPosition(list<OneDayVariable> dayVariableList, Coefficients coefData)
{
	if (NULL != _TheMethod)
	{
		return _TheMethod->AnalysisPosition(dayVariableList, coefData);
	}
	else
	{
		return -1; 
	}
}

int CPosiAnalyzerHandle::GetCofListSize()
{
	if (NULL != _TheMethod)
	{
		return _TheMethod->GetCofListSize();
	}
	else
	{
		return 0;
	}
}



