#include "stdafx.h"
#include <numeric>
#include <complex>//复数
#include <cmath>
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

CStatisticeInter::CStatisticeInter():
moveVarDaySize(100)
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
	const StockDataTable& daydata,
	const StockDataTable& weekdata,
	const StockDataTable& mounthdata,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	Inition();

	vector<unsigned int> MALineResult1;
	vector<unsigned int> MALineResult2;
	MALineStatistice(daydata._vMa1, daydata._vMa4, MALineResult1, MALineResult2);
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

	for (unsigned int i = interval; i < _inputdata.size(); i++)
	{

	}
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




bool CStatisticeInter::StatisticeASIData(
	const StockDataTable& daynumber,
	CStateInter& daystate,
	const string& Stockcode)
{
	DayPrice oneDayPrice;//用于暂存日价格数据
	MeanVar _localFilterMeanVar;//moveMeanVarTool迭代总移动方差和均值的返回变量
	MeanVar _localAllMeanVar;//moveMeanVarTool迭代总移动方差和均值的返回变量
	CMeanVariance MeanVarTool;
	StateTable& states = daystate.allIndexStates[_eASI_I];
	const StockDataTable& AnaNumber = daynumber;
	StatePoint tempPoint;
	unsigned _Passtimeinedx = 0;
	for (unsigned int index = 20; index < daynumber._vTimeDay.size(); index++)
	{
		GetOneDayPrice(oneDayPrice, index, AnaNumber);
		tempPoint = states.CloselyLocalPoint(index - 2);
		if (tempPoint._IndexType == _eLocalLow)
			tempPoint = states.CloselyLocalPoint(tempPoint._TimeIndex - 1);
		if (   !(daynumber._vAsi_i[index-1] > tempPoint._Value)
			|| !(tempPoint._IndexType == _eLocalHigh)
			|| !(daynumber._vAsi_i[index - 1] > daynumber._vAsi_i[index - 2])
// 			|| _Passtimeinedx+20 >= index
			)
		{
			continue;
		}
		_Passtimeinedx = index;
		MeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _localFilterMeanVar);
		MeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _localFilterMeanVar);
		MeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _localFilterMeanVar);
		LOG(INFO) << Stockcode << " Check Day index :" << daynumber._vTimeDay[index];
	}
	SaveMeanVarList_CS(Stockcode, _localFilterMeanVar, _localAllMeanVar);
	return true;
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

