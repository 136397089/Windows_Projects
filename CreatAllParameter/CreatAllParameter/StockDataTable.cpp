#include "stdafx.h"
#include "StockDataTable.h"



void StockDataTable::clear()
{
	//Price
	_vOpen.clear();
	_vClose.clear();
	_vHigh.clear();
	_vLow.clear();
	_vVolume.clear();
	_vTimeDay.clear();
	//MA
	_vMa1.clear();
	_vMa2.clear();
	_vMa3.clear();
	_vMa4.clear();
	//MACD
	_vMACDValue.clear();
	_vMACDMa12.clear();
	_vMACDMa26.clear();
	_vDiff.clear();
	_vDEA.clear();
	//DMA
	_vDMAValue.clear();
	_vAMAValue.clear();
	//TRIX
	_vTRIX.clear();
	_vTRMA.clear();
	_vtr.clear();
	_vtb.clear();
	_vta.clear();
	//KDJ
	_vK.clear();
	_vD.clear();
	_vJ.clear();
	//
	_vAsi_i.clear();
	_vAsit.clear();
	//
	_vPriChaRate.clear();
	_vVolChaRate.clear();
	//CDP
	_vCDP.clear();
	_vAH_High.clear();
	_vNH_NormalHigh.clear();
	_vAL_Low.clear();
	_vNL_NormalLow.clear();
}


#define COPYVECTOR(VECTORNAME,BEGININDEX,ENDINDEX)\
		if (	ENDINDEX < BEGININDEX ||\
			BEGININDEX >= VECTORNAME.size())\
			temp.VECTORNAME = VECTORNAME;\
								else if(ENDINDEX >= VECTORNAME.size())\
			temp.VECTORNAME.insert(temp.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.end());\
								else\
			temp.VECTORNAME.insert(temp.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.begin() + ENDINDEX)
StockDataTable StockDataTable::NewDataByIndex(unsigned int beginindex, unsigned int endindex) const
{
	StockDataTable temp;

	//用宏截取新的vector
	COPYVECTOR(_vTimeDay, beginindex, endindex);
	COPYVECTOR(_vClose, beginindex, endindex);
	COPYVECTOR(_vOpen, beginindex, endindex);
	COPYVECTOR(_vHigh, beginindex, endindex);
	COPYVECTOR(_vLow, beginindex, endindex);
	COPYVECTOR(_vVolume, beginindex, endindex);
	COPYVECTOR(_vPriChaRate, beginindex, endindex);

	return temp;
}

bool StockDataTable::ChackDataSize() const
{
	if (
		_vClose.size() != _vOpen.size()
		|| _vClose.size() != _vHigh.size()
		|| _vClose.size() != _vLow.size()
		|| _vClose.size() != _vTimeDay.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}


//
StockDataPointer StockDataTable::GetAllPointer() const
{
	StockDataPointer indexPointerMap;
	//pirce
	indexPointerMap[_eFile_Open_INDEX] = &_vOpen;
	indexPointerMap[_eFile_Close_INDEX] = &_vClose;
	indexPointerMap[_eFile_High_INDEX] = &_vHigh;
	indexPointerMap[_eFile_Low_INDEX] = &_vLow;
	indexPointerMap[_eFile_Volume_INDEX] = &_vVolume;
	//change rate
	indexPointerMap[_ePRICECHANGE] = &_vPriChaRate;
	indexPointerMap[_eVOLHANGE] = &_vVolChaRate;
	//MA
	indexPointerMap[_eMA_MA5] = &_vMa1;
	indexPointerMap[_eMA_MA10] = &_vMa2;
	indexPointerMap[_eMA_MA20] = &_vMa3;
	indexPointerMap[_eMA_MA40] = &_vMa4;
	//MACD
	indexPointerMap[_eMACD_BAR] = &_vMACDValue;
	indexPointerMap[_eMACD_MA12] = &_vMACDMa12;
	indexPointerMap[_eMACD_MA26] = &_vMACDMa26;
	indexPointerMap[_eMACD_DIFF] = &_vDiff;
	indexPointerMap[_eMACD_DEA] = &_vDEA;
	//DMA
	indexPointerMap[_eDMA_D] = &_vDMAValue;
	indexPointerMap[_eDMA_A] = &_vAMAValue;
	//TRIX
	indexPointerMap[_eTRIX_TRIX] = &_vTRIX;
	indexPointerMap[_eTRIX_MA] = &_vTRMA;
	indexPointerMap[_eTRIX_VTR] = &_vtr;
	indexPointerMap[_eTRIX_VTB] = &_vtb;
	indexPointerMap[_eTRIX_VTA] = &_vta;
	//KDJ
	indexPointerMap[_eKDJ_K] = &_vK;
	indexPointerMap[_eKDJ_D] = &_vD;
	indexPointerMap[_eKDJ_J] = &_vJ;
	//ASI
	indexPointerMap[_eASI_I] = &_vAsi_i;
	indexPointerMap[_eASI_T] = &_vAsit;
	//CDP
	indexPointerMap[_eCDP_CDP] = &_vCDP;
	indexPointerMap[_eCDP_AH] = &_vAH_High;
	indexPointerMap[_eCDP_NH] = &_vNH_NormalHigh;
	indexPointerMap[_eCDP_NL] = &_vNL_NormalLow;
	indexPointerMap[_eCDP_AL] = &_vAL_Low;

	return indexPointerMap;
}
//
StockDataPointer StockDataTable::GetIndexPointer() const
{
	StockDataPointer indexPointerMap;
	//
	indexPointerMap[_eMA_MA5] = &_vMa1;
	indexPointerMap[_eMA_MA10] = &_vMa2;
	indexPointerMap[_eMA_MA20] = &_vMa3;
	indexPointerMap[_eMA_MA40] = &_vMa4;
	//
	indexPointerMap[_eMACD_BAR] = &_vMACDValue;
	indexPointerMap[_eMACD_MA12] = &_vMACDMa12;
	indexPointerMap[_eMACD_MA26] = &_vMACDMa26;
	indexPointerMap[_eMACD_DIFF] = &_vDiff;
	indexPointerMap[_eMACD_DEA] = &_vDEA;
	//
	indexPointerMap[_eDMA_D] = &_vDMAValue;
	indexPointerMap[_eDMA_A] = &_vAMAValue;
	//
	indexPointerMap[_eTRIX_TRIX] = &_vTRIX;
	indexPointerMap[_eTRIX_MA] = &_vTRMA;
	indexPointerMap[_eTRIX_VTR] = &_vtr;
	indexPointerMap[_eTRIX_VTB] = &_vtb;
	indexPointerMap[_eTRIX_VTA] = &_vta;
	//
	indexPointerMap[_eKDJ_K] = &_vK;
	indexPointerMap[_eKDJ_D] = &_vD;
	indexPointerMap[_eKDJ_J] = &_vJ;
	//
	indexPointerMap[_eASI_I] = &_vAsi_i;
	indexPointerMap[_eASI_T] = &_vAsit;

	indexPointerMap[_eCDP_CDP] = &_vCDP;
	indexPointerMap[_eCDP_AH] = &_vAH_High;
	indexPointerMap[_eCDP_NH] = &_vNH_NormalHigh;
	indexPointerMap[_eCDP_NL] = &_vNL_NormalLow;
	indexPointerMap[_eCDP_AL] = &_vAL_Low;

	return indexPointerMap;
}






