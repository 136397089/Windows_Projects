#include "stdafx.h"
#include "StockDataTable.h"
#include "glog/logging.h"


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
			NewDataReturn.VECTORNAME = VECTORNAME;\
								else if(ENDINDEX >= VECTORNAME.size())\
			NewDataReturn.VECTORNAME.insert(NewDataReturn.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.end());\
								else\
			NewDataReturn.VECTORNAME.insert(NewDataReturn.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.begin() + ENDINDEX)
StockDataTable StockDataTable::NewDataByIndex(unsigned int beginindex, unsigned int endindex) const
{
	StockDataTable NewDataReturn;
	NewDataReturn._strStockCode = _strStockCode;
	//用宏截取新的vector
	COPYVECTOR(_vTimeDay, beginindex, endindex);
	COPYVECTOR(_vClose, beginindex, endindex);
	COPYVECTOR(_vOpen, beginindex, endindex);
	COPYVECTOR(_vHigh, beginindex, endindex);
	COPYVECTOR(_vLow, beginindex, endindex);
	COPYVECTOR(_vVolume, beginindex, endindex);
	COPYVECTOR(_vDate, beginindex, endindex);

	return NewDataReturn;
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

unsigned int StockDataTable::GetCloselyFrontTimeIndexByDate(CDate inputdate)const
{
	//1.0处理特殊情况下的日期index
	if (_vDate.size() < 2)//数据太少
		return 0;
	if (inputdate > _vDate.back())//大于最后一天的情况
		return _vDate.size() - 1;
	if (inputdate.IsOnTheSameDay(_vDate.back()))
		return _vDate.size() - 2;
	//2.0开始查找目标日期
	unsigned int frontIndex = 0;
	unsigned int backIndex = _vDate.size() - 1;
	unsigned int midIndex = backIndex/2;
	//2.1二分法缩小范围
	while ((backIndex - frontIndex) > 5)
	{
		if (_vDate[midIndex] < inputdate && inputdate < _vDate[backIndex])
		{
			frontIndex = midIndex;
			midIndex = (midIndex + backIndex) / 2+1;
		}
		else if (_vDate[midIndex] > inputdate && inputdate > _vDate[frontIndex])
		{
			backIndex = midIndex;
			midIndex = (midIndex + frontIndex) / 2;
		}
		else if (_vDate[midIndex] == inputdate)
		{
			frontIndex = midIndex - 2;
			break;
		}
	}
	//2.2查找目标日期index
	for (unsigned int i = frontIndex; i < backIndex;i++)
		if (_vDate[i] < inputdate && _vDate[i+1] >= inputdate)
			midIndex = i;
	//3.0检查输出是否正确
	if (!(_vDate[midIndex] < inputdate && inputdate<=_vDate[midIndex+1]))
		LOG(INFO) << "GetCloselyFrontTime IndexByDate return data error. return date:" << _vDate[midIndex].GetDay() << " inputdate:" << inputdate.GetDay();

	return midIndex;
}

unsigned int StockDataTable::GetCloselyFrontTimeIndexByDate(string strDate)const
{
	CDate tempdate;
	tempdate.SetDay(strDate);
	unsigned int index = GetCloselyFrontTimeIndexByDate(tempdate);
	CDate weekdata = _vDate[index];
	return GetCloselyFrontTimeIndexByDate(tempdate);
}

void StockDataTable::GetIndexMap(const vector<CDate>& marketDate, map<unsigned int, unsigned int>& indexMap)
{
	indexMap.clear();
	unsigned int marketIndex = 0;
	CDate marketdate = marketDate[marketIndex];
	for (unsigned int index = 0; index < _vDate.size(); index++)
	{
		while (!(marketdate.IsOnTheSameDay(_vDate[index])) && marketIndex < marketDate.size())
		{
			marketdate = marketDate[marketIndex];
			if (marketdate.IsOnTheSameDay(_vDate[index]))
			{
				indexMap[index] = marketIndex;
				break;
			}
			marketIndex++;
		}
	}
}