bool CStatisticeInter::GroupDabug(
	const StockDataTable& daynumber,
	const StockDataTable& weeknumber,
	const StockDataTable& mounthnumber,
	const StockDataTable shnumber,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	//EnterCriticalSection(&filterDataMeancs);
	const StockDataTable& AnaNumber = daynumber;

	if (!AnaNumber.ChackDataSize())
	{
		//LeaveCriticalSection(&filterDataMeancs);
		return false;
	}
	DayPrice oneDayPrice;//用于暂存日价格数据
	CHistoryGroup GroupTool;
	NumberEvent tempNumberEvent;
	vector<NumberEvent> _numberdata;
	map<Group, MeanVar> meanVarMiddle;
	unsigned int shIndex = 0;
	unsigned int mInx = 0;
	for (unsigned int index = 80; index < AnaNumber._vTimeDay.size(); index++)
	{
		GetOneDayPrice(oneDayPrice, index, AnaNumber);
		if (oneDayPrice._closeData / oneDayPrice._frontclose >= 1.098
			|| oneDayPrice._closeData / oneDayPrice._frontclose <= 0.902)
			continue;//排除当天涨停和跌停的情况
		mInx = mounthnumber.GetLastTimeIndexByDate(AnaNumber._vDate[index]);
		_numberdata.clear();
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayDIFF, AnaNumber._vDiff[index - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDIFF, AnaNumber._vDiff[index - 1] - AnaNumber._vDiff[index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayDIFF, AnaNumber._vCR[index - 1] + AnaNumber._vDiff[index - 3] - 2 * AnaNumber._vDiff[index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayKDJ_K, AnaNumber._vK[index - 1] - AnaNumber._vK[index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayKDJ_K, AnaNumber._vK[index - 1] + AnaNumber._vK[index - 3] - 2 * AnaNumber._vK[index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDayCR, AnaNumber._vCR[index - 1] - 80));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayVR, AnaNumber._vVR[index - 1] - AnaNumber._vVR[index - 2]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisMonthKDJ_KJ, (mounthnumber._vJ[mInx - 1] - mounthnumber._vK[mInx - 1]) + 10));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayCRMA, AnaNumber._vCRMA1[index - 1] - AnaNumber._vCRMA1[index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA1, AnaNumber._vMa1[index - 1] - AnaNumber._vMa2[index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA2, AnaNumber._vMa2[index - 1] - AnaNumber._vMa3[index - 2]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayMA3, AnaNumber._vMa3[index - 1] - AnaNumber._vMa4[index - 2]));

// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisMonthDIFF, mounthnumber._vDiff[MonthIndex]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthDIFF, mounthnumber._vDiff[mInx] - mounthnumber._vDiff[mInx - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeMonthDIFF, mounthnumber._vDiff[MonthIndex] + mounthnumber._vDiff[MonthIndex - 2] - 2 * mounthnumber._vDiff[MonthIndex - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeMonthKDJ_K, mounthnumber._vK[mInx] - mounthnumber._vK[mInx - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeMonthKDJ_K, mounthnumber._vK[MonthIndex] + mounthnumber._vK[MonthIndex - 2] - 2 * mounthnumber._vK[MonthIndex - 1]));

		shIndex = shnumber.GetLastTimeIndexByDate(AnaNumber._vTimeDay[index]);
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDayDIFF, shnumber._vDiff[shIndex]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayDIFF, shnumber._vDiff[shIndex] - shnumber._vDiff[shIndex - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDeDayDIFF, shnumber._vDiff[shIndex] + shnumber._vDiff[shIndex - 2] - 2 * shnumber._vDiff[shIndex - 1]));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayKDJ_K, shnumber._vK[shIndex] - shnumber._vK[shIndex - 1]));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDeDayKDJ_K, shnumber._vK[shIndex] + shnumber._vK[shIndex - 2] - 2 * shnumber._vK[shIndex - 1] + 5));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisShDeDayCR, shnumber._vCR[shIndex] - shnumber._vCR[shIndex - 1] - 20));
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
		if (index > (AnaNumber._vTimeDay.size() / 2) && index <= (AnaNumber._vTimeDay.size() * 3/4))	{
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup3);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup3);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup3);
		}
		if (index > (AnaNumber._vTimeDay.size() * 3 / 4))	{
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_C(oneDayPrice), _eGroup4);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_H(oneDayPrice), _eGroup4);
			GroupTool.RecordDataFreq(_numberdata, GetReturnRate_L(oneDayPrice), _eGroup4);
		}
		GroupTool.RecordDay(_numberdata, oneDayPrice._frontdate);

	}
	vector<unsigned int> FreqAndCountData;
	Group keyGroup;
	for (unsigned int i = 0; i < GroupTool.vkeyGroupType.size();i++)
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
	GroupTool.GetNumberFreqData(_GroupType, FreqAndCountData);//获得统计的频次数据
	GroupTool.GetGroupMeanVar(_GroupType, groupMeanVar);//获得统计的均值和方差
	vector<float> meanVarData;
	FreqAndCountData.push_back(groupMeanVar.n);
	meanVarData.push_back(groupMeanVar.mean);
	meanVarData.push_back(groupMeanVar.var);
	EnterCriticalSection(&filterDataMeancs);
	CNumbersToSql SQlTool;
	SQlTool.IniMysqlTool();
	SQlTool.InsertData(stockCoed, FreqAndCountData, meanVarData, GroupTool.FreqTool.beginGroup, GroupTool.FreqTool.endGroup);
	SQlTool.SaveFreqCacheData();
	LeaveCriticalSection(&filterDataMeancs);
	return true;
}

