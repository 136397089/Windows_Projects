#include "stdafx.h"
#include "ARBRCRVRPSY.h"
#include "glog/logging.h"

CArBrCrVrPsy::CArBrCrVrPsy()
{
	VRParameter = 26;
	ARBRParameter = 26;
	PSYParameter = 24;
	CRParameter = 26;
	CR_Close_Weight = 1;
	CR_Open_Weight = 0;
	CR_High_Weight = 1;
	CR_Low_Weight = 1;
	CR_MA1Para = 5;
	CR_MA2Para = 10;
	CR_MA3Para = 20;
	CR_MA4Para = 40;

	CRMa1_List.clear();
	CRMa2_List.clear();
	CRMa3_List.clear();
	CRMa4_List.clear();
}


CArBrCrVrPsy::~CArBrCrVrPsy()
{
}

bool CArBrCrVrPsy::GetNextArBrVrPsy(const SinCyclePriceData& OneDayData, ARBRCRVRPSY& mLastData)
{
	TemporaryStorageData.push_front(OneDayData);
	if (TemporaryStorageData.size() > VRParameter &&
		TemporaryStorageData.size() > ARBRParameter + 1 &&
		TemporaryStorageData.size() > CRParameter + 1 &&
		TemporaryStorageData.size() > PSYParameter )
	{
		TemporaryStorageData.pop_back();
	}

	mLastData.AR = GetAR();
	mLastData.BR = GetBR();
	mLastData.VR = GetVR();
	mLastData.PSY = GetPSY();

	mLastData.CR = GetCR();
	CR_List.push_front(mLastData.CR);
	if (CR_List.size() > CR_MA1Para &&
		CR_List.size() > CR_MA2Para &&
		CR_List.size() > CR_MA3Para &&
		CR_List.size() > CR_MA4Para)
		CR_List.pop_back();

	mLastData.CRMa1 = GetCRMA(CR_MA1Para, CRMa1_List);
	mLastData.CRMa2 = GetCRMA(CR_MA2Para, CRMa2_List);
	mLastData.CRMa3 = GetCRMA(CR_MA3Para, CRMa3_List);
	mLastData.CRMa4 = GetCRMA(CR_MA4Para, CRMa4_List);

	return true;
}

StockDataType CArBrCrVrPsy::GetCRMA(unsigned int maPara, list<StockDataType>& crmalist)
{
	StockDataType Sum = 0;
	unsigned int j = 1;
	list<StockDataType>::iterator currentIte = CR_List.begin();
	while (currentIte != CR_List.end()){
		Sum += *currentIte;
		if (j >= maPara)
		{
			break;
		}
		currentIte++;
		j++;
	}
	crmalist.push_back(Sum / maPara);
	if (crmalist.size() > (maPara/2.5f+2))
		crmalist.pop_front();
	return crmalist.front();
}

StockDataType CArBrCrVrPsy::GetAR()
{
	if (TemporaryStorageData.size() == 0)
		return 0.0f;

	StockDataType sum_high_open = 0;
	StockDataType sum_open_low = 0;
	unsigned int j = 1;
	list<SinCyclePriceData>::iterator currentIte = TemporaryStorageData.begin();
	while (currentIte != TemporaryStorageData.end())
	{
		sum_high_open = sum_high_open + (currentIte->_High - currentIte->_Open);
		sum_open_low = sum_open_low + (currentIte->_Open - currentIte->_Low);

		if ( j >= ARBRParameter)
		{
			break;
		}
		currentIte++;
		j++;
	}
	if (sum_open_low == 0)
	{
		//LOG(ERROR) << "sum_open_low is 0.";
		return 0;
	}

	return sum_high_open / sum_open_low * 100;
}

