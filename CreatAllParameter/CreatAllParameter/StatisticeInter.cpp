#include "stdafx.h"
#include <numeric>
#include <complex>//复数
#include <cmath>
#include <stdlib.h>
#include <time.h> 
// #include <afxmt.h>
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "DFT.h"//快速傅立叶
#include "glog/logging.h"//Log工具
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
#include "FreqStatistice.h"
#include "NumbersToSql.h"
#include "Group.h"

CRITICAL_SECTION filterDataMeancs;

CStatisticeInter::CStatisticeInter()
{

}


CStatisticeInter::~CStatisticeInter()
{
	DeleteCriticalSection(&filterDataMeancs);
}
//
bool CStatisticeInter::Inition()
{
	InitializeCriticalSection(&filterDataMeancs);
	return true;
}
//
bool CStatisticeInter::Inter(
	StockDataTable& daydata,
	StockDataTable& weekdata,
	StockDataTable& mounthdata,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	Inition();

	vector<unsigned int> MALineResult1;
	vector<unsigned int> MALineResult2;
	MALineStatistice(daydata._vTableAllIndex[_eMA_MA1], daydata._vTableAllIndex[_eMA_MA4], MALineResult1, MALineResult2);
	return true;

	CcdpStatistics CDPStaTool;
	CDPStaTool.CountCDPData(weekdata);

	//GroupFreqStatistice(allnumber, stateinter);
	return true;
}

StockDataType CStatisticeInter::Autocorrelation(const VStockData& _inputdata, unsigned int interval)
{
	if (interval >= _inputdata.size())
	{
		return 0;
	}
	StockDataType Average = accumulate(_inputdata.begin(), _inputdata.end(), 0) / _inputdata.size();

	StockDataType Var = 0;
	for (unsigned int i = 0; i < _inputdata.size();i++)
	{
		Var = Var + (_inputdata[i] - Average)*(_inputdata[i] - Average);
	}

// 	for (unsigned int i = interval; i < _inputdata.size(); i++)
// 	{
// 
// 	}
	return 0;
}
//对均线_madata1在均线_madata2之上和之下的时长进行统计
//返回的结果放在_UpCloseresult，_DownCloseresult当中
void CStatisticeInter::MALineStatistice(
	const VStockData& _madata1,
	const VStockData& _madata2,
	vector<unsigned int>& _UpCloseresult,
	vector<unsigned int>& _DownCloseresult)
{
	//
	_UpCloseresult.clear();
	_DownCloseresult.clear();
	if (_madata1.size() != _madata2.size() && _madata1.size() < DATAMINSIZE)
	{
		return;
	}
	unsigned int biggercount = 0;
	unsigned int lesscount = 0;
	VStockData::const_iterator iteMA1 = _madata1.begin();
	VStockData::const_iterator iteMA2 = _madata2.begin();
	while (iteMA1 != _madata1.end()){
		if (*iteMA1 >= *iteMA2){
			biggercount++;
			if (lesscount > 0)
				_UpCloseresult.push_back(lesscount);
			lesscount = 0;
		}
		else{
			lesscount++;
			if (biggercount > 0)
				_DownCloseresult.push_back(biggercount);
			biggercount = 0;
		}
		iteMA1++;
		iteMA2++;
	}
	//对不同的时长进行分组统计
	unsigned int groupsize = 2;
	map<unsigned int, unsigned int> _Upresult2;
	map<unsigned int, unsigned int> _Downresult2;
	_Upresult2.clear();
	_Downresult2.clear();
	for (size_t i = 0; i < 240; i++)
	{
		_Upresult2[i*groupsize] = 0;
		_Downresult2[i*groupsize] = 0;
	}
	GroupStatistice(_UpCloseresult, groupsize, _Upresult2);
	GroupStatistice(_DownCloseresult, groupsize, _Downresult2);
	return;
}



bool CStatisticeInter::SaveMeanVarList_CS(
	const string& Stockcode,
	const MeanVar& _lastMeanVar,
	const MeanVar& _allDataMeanVar)
{
	EnterCriticalSection(&filterDataMeancs);
	CFreqStatistice freqTool;
	filterDataVarList[Stockcode] = _lastMeanVar.var;
	filterDataMeanList[Stockcode] = _lastMeanVar.mean;
	allDataVarList[Stockcode] = _allDataMeanVar.var;
	allDataMeanList[Stockcode] = _allDataMeanVar.mean;
	freqTool.SetEmptydate(_FreqList);
	LeaveCriticalSection(&filterDataMeancs);
	return true;
}

