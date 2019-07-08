#include "stdafx.h"
#include "ARBRCRVRPSY.h"
#include "glog/logging.h"

CArBrCrVrPsy::CArBrCrVrPsy():
VRMA(10),//
AR_HighTo_Open(26),
AR_OpenTo_Low(26),
BR_HighTo_Close(26),
BR_CloseTo_Low(26),
VR_AVS(26),
VR_BVS(26),
CR_HighToMid(26),
CR_MidToLow(26),
PSYMA(24),
LimitDownTimesMA(200),
LimitUpTimesMA(200),
CR_MA1Para(5),
CR_MA2Para(10),
CR_MA3Para(20),
CRMA1(CR_MA1Para),
CRMA2(CR_MA2Para),
CRMA3(CR_MA3Para),
RefCRMA1(CR_MA1Para / 2.5 + 1),
RefCRMA2(CR_MA2Para / 2.5 + 1),
RefCRMA3(CR_MA3Para / 2.5 + 1)
{
}


CArBrCrVrPsy::~CArBrCrVrPsy()
{
}

bool CArBrCrVrPsy::GetNextArBrVrPsy(const SinCyclePriceData& OneDayData, ARBRCRVRPSY& mLastData)
{
	CurrentData = OneDayData;

	mLastData.AR = GetAR();
	mLastData.BR = GetBR();
	mLastData.VR = GetVR();
	mLastData.VRMA = VRMA.GetNextMA(mLastData.VR);
	mLastData.PSY = GetPSY();
	mLastData.LimitUpTimes = GetLimitUpTimes();
	mLastData.LimitDownTimes = GetLimitDownTimes();
	mLastData.CR = GetCR();


	mLastData.CRMa1 = RefCRMA1.GetRefData(CRMA1.GetNextMA(mLastData.CR));
	mLastData.CRMa2 = RefCRMA2.GetRefData(CRMA2.GetNextMA(mLastData.CR));
	mLastData.CRMa3 = RefCRMA3.GetRefData(CRMA3.GetNextMA(mLastData.CR));


	RefData = CurrentData;
	return true;
}


StockDataType CArBrCrVrPsy::GetAR()
{
	AR_HighTo_Open.GetNextMA(CurrentData._High - CurrentData._Open);
	AR_OpenTo_Low.GetNextMA(CurrentData._Open - CurrentData._Low);
	if (AR_OpenTo_Low.GetDataMovingSum() == 0)
		return 0;
	return (AR_HighTo_Open.GetDataMovingSum() / AR_OpenTo_Low.GetDataMovingSum() * 100);
}

StockDataType CArBrCrVrPsy::GetBR()
{
	StockDataType sum_high_pcolse = 0;
	StockDataType sum_pcolse_low = 0;
	BR_HighTo_Close.GetNextMA(max(0,CurrentData._High - RefData._Close));
	BR_CloseTo_Low.GetNextMA(max(0,RefData._Close - CurrentData._Low));
	if (BR_CloseTo_Low.GetDataMovingSum() == 0)
		return 0;

	return (BR_HighTo_Close.GetDataMovingSum() / BR_CloseTo_Low.GetDataMovingSum() * 100);
}

StockDataType CArBrCrVrPsy::GetVR()
{

	if (CurrentData._Close > RefData._Close)
		VR_AVS.GetNextMA(CurrentData._Volume);
	else
		VR_AVS.GetNextMA(0.0f);
	if (CurrentData._Close <= RefData._Close)
		VR_BVS.GetNextMA(CurrentData._Volume);
	else
		VR_BVS.GetNextMA(0.0f);
	
	if (VR_BVS.GetDataMovingSum() == 0)
		return 0;

	return (100 * VR_AVS.GetDataMovingSum() / VR_BVS.GetDataMovingSum());

}

StockDataType CArBrCrVrPsy::GetPSY()
{
	if (CurrentData._Close > RefData._Close)
		return PSYMA.GetNextMA(1.0) * 100;
	else
		return PSYMA.GetNextMA(0.0) * 100;
}

StockDataType CArBrCrVrPsy::GetCR()
{
	StockDataType MidPrice = (RefData._High + RefData._Close + RefData._Low) / 3;
	CR_HighToMid.GetNextMA(max(0,CurrentData._High - MidPrice));
	CR_MidToLow.GetNextMA(max(0, MidPrice - CurrentData._Low));

	if (CR_MidToLow.GetDataMovingSum() == 0)
		return 0.0f;
	return (100 * CR_HighToMid.GetDataMovingSum() / CR_MidToLow.GetDataMovingSum());

}

void CArBrCrVrPsy::Inition()
{
	VRMA.Inition();
	AR_HighTo_Open.Inition();
	AR_OpenTo_Low.Inition();
	BR_HighTo_Close.Inition();
	BR_CloseTo_Low.Inition();
	VR_AVS.Inition();
	VR_BVS.Inition();
	CR_HighToMid.Inition();
	CR_MidToLow.Inition();
	PSYMA.Inition();
	CRMA1.Inition();
	CRMA2.Inition();
	CRMA3.Inition();
	RefCRMA1.Inition();
	RefCRMA2.Inition();
	RefCRMA3.Inition();

	RefData.Inition();
	CurrentData.Inition();
}

StockDataType CArBrCrVrPsy::GetLimitDownTimes()
{
	if (CurrentData._Close/RefData._Close < 0.902)
		LimitDownTimesMA.GetNextMA(1);
	else
		LimitDownTimesMA.GetNextMA(0);

	return LimitDownTimesMA.GetDataMovingSum();
}

StockDataType CArBrCrVrPsy::GetLimitUpTimes()
{
	if (CurrentData._Close / RefData._Close > 1.098)
		LimitUpTimesMA.GetNextMA(1);
	else
		LimitUpTimesMA.GetNextMA(0);

	return LimitUpTimesMA.GetDataMovingSum();

}

