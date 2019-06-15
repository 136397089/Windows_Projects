#include "stdafx.h"
#include "StockDataTable.h"



void StockDataTable::clear()
{
	map<StockNumberType, string> indexs = GetAllNumberype();
	VStockData emptyData;
	for (map<StockNumberType, string>::iterator ite = indexs.begin(); ite != indexs.end(); ite++)
	{
		StockNumberType currentType = ite->first;
		if (_vTableAllIndex.count(ite->first) == 0)
			_vTableAllIndex[ite->first] = emptyData;
		else
			_vTableAllIndex[ite->first].clear();
	}
	_strStockCode = "";
	//Price
	_vOpen.clear();
	_vClose.clear();
	_vHigh.clear();
	_vLow.clear();
	_vVolume.clear();
	_vTimeDay.clear();
	_vDate.clear();
	//MA
// 	_vMa1.clear();
// 	_vMa2.clear();
// 	_vMa3.clear();
// 	_vMa4.clear();
	//MACD
// 	_vMACDValue.clear();
// 	_vMACDMa12.clear();
// 	_vMACDMa26.clear();
// 	_vDiff.clear();
// 	_vDEA.clear();
// 	//DMA
// 	_vDMAValue.clear();
// 	_vAMAValue.clear();
// 	//TRIX
// 	_vTRIX.clear();
// 	_vTRMA.clear();
// 	_vtr.clear();
// 	_vtb.clear();
// 	_vta.clear();
// 	//KDJ
// 	_vK.clear();
// 	_vD.clear();
// 	_vJ.clear();
	//
// 	_vAsi_i.clear();
// 	_vAsit.clear();
	//
// 	_vPriChaRate.clear();
// 	_vVolChaRate.clear();
	//CDP
// 	_vCDP.clear();
// 	_vAH_High.clear();
// 	_vNH_NormalHigh.clear();
// 	_vAL_Low.clear();
// 	_vNL_NormalLow.clear();
	//
// 	_vAR.clear();
// 	_vBR.clear();
// 	_vCR.clear();
// 	_vCRMA1.clear();
// 	_vCRMA2.clear();
// 	_vCRMA3.clear();
// 	_vCRMA4.clear();
// 	_vVR.clear();
// 	_vPSY.clear();
	//
// 	_vEMV.clear();
// 	_vEMVMA.clear();
	//
// 	_vBOOLMid.clear();
// 	_vBOOLUp.clear();
// 	_vBOOLDown.clear();
	//
// 	_vDIP.clear();
// 	_vDIN.clear();
// 	_vADX.clear();
// 	_vADXR.clear();
	//
// 	_vRSI1.clear();
// 	_vRSI2.clear();
// 	_vRSI3.clear();
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
//	COPYVECTOR(_vPriChaRate, beginindex, endindex);
	COPYVECTOR(_vDate, beginindex, endindex);

	return temp;
}

bool StockDataTable::ChackDataSize() const
{
	if (_vClose.size() != _vOpen.size()
		|| _vClose.size() != _vHigh.size()
		|| _vClose.size() != _vLow.size()
		|| _vClose.size() != _vTimeDay.size()
		|| _vClose.size() <= 10)
	{
		return false;
	}

	return true;
}