// bool CStatisticeInter::GroupDabug(
// 	StockDataTable& daynumber,
// 	StockDataTable& weeknumber,
// 	StockDataTable& mounthnumber,
// 	StockDataTable& shnumber,
// 	CStateInter& daystate,
// 	CStateInter& weekstate,
// 	CStateInter& monthstate)
// {
// 	StockDataTable& AnaNumber = daynumber;
// 
// 	if (!AnaNumber.ChackDataSize())
// 	{
// 		return false;
// 	}
// 	DayPrice oneDayPrice;//用于暂存日价格数据
// 	CHistoryGroup GroupTool;
// 	CProcessGrouping PGroupTool;
// 	PGroupTool.Inition(AnaNumber._strStockCode);
// 	NumberEvent tempNumberEvent;
// 	vector<NumberEvent> _numberdata;
// 	map<Group, MeanVar> meanVarMiddle;
// 	unsigned int shIndex = 0;
// 	unsigned int mInx = 0;
// 	for (unsigned int index = 80; index < AnaNumber._vTimeDay.size(); index++)
// 	{
// 		srand((unsigned)time(NULL));
// 		GetOneDayPrice(oneDayPrice, index, AnaNumber);
// 		if (oneDayPrice._closeData / oneDayPrice._frontClose >= 1.098
// 			|| oneDayPrice._closeData / oneDayPrice._frontClose <= 0.902)
// 			continue;//排除当天涨停和跌停的情况
// 		mInx = mounthnumber.GetLastTimeIndexByDate(AnaNumber._vDate[index]);
// 		_numberdata.clear();
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayBar, AnaNumber._vTableAllIndex[_eMACD_BAR][index - 1] - AnaNumber._vTableAllIndex[_eMACD_BAR][index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDEA, AnaNumber._vTableAllIndex[_eMACD_DEA][index - 1] - AnaNumber._vTableAllIndex[_eMACD_DEA][index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDIFF, AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 1] - AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 2]));
// 		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayKDJ_K, AnaNumber._vTableAllIndex[_eKDJ_K][index - 1]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayKDJ_D, AnaNumber._vTableAllIndex[_eKDJ_D][index - 1]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 1]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 1] - AnaNumber._vTableAllIndex[_eRSI1][index - 2]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisFroDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayVR, AnaNumber._vTableAllIndex[_eVR][index - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayCTO, oneDayPrice._frontClose-oneDayPrice._frontOpen));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayCR, AnaNumber._vTableAllIndex[_eCRMA1][index - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayCRMA, AnaNumber._vTableAllIndex[_eCRMA1][index - 1] - AnaNumber._vTableAllIndex[_eCRMA1][index - 2]));
// 
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayKDJ_D, AnaNumber._vTableAllIndex[_eKDJ_D][index - 1] - AnaNumber._vTableAllIndex[_eKDJ_D][index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayEMV, AnaNumber._vTableAllIndex[_eEMV][index - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayDIFF, AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 1]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA2, AnaNumber._vMa2[index - 1] - AnaNumber._vMa3[index - 2]));
// // 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA3, AnaNumber._vMa3[index - 1] - AnaNumber._vMa4[index - 2]));
// 
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthDIFF, mounthnumber._vTableAllIndex[_eMACD_DIFF][mInx] - mounthnumber._vTableAllIndex[_eMACD_DIFF][mInx - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthDEA, mounthnumber._vTableAllIndex[_eMACD_DEA][mInx] - mounthnumber._vTableAllIndex[_eMACD_DEA][mInx - 1]));
// 		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthKDJ_K, mounthnumber._vK[mInx] - mounthnumber._vK[mInx - 1]));
// 
// 		shIndex = shnumber.GetLastTimeIndexByDate(AnaNumber._vTimeDay[index]);
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayBAR, shnumber._vTableAllIndex[_eMACD_BAR][shIndex] - shnumber._vTableAllIndex[_eMACD_BAR][shIndex - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_D, shnumber._vTableAllIndex[_eKDJ_D][shIndex] - shnumber._vTableAllIndex[_eKDJ_D][shIndex - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayDIFF, shnumber._vTableAllIndex[_eMACD_DIFF][shIndex] - shnumber._vTableAllIndex[_eMACD_DIFF][shIndex - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayDEA, shnumber._vTableAllIndex[_eMACD_DEA][shIndex] - shnumber._vTableAllIndex[_eMACD_DEA][shIndex - 1]));
// 		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_K, shnumber._vK[shIndex] - shnumber._vK[shIndex - 1]));
// 		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_D, shnumber._vD[shIndex] - shnumber._vD[shIndex - 1]));
// 		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDeDayKDJ_K, shnumber._vK[shIndex] + shnumber._vK[shIndex - 2] - 2 * shnumber._vK[shIndex - 1] + 5));
// 		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayCR, shnumber._vCR[shIndex] - shnumber._vCR[shIndex - 1] - 20));
// 		if (index > 0 && index <= (AnaNumber._vTimeDay.size() / 2)){
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup1);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup1);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup1);
// 		}
// 		if (index > (AnaNumber._vTimeDay.size() / 4) && index <= (AnaNumber._vTimeDay.size() / 2))	{
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup2);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup2);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup2);
// 		}
// 		if (index > (AnaNumber._vTimeDay.size() / 2) && index <= (AnaNumber._vTimeDay.size() * 3/4))	{
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup3);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup3);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup3);
// 		}
// 		if (index > (AnaNumber._vTimeDay.size() * 3 / 4))	{
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup4);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup4);
// 			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup4);
// 		}
// 		PGroupTool.RecordDataFreq(_numberdata, oneDayPrice, _eGroup1);
// 		GroupTool.RecordDay(_numberdata, oneDayPrice._frontdate);
// 	}
// 	PGroupTool.CalReturnRate();
// 	return true;
// 
// 	Group keyGroup;
// 	for (unsigned int i = 0; i < GroupTool.vkeyGroupType.size();i++)
// 	{
// 		keyGroup = GroupTool.vkeyGroupType[i];
// 		keyGroup.changeRateType = _eGroup1;
// 		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
// 		keyGroup.changeRateType = _eGroup2;
// 		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
// 		keyGroup.changeRateType = _eGroup3;
// 		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
// 		keyGroup.changeRateType = _eGroup4;
// 		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
// 		keyGroup.changeRateType = _eMixed;
// 		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
// 	}
// 	return true;
// }

