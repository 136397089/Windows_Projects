#pragma once
#ifndef _INDEXNUMBER_H
#define _INDEXNUMBER_H
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "CommonFunction.h"
#define MAX_FILE_LINE 16384


#define File_Close_INDEX "Close"
#define File_Date_INDEX "Date"
#define File_Open_INDEX "Open"
#define File_High_INDEX "High"
#define File_Low_INDEX "Low"
#define File_Volume_INDEX "Volume"

#define MACD_MA12  "Ma12"
#define MACD_MA26  "Ma26"
#define MACD_DIFF "Diff"
#define MACD_DEA "DEA"
#define MACD_BAR "BAR"

#define KDJ_K "K"
#define KDJ_D "D"
#define KDJ_J "J"


class CDate
{
public:
	CDate(string day)
	{
	}
	CDate()
	{
	}
	int iYear;
	int iMonth;
	int iDay;
};

struct CDateData
{
public:
	CDateData():fOpen(0.0f),fClode(0.0f),fHigh(0.0f),fLow(0.0f){}
	CDate mDate;
	float fOpen;
	float fClode;
	float fHigh;
	float fLow;
	float operator [] (string index);
};

class CIndexNumber
{
public:
	CIndexNumber(void);
	~CIndexNumber(void);
	vector<double>  ReadStringFormFile(string filepath,string strTittle);
	void ReSavefileColumn(string FilePath,vector<double> vMACDValue,string tittle);
	virtual void UpdateFile(string strPath) = 0;
};



#endif
