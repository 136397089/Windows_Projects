#ifndef _POSICAL_H
#define _POSICAL_H

#pragma once
#include "CommonStruct.h"
#include <list>
using namespace std;

class CPosiAnalyzer;
class CPosiMethod1;


class CPosiAnalyzerHandle
{
public:
	CPosiAnalyzerHandle(int methodIndex);
	virtual ~CPosiAnalyzerHandle();
	double AnalysisPosition(list<OneDayVariable> dayVariableList, Coefficients coefData);
	virtual int GetCofListSize();
	void Initmethod();
};

class CPosiAnalyzer
{
public:
	CPosiAnalyzer();
	virtual ~CPosiAnalyzer();
	virtual double AnalysisPosition(list<OneDayVariable> dayVariableList, Coefficients coefData) = 0;
	virtual int GetCofListSize() = 0;
	virtual void Initmethod() = 0 ;
	string GetLastError();
protected:
	string _LastError;
};


/***********************************************************************/
/***********************************************************************/

class CPosiMethod1 :public CPosiAnalyzer
{
public:
	CPosiMethod1();
	~CPosiMethod1();
	virtual double AnalysisPosition(list<OneDayVariable> dayVariableList, Coefficients coefData);
	//要求有多少个系数用于计算
	virtual int GetCofListSize();
	virtual void Initmethod();
private:
	int CofListSize;
};


#endif//_POSICAL_H