//
StockDataPointer const StockDataTable::GetAllPointerToSave()
{
	StockDataPointer indexPointerMap;
	//
	indexPointerMap[_eFile_Close_INDEX] = &_vClose;
	indexPointerMap[_eFile_Open_INDEX] = &_vOpen;
	indexPointerMap[_eFile_High_INDEX] = &_vHigh;
	indexPointerMap[_eFile_Low_INDEX] = &_vLow;
	indexPointerMap[_eFile_Volume_INDEX] = &_vVolume;

	map<StockNumberType, string> indexs = GetAllNumberype();
	for (map<StockNumberType, string>::iterator ite = indexs.begin(); ite != indexs.end(); ite++)
	{
 		indexPointerMap[ite->first] = &(_vTableAllIndex[ite->first]);
	}

	return indexPointerMap;
}
//
StockDataPointer StockDataTable::GetNumberPointer() const
{
	StockDataPointer indexPointerMap;
	//
// 	indexPointerMap[_eMA_MA1] = &_vMa1;
// 	indexPointerMap[_eMA_MA2] = &_vMa2;
// 	indexPointerMap[_eMA_MA3] = &_vMa3;
// 	indexPointerMap[_eMA_MA4] = &_vMa4;
	//
// 	indexPointerMap[_eMACD_BAR] = &_vMACDValue;
// 	indexPointerMap[_eMACD_MA12] = &_vMACDMa12;
// 	indexPointerMap[_eMACD_MA26] = &_vMACDMa26;
// 	indexPointerMap[_eMACD_DIFF] = &_vDiff;
// 	indexPointerMap[_eMACD_DEA] = &_vDEA;
	//
// 	indexPointerMap[_eDMA_D] = &_vDMAValue;
// 	indexPointerMap[_eDMA_A] = &_vAMAValue;
	//
// 	indexPointerMap[_eTRIX_TRIX] = &_vTRIX;
// 	indexPointerMap[_eTRIX_MA] = &_vTRMA;
// 	indexPointerMap[_eTRIX_VTR] = &_vtr;
// 	indexPointerMap[_eTRIX_VTB] = &_vtb;
// 	indexPointerMap[_eTRIX_VTA] = &_vta;
	//
// 	indexPointerMap[_eKDJ_K] = &_vK;
// 	indexPointerMap[_eKDJ_D] = &_vD;
// 	indexPointerMap[_eKDJ_J] = &_vJ;
// 	//
// 	indexPointerMap[_eASI_I] = &_vAsi_i;
// 	indexPointerMap[_eASI_T] = &_vAsit;
// 	//
// 	indexPointerMap[_eCDP_CDP] = &_vCDP;
// 	indexPointerMap[_eCDP_AH] = &_vAH_High;
// 	indexPointerMap[_eCDP_NH] = &_vNH_NormalHigh;
// 	indexPointerMap[_eCDP_NL] = &_vNL_NormalLow;
// 	indexPointerMap[_eCDP_AL] = &_vAL_Low;
	//
// 	indexPointerMap[_eAR] = &_vAR;
// 	indexPointerMap[_eBR] = &_vBR;
// 	indexPointerMap[_eCR] = &_vCR;
// 	indexPointerMap[_eCRMA1] = &_vCRMA1;
// 	indexPointerMap[_eCRMA2] = &_vCRMA2;
// 	indexPointerMap[_eCRMA3] = &_vCRMA3;
// 	indexPointerMap[_eCRMA4] = &_vCRMA4;
// 	indexPointerMap[_eVR] = &_vVR;
// 	indexPointerMap[_ePSY] = &_vPSY;
// 	//
// 	indexPointerMap[_eEMV] = &_vEMV;
// 	indexPointerMap[_eEMVMA] = &_vEMVMA;
// 	//
// 	indexPointerMap[_eBOOLMid] = &_vBOOLMid;
// 	indexPointerMap[_eBOOLUp] = &_vBOOLUp;
// 	indexPointerMap[_eBOOLDown] = &_vBOOLDown;
	//
// 	indexPointerMap[_eDMI_DIP] = &_vDIP;
// 	indexPointerMap[_eDMI_DIN] = &_vDIN;
// 	indexPointerMap[_eDMI_ADX] = &_vADX;
// 	indexPointerMap[_eDMI_ADXR] = &_vADXR;

	return indexPointerMap;
}

void StockDataTable::SetDate()
{
	CDate date;
	_vDate.clear();
	for (unsigned int i = 0; i < _vTimeDay.size();i++)
	{
		date.SetDay(_vTimeDay[i]);
		_vDate.push_back(date);
	}
}

unsigned int StockDataTable::GetLastTimeIndexByDate(CDate date)const
{
	if (_vDate.size() < 2)//数据太少
		return 0;
	
	for (unsigned int i = 1; i < _vDate.size();i++)
	{
		if (_vDate[i - 1] < date && date <= _vDate[i])
		{
			CDate weekdata = _vDate[i - 1];
			return i - 1;
		}
	}
	//找不到目标日期，只能返回用最开始的index
	return _vDate.size() - 1;
}

unsigned int StockDataTable::GetLastTimeIndexByDate(string strDate)const
{
	CDate tempdate;
	tempdate.SetDay(strDate);
	unsigned int index = GetLastTimeIndexByDate(tempdate);
	CDate weekdata = _vDate[index];
	return GetLastTimeIndexByDate(tempdate);
}