bool CStatisticeInter::ReturnStatistics(CIndicatorsInterface& daynumber, StockDataTable& shnumber)
{
	StockDataTable& AnaNumber = daynumber.GetResourceValue();
	StockDataTable& mounthnumber = daynumber.GetMonthValue();

	if (!AnaNumber.ChackDataSize())
	{
		return false;
	}
	DayPrice oneDayPrice;//用于暂存日价格数据
	CHistoryGroup GroupTool;
	CProcessGrouping PGroupTool;
	PGroupTool.Inition(AnaNumber._strStockCode);
	NumberEvent tempNumberEvent;
	vector<NumberEvent> _numberdata;
	map<Group, MeanVar> meanVarMiddle;
	unsigned int shIndex = 0;
	unsigned int mInx = 0;
	for (unsigned int index = 80; index < AnaNumber._vTimeDay.size(); index++)
	{
		srand((unsigned)time(NULL));
		GetOneDayPrice(oneDayPrice, index, AnaNumber);
		if (oneDayPrice._closeData / oneDayPrice._frontClose >= 1.098
			|| oneDayPrice._closeData / oneDayPrice._frontClose <= 0.902)
			continue;//排除当天涨停和跌停的情况
		mInx = mounthnumber.GetLastTimeIndexByDate(AnaNumber._vDate[index]);
		_numberdata.clear();
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayBar, AnaNumber._vTableAllIndex[_eMACD_BAR][index - 1] - AnaNumber._vTableAllIndex[_eMACD_BAR][index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDEA, AnaNumber._vTableAllIndex[_eMACD_DEA][index - 1] - AnaNumber._vTableAllIndex[_eMACD_DEA][index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDIFF, AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 1] - AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 2]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayKDJ_K, AnaNumber._vTableAllIndex[_eKDJ_K][index - 1]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayKDJ_D, AnaNumber._vTableAllIndex[_eKDJ_D][index - 1]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 1]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 1] - AnaNumber._vTableAllIndex[_eRSI1][index - 2]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisFroDayRSI1, AnaNumber._vTableAllIndex[_eRSI1][index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayVR, AnaNumber._vTableAllIndex[_eVR][index - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayCTO, oneDayPrice._frontClose - oneDayPrice._frontOpen));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayCR, AnaNumber._vTableAllIndex[_eCRMA1][index - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayCRMA, AnaNumber._vTableAllIndex[_eCRMA1][index - 1] - AnaNumber._vTableAllIndex[_eCRMA1][index - 2]));

		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayKDJ_D, AnaNumber._vTableAllIndex[_eKDJ_D][index - 1] - AnaNumber._vTableAllIndex[_eKDJ_D][index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayEMV, AnaNumber._vTableAllIndex[_eEMV][index - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayDIFF, AnaNumber._vTableAllIndex[_eMACD_DIFF][index - 1]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA2, AnaNumber._vMa2[index - 1] - AnaNumber._vMa3[index - 2]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA3, AnaNumber._vMa3[index - 1] - AnaNumber._vMa4[index - 2]));

		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthDIFF, mounthnumber._vTableAllIndex[_eMACD_DIFF][mInx] - mounthnumber._vTableAllIndex[_eMACD_DIFF][mInx - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthDEA, mounthnumber._vTableAllIndex[_eMACD_DEA][mInx] - mounthnumber._vTableAllIndex[_eMACD_DEA][mInx - 1]));
		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthKDJ_K, mounthnumber._vK[mInx] - mounthnumber._vK[mInx - 1]));

		shIndex = shnumber.GetLastTimeIndexByDate(AnaNumber._vTimeDay[index]);
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayBAR, shnumber._vTableAllIndex[_eMACD_BAR][shIndex] - shnumber._vTableAllIndex[_eMACD_BAR][shIndex - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_D, shnumber._vTableAllIndex[_eKDJ_D][shIndex] - shnumber._vTableAllIndex[_eKDJ_D][shIndex - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayDIFF, shnumber._vTableAllIndex[_eMACD_DIFF][shIndex] - shnumber._vTableAllIndex[_eMACD_DIFF][shIndex - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayDEA, shnumber._vTableAllIndex[_eMACD_DEA][shIndex] - shnumber._vTableAllIndex[_eMACD_DEA][shIndex - 1]));
		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_K, shnumber._vK[shIndex] - shnumber._vK[shIndex - 1]));
		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_D, shnumber._vD[shIndex] - shnumber._vD[shIndex - 1]));
		// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDeDayKDJ_K, shnumber._vK[shIndex] + shnumber._vK[shIndex - 2] - 2 * shnumber._vK[shIndex - 1] + 5));
		//_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayCR, shnumber._vCR[shIndex] - shnumber._vCR[shIndex - 1] - 20));
		if (index > 0 && index <= (AnaNumber._vTimeDay.size() / 2)){
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup1);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup1);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup1);
		}
		if (index > (AnaNumber._vTimeDay.size() / 4) && index <= (AnaNumber._vTimeDay.size() / 2))	{
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup2);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup2);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup2);
		}
		if (index > (AnaNumber._vTimeDay.size() / 2) && index <= (AnaNumber._vTimeDay.size() * 3 / 4))	{
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup3);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup3);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup3);
		}
		if (index > (AnaNumber._vTimeDay.size() * 3 / 4))	{
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup4);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup4);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup4);
		}
		PGroupTool.RecordDataFreq(_numberdata, oneDayPrice, _eGroup1);
		GroupTool.RecordDay(_numberdata, oneDayPrice._frontdate);
	}
	PGroupTool.CalReturnRate();
	return true;

	Group keyGroup;
	for (unsigned int i = 0; i < GroupTool.vkeyGroupType.size(); i++)
	{
		keyGroup = GroupTool.vkeyGroupType[i];
		keyGroup.changeRateType = _eGroup1;
		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
		keyGroup.changeRateType = _eGroup2;
		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
		keyGroup.changeRateType = _eGroup3;
		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
		keyGroup.changeRateType = _eGroup4;
		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
		keyGroup.changeRateType = _eMixed;
		SaveOneGroupFreqData(AnaNumber._strStockCode, keyGroup, GroupTool);
	}
	return true;
}

