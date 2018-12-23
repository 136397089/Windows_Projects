#ifndef _MACD_H
#define _MACD_H

#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "IndexNumber.h"
#include "CommonFunction.h"


using namespace std;


//MACD
struct Macd
{
public:
	Macd():m12(0.0f),m26(0.0f),diff(0.0f),dea(0.0f),bar(0.0f){}
	float m12;
	float m26;
	float diff;
	float dea;
	float bar;
};


//
class CMacdCalculater : public CIndexNumber
{
public:
	CMacdCalculater(void);
	~CMacdCalculater(void);
// 	vector<string> CutString(string inputstring,string CutIndex);
	Macd GetNextMacd(float closeData, Macd mMacd);
	virtual void UpdateFile(string strPath);
};


#endif
