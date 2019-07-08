#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "StateB.h"
#include "StockDataTable.h"
#include "number/_DMA.h"
#include "number/Macd.h"
#include "number/trix.h"
#include "number/KDJ.h"
#include "number/ASI.h"
#include "CDP.h"
#include "DMI.h"
#include "Ma.h"
#include "PriChag.h"
#include "ARBRCRVRPSY.h"
#include "EMV.h"
#include "Bool.h"
#include "RSICal.h"
#include "NumbersToSql.h"

class CIndicatorsCalculator
{
public:
	CIndicatorsCalculator();
	~CIndicatorsCalculator();
	//
	bool GetAllNumbers(StockDataTable& datas);
	//
	void Inition();
	//
	bool SaveTempIndicators(string& _stockName, BasisCycleType cycle, CDate _date);
	//
	bool GetTempIndicators(StockDataTable& mValue,const string& _stockName, BasisCycleType cycle, const CDate& _date);
	//
private:
	void PushBackIndex(const SigDayTechIndex& AllIndex, StockDataTable& datas);
	CGeneraIindicators hGI;
	CMacdCal hMacd;
	CDMACal hDMA;
	CTRIXCal hTRIX;
	CKDJCal hKDJ;
	CAsiCal hAsi;
	CCDPCal hCdp;
	CDMI hDmi;
	CMaCal hma;
	CEMVCal hemv;
	CBool hbool;
	CRSICal hrsi;
	CArBrCrVrPsy ArBrVrPsy;
	CNumbersToSql SQLTool;
	//½á¹ûÊä³ö
	SigDayTechIndex currentIndicators;
	string stockCode;
	BasisCycleType DataCycle;
};

#endif