void CStatisticeInter::GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& AnaNumber) const 
{
	oneDayPrice._openData = AnaNumber._vOpen[index];
	oneDayPrice._closeData = AnaNumber._vClose[index];
	oneDayPrice._highData = AnaNumber._vHigh[index];
	oneDayPrice._lowData = AnaNumber._vLow[index];
	oneDayPrice._date.SetDay(AnaNumber._vTimeDay[index]);

	oneDayPrice._frontopen = AnaNumber._vOpen[index - 1];
	oneDayPrice._frontclose = AnaNumber._vClose[index - 1];
	oneDayPrice._fronthigh = AnaNumber._vHigh[index - 1];
	oneDayPrice._frontlow = AnaNumber._vLow[index - 1];
	oneDayPrice._frontdate = AnaNumber._vDate[index - 1];
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



void CheckCurrentStockDataAndGetName(
	const StockDataTable& AnaNumber,
	const StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode)
{
	if (monthIndex != 0 && Currentindex >= AnaNumber._vTimeDay.size() - 1)
	{
		if ((AnaNumber._vDiff[Currentindex] - AnaNumber._vDiff[Currentindex - 1] < 0.00f))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((2 * AnaNumber._vDiff[Currentindex - 1] - AnaNumber._vDiff[Currentindex] - AnaNumber._vDiff[Currentindex - 2] > 0))
			LOG(WARNING) << StockCode << " Day diff failure to meet conditions.";
		else if ((AnaNumber._vK[Currentindex] - AnaNumber._vK[Currentindex - 1] < 0.00))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((2 * AnaNumber._vK[Currentindex - 1] - AnaNumber._vK[Currentindex] - AnaNumber._vK[Currentindex - 2] > 0.00f))
			LOG(WARNING) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((mounthnumber._vDiff[monthIndex] - mounthnumber._vDiff[monthIndex - 1] < 0))
			LOG(WARNING) << StockCode << " Month diff failure to meet conditions.";
		else if ((allDataMeanVar.mean < 0.00f))
			LOG(WARNING) << StockCode << " Day mean failure to meet conditions.";
		else
		{
			LOG(INFO) << " Get stock name:    " << StockCode;
			LOG(INFO) << " Data: " << AnaNumber._vTimeDay[Currentindex] << " inedx: " << Currentindex;
			LOG(INFO) << " MACD DIF: " << AnaNumber._vDiff[Currentindex] << " ," << AnaNumber._vDiff[Currentindex - 1];
		}
	}
}




CRealTimeAna::CRealTimeAna()
{
}

CRealTimeAna::~CRealTimeAna()
{
}



bool CRealTimeAna::AnalysisRealTimeData(map<string, MACDCombin>& macdData, map<string, KDJCombin>& KDJData)
{
	if (macdData.size() != KDJData.size())
	{
		return false;
	}
	vector<NumberEvent> _numberdata;
	NumberEvent tempNumberEvent;
	realTimeDataToAna RealTimeData;
	CurrentGroupingTool.Inition();
	YesterdayGroupingTool.Inition();
	for (map<string, MACDCombin>::const_iterator ite = macdData.begin(); ite != macdData.end(); ite++)
	{
		RealTimeData.CloselyMACDs = ite->second;
		RealTimeData.CloselyKDJs = KDJData[ite->first];
		_numberdata.clear();
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDIFF, RealTimeData.CloselyMACDs.CurrentMacd.dif - RealTimeData.CloselyMACDs.TodayMacd.dif));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayDIFF, RealTimeData.CloselyMACDs.CurrentMacd.dif + RealTimeData.CloselyMACDs.YesterdayMacd.dif - 2 * RealTimeData.CloselyMACDs.TodayMacd.dif));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayKDJ_K, RealTimeData.CloselyKDJs.CurrentKDJ.K_OF_KDJ - RealTimeData.CloselyKDJs.TodayKDJ.K_OF_KDJ));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayKDJ_K, RealTimeData.CloselyKDJs.CurrentKDJ.K_OF_KDJ + RealTimeData.CloselyKDJs.YesterdayKDJ.K_OF_KDJ - 2 * RealTimeData.CloselyKDJs.TodayKDJ.K_OF_KDJ));

		CurrentGroupingTool.GroupingNumbers(ite->first, _numberdata, _eMixed);

		_numberdata.clear();
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayDIFF, RealTimeData.CloselyMACDs.TodayMacd.dif - RealTimeData.CloselyMACDs.YesterdayMacd.dif));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayDIFF, RealTimeData.CloselyMACDs.TodayMacd.dif + RealTimeData.CloselyMACDs.BefoYesMacd.dif - 2 * RealTimeData.CloselyMACDs.YesterdayMacd.dif));
		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDayKDJ_K, RealTimeData.CloselyKDJs.TodayKDJ.K_OF_KDJ - RealTimeData.CloselyKDJs.YesterdayKDJ.K_OF_KDJ));