bool CStatisticeInter::SaveOneGroupFreqData(const string& stockCoed,Group _GroupType, CHistoryGroup& GroupTool)
{
	vector<unsigned int> FreqAndCountData;
	MeanVar groupMeanVar;
	GroupTool.GetNumberFreqData(_GroupType, FreqAndCountData);//获得统计的频次数据,其中前三个数据表示分组类型
	GroupTool.GetGroupMeanVar(_GroupType, groupMeanVar);//获得统计的均值和方差
	vector<float> meanVarData;
	FreqAndCountData.push_back(groupMeanVar.n);
	meanVarData.push_back(groupMeanVar.mean);
	meanVarData.push_back(groupMeanVar.var);

	return true;
}

void CStatisticeInter::GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& AnaNumber) const 
{
	oneDayPrice._openData = AnaNumber._vOpen[index];
	oneDayPrice._closeData = AnaNumber._vClose[index];
	oneDayPrice._highData = AnaNumber._vHigh[index];
	oneDayPrice._lowData = AnaNumber._vLow[index];
	oneDayPrice._date.SetDay(AnaNumber._vTimeDay[index]);

	oneDayPrice._frontOpen = AnaNumber._vOpen[index - 1];
	oneDayPrice._frontClose = AnaNumber._vClose[index - 1];
	oneDayPrice._frontHigh = AnaNumber._vHigh[index - 1];
	oneDayPrice._frontLow = AnaNumber._vLow[index - 1];
	oneDayPrice._frontdate.SetDay(AnaNumber._vTimeDay[index-1]);
}