StockDataType CArBrCrVrPsy::GetBR()
{
	if (TemporaryStorageData.size() <= 1)
		return 0.0f;

	StockDataType sum_high_pcolse = 0;
	StockDataType sum_pcolse_low = 0;

	unsigned int j = 1;
	list<SinCyclePriceData>::iterator currentIte = TemporaryStorageData.begin();
	list<SinCyclePriceData>::iterator lastTimeIte = TemporaryStorageData.begin();
	lastTimeIte++;
	while (lastTimeIte != TemporaryStorageData.end())
	{
		sum_high_pcolse = sum_high_pcolse + max(0.0f,(currentIte->_High - lastTimeIte->_Close));
		sum_pcolse_low = sum_pcolse_low + max(0.0f,(lastTimeIte->_Close - currentIte->_Low));

		if (j >= ARBRParameter)
		{
			break;
		}
		currentIte++;
		lastTimeIte++;
		j++;
	}
	if (sum_pcolse_low == 0)
	{
		//LOG(ERROR) << "sum_pcolse_low is 0.";
		return 0;
	}

	return sum_high_pcolse / sum_pcolse_low * 100;
}

StockDataType CArBrCrVrPsy::GetVR()
{
	if (TemporaryStorageData.size() == 0)
		return 0.0f;

	StockDataType AVS = 0;
	StockDataType BVS = 0;
	StockDataType CVS = 0;
	unsigned int j = 1;
	list<SinCyclePriceData>::iterator currentIte = TemporaryStorageData.begin();
	list<SinCyclePriceData>::iterator lastTimeIte = TemporaryStorageData.begin();
	lastTimeIte++;
	while (lastTimeIte != TemporaryStorageData.end())
	{
		if (currentIte->_Close > lastTimeIte->_Close)
			AVS = AVS + currentIte->_Volume;
		if (currentIte->_Close < lastTimeIte->_Close)
			BVS = BVS + currentIte->_Volume;
		if (currentIte->_Close == lastTimeIte->_Close)
			CVS = CVS + currentIte->_Volume;
		if (j >= VRParameter)
		{
			break;
		}
		currentIte++;
		lastTimeIte++;
		j++;
	}
	if (BVS+CVS == 0)
	{
		//LOG(ERROR) << "BV+CV is 0.";
		return 0;
	}

	return 100*(2*AVS+CVS) / (2*BVS+CVS);

}

StockDataType CArBrCrVrPsy::GetPSY()
{
	if (TemporaryStorageData.size() == 0)
		return 0.0f;

	float PSY = 0;
	unsigned int j = 1;
	list<SinCyclePriceData>::iterator currentIte = TemporaryStorageData.begin();
	list<SinCyclePriceData>::iterator lastTimeIte = TemporaryStorageData.begin();
	lastTimeIte++;
	while (lastTimeIte != TemporaryStorageData.end())
	{
		if (currentIte->_Close > lastTimeIte->_Close)
		{
			PSY++;
		}
		if (j >= PSYParameter)
		{
			break;
		}
		currentIte++;
		lastTimeIte++;
		j++;
	}

	return PSY *100.f / PSYParameter;
}

StockDataType CArBrCrVrPsy::GetCR()
{
	if (TemporaryStorageData.size() <= 1)
	{
		return 0.0f;
	}

	StockDataType PSY = 0;
	StockDataType YesterdayM = 0;
	StockDataType P1 = 0;
	StockDataType P2 = 0;

	unsigned int j = 1;
	list<SinCyclePriceData>::iterator currentIte = TemporaryStorageData.begin();
	list<SinCyclePriceData>::iterator lastTimeIte = TemporaryStorageData.begin();
	lastTimeIte++;
	while (lastTimeIte != TemporaryStorageData.end())
	{
		YesterdayM = 
			(lastTimeIte->_Close *CR_Close_Weight
			+ lastTimeIte->_High * CR_High_Weight
			+ lastTimeIte->_Low * CR_Low_Weight
			+ lastTimeIte->_Open * CR_Open_Weight)
			/ (CR_Close_Weight + CR_High_Weight + CR_Low_Weight + CR_Open_Weight);
		if (currentIte->_High > YesterdayM)
			P1 = P1 + (currentIte->_High - YesterdayM);
		if (YesterdayM > currentIte->_Low)
			P2 = P2 + (YesterdayM - currentIte->_Low);
		if (j >= CRParameter){
			break;
		}
		currentIte++;
		lastTimeIte++;
		j++;
	}
	if (P2 == 0)
		return 0;
	return P1 / P2 *100;

}

void CArBrCrVrPsy::Inition()
{

	TemporaryStorageData.clear();
	CRMa1_List.clear();
	CRMa2_List.clear();
	CRMa3_List.clear();
	CRMa4_List.clear();
	CR_List.clear();

}