// 		_numberdata.push_back(tempNumberEvent.IniData(_eBasisDeDeDayKDJ_K, RealTimeData.CloselyKDJs.TodayKDJ.K_OF_KDJ + RealTimeData.CloselyKDJs.BefYesKDJ.K_OF_KDJ - 2 * RealTimeData.CloselyKDJs.YesterdayKDJ.K_OF_KDJ));
		YesterdayGroupingTool.GroupingNumbers(ite->first, _numberdata, _eMixed);
	}
	ShowCurrentCode();
	ShowYesterdayCode();
	return true;
}

void CRealTimeAna::Inition()
{
	CurrentGroupingTool.Inition();
}

void CRealTimeAna::ShowCurrentCode()
{
	vector<string> returnCode;
	CurrentGroupingTool.GetGroup(
		(BasisType)(_eBasisDeDayDIFF/* | _eBasisDeDeDayDIFF*/ | _eBasisDeDayKDJ_K /*| _eBasisDeDeDayKDJ_K*/),
		(PosNegType)(_eBasisDeDayDIFF_P /*| _eBasisDeDeDayDIFF_P*/ | _eBasisDeDayKDJ_K_P /*| _eBasisDeDeDayKDJ_K_P*/),
		returnCode);
	CNumbersToSql SQLTool;
	SQLTool.IniMysqlTool();
	int countFromFund = 0;
	int index = 0;
	float pe = 0;
	int TableCout = 8;

	cout << endl<<"*******Current data********"<<endl;
	for (unsigned int i = 0; i < returnCode.size(); i++)
	{
		countFromFund = SQLTool.CheckStockFundCount(returnCode[i]);
		pe = SQLTool.ReadPEFromSQL(returnCode[i]);
		if (countFromFund > 2 && pe < 25 && pe > 2)
		{
			if (0 == index)
				cout << setw(TableCout) << "code"
				<< setw(TableCout) << "count"
				<< setw(TableCout) << "PE"
				<< setw(TableCout) << "code"
				<< setw(TableCout) << "count"
				<< setw(TableCout) << "PE";
			if (index % 2 == 0)
				cout << endl;
			index++;
			cout << setw(TableCout) << returnCode[i]
				<< setw(TableCout) << countFromFund
				<< setw(TableCout) << pe;
		}
	}
}

void CRealTimeAna::ShowYesterdayCode()
{
	vector<string> returnCode;
	YesterdayGroupingTool.GetGroup(
		(BasisType)(_eBasisDeDayDIFF /*| _eBasisDeDeDayDIFF*/ | _eBasisDeDayKDJ_K /*| _eBasisDeDeDayKDJ_K*/),
		(PosNegType)(_eBasisDeDayDIFF_P /*| _eBasisDeDeDayDIFF_P*/ | _eBasisDeDayKDJ_K_P /*| _eBasisDeDeDayKDJ_K_P*/),
		returnCode);
	CNumbersToSql SQLTool;
	SQLTool.IniMysqlTool();
	int countFromFund = 0;
	int index = 0;
	float pe = 0;
	int TableCout = 8;
	cout << endl << "*******Yesterday data********" << endl;
	for (unsigned int i = 0; i < returnCode.size(); i++)
	{
		countFromFund = SQLTool.CheckStockFundCount(returnCode[i]);
		pe = SQLTool.ReadPEFromSQL(returnCode[i]);
		if (countFromFund > 2 && pe < 25 && pe > 2)
		{
			if (0 == index)
				cout << setw(TableCout) << "code"
				<< setw(TableCout) << "count"
				<< setw(TableCout) << "PE"
				<< setw(TableCout) << "code"
				<< setw(TableCout) << "count"
				<< setw(TableCout) << "PE";
			if (index % 2 == 0)
				cout << endl;
			index++;
			cout << setw(TableCout) << returnCode[i]
				<< setw(TableCout) << countFromFund
				<< setw(TableCout) << pe;
		}
	}
}



//