bool CStatisticeInter::GetMeanFromSQL(string columns, unsigned int Numtype)
{
	MySQLInterFace SQLTool;
	SQLTool.Inition();

	string sqlIns = "show tables;";
	vector<string> tables;
	SQLTool.GetTableList(tables);



	return true;
}

StockDataType CStatisticeInter::GetRandData(int _databegin, int _dataend)
{
	return 0.0;
	return (rand() % (_dataend - _databegin-1)) + _databegin + (rand() / double(RAND_MAX));
}



void CheckCurrentStockDataAndGetName(
	StockDataTable& AnaNumber,
	StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode)
{
	if (monthIndex != 0 && Currentindex >= AnaNumber._vTimeDay.size() - 1)
	{
		if ((AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1] < 0.00f))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((2 * AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] - AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 2] > 0))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 1] < 0.00))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((2 * AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 1] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex] - AnaNumber._vTableAllIndex[_eKDJ_K][Currentindex - 2] > 0.00f))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((mounthnumber._vTableAllIndex[_eMACD_DIFF][monthIndex] - mounthnumber._vTableAllIndex[_eMACD_DIFF][monthIndex - 1] < 0))
			LOG(WARNING) << StockCode << " Month diff failure to meet conditions.";
		else if ((allDataMeanVar.mean < 0.00f))
			LOG(WARNING) << StockCode << " Day mean failure to meet conditions.";
		else
		{
			LOG(INFO) << " Get stock name:    " << StockCode;
			LOG(INFO) << " Data: " << AnaNumber._vTimeDay[Currentindex] << " inedx: " << Currentindex;
			LOG(INFO) << " MACD DIF: " << AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex] << " ," << AnaNumber._vTableAllIndex[_eMACD_DIFF][Currentindex - 1];
		}
	}
}




CRealTimeAna::CRealTimeAna()
{
}

CRealTimeAna::~CRealTimeAna()
{
}



bool CRealTimeAna::AnalysisRealTimeData(StockDataTable& Indicators)
{
	vector<NumberEvent> _numberdata;
	NumberEvent tempNumberEvent;
	realTimeDataToAna RealTimeData;
	vector<StockDataType>& macdData = Indicators._vTableAllIndex[_eMACD_BAR];
	vector<StockDataType>& DEAData = Indicators._vTableAllIndex[_eMACD_DEA];
	vector<StockDataType>& DiffData = Indicators._vTableAllIndex[_eMACD_DIFF];
	vector<StockDataType>& KDJ_KData = Indicators._vTableAllIndex[_eKDJ_K];//K要求小于20
	vector<StockDataType>& KDJ_DData = Indicators._vTableAllIndex[_eKDJ_D];//K要求上升
	vector<string>& timeData = Indicators._vTimeDay;

	if (	   macdData.size() != DEAData.size() 
		|| DiffData.size() != KDJ_KData.size()
		|| DiffData.size() != KDJ_DData.size()
		|| macdData.size() != DiffData.size()
		|| macdData.size() <= 1)
	{
		LOG(INFO) << "Real time data size" << macdData.size() << DEAData.size() << DiffData.size() << KDJ_KData.size() << KDJ_DData.size();
		LOG(ERROR) << Indicators._strStockCode << " AnalysisReal timedata data size error." ;
		return false;
	}
	unsigned int lastIndex = macdData.size() - 1;
	if (DiffData[lastIndex] > DiffData[lastIndex - 1]
		&& KDJ_DData[lastIndex] > KDJ_DData[lastIndex-1]
		&& KDJ_KData[lastIndex] < 20)
	{
		LOG(INFO) << "Get Stock code:" << Indicators._strStockCode << " Time:" << Indicators._vDate[lastIndex].GetDateTime();
		cout << "Get Stock code:" << Indicators._strStockCode << " Time:" << Indicators._vDate[lastIndex].GetDateTime() << endl;
	}
// 	ShowCurrentCode();
// 	ShowYesterdayCode();
	return true;
}

void CRealTimeAna::Inition()
{
}

void CRealTimeAna::ShowCurrentCode()
{

}

void CRealTimeAna::ShowYesterdayCode()
{

}



//
