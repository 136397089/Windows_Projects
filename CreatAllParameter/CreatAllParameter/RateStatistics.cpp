#include "stdafx.h"
#include "RateStatistics.h"


CRateStatistics::CRateStatistics()
{
}


CRateStatistics::~CRateStatistics()
{
}

bool CRateStatistics::AnalysisData(CIndicatorsInterface& daynumber, CIndicatorsInterface& _wholeSHnumber)
{
	_AnaNumber = &daynumber.GetResourceValue();
	_Weeknumber = &daynumber.GetWeekValue();
	_Mounthnumber = &daynumber.GetMonthValue();
	_shNumber = &_wholeSHnumber.GetResourceValue();
	_shWeekNumber = &_wholeSHnumber.GetWeekValue();
	_shMonNumber = &_wholeSHnumber.GetMonthValue();

	if (!_AnaNumber->ChackDataSize())
		return false;
	Inition();
	for (CurrentIndex = 0; CurrentIndex < _AnaNumber->_vTimeDay.size(); CurrentIndex++)
	{
		GetSigDayTechIndex();
		AnaNumberStateTool.IterationSingleData(ResourceValueSigDayData, _AnaNumber->_vTimeDay[CurrentIndex]);
		if (CurrentIndex < 200)
			continue;
		srand((unsigned)time(NULL));
		GetOneDayPrice(oneDayPrice, CurrentIndex, *_AnaNumber);
// 		if (oneDayPrice._closeData / oneDayPrice._frontClose >= 1.098	|| oneDayPrice._closeData / oneDayPrice._frontClose <= 0.902)
// 			continue;//排除当天涨停和跌停的情况
		FroShInd = _shNumber->GetCloselyFrontTimeIndexByDate(_AnaNumber->_vDate[CurrentIndex]);
		FroShWeInd = _shWeekNumber->GetCloselyFrontTimeIndexByDate(_AnaNumber->_vDate[CurrentIndex]);
		FroShMonInd = _shMonNumber->GetCloselyFrontTimeIndexByDate(_AnaNumber->_vDate[CurrentIndex]);
		FrWeekInd = _Weeknumber->GetCloselyFrontTimeIndexByDate(_AnaNumber->_vDate[CurrentIndex]);
		FroMonInd = _Mounthnumber->GetCloselyFrontTimeIndexByDate(_AnaNumber->_vDate[CurrentIndex]);

		SetIndicatorsData();
		CurrentDataRecordToGroup();
		PGroupTool.RecordDataFreq(IndtData, oneDayPrice, _eGroup1, true);// AnaNumberStateTool.CheckBeginState());
		GroupTool.RecordDay(IndtData, oneDayPrice._frontdate);
	}
	PGroupTool.CalReturnRate();

	return true;
	SaveAllGroupFreqData();
	return true;

}

void CRateStatistics::GetOneDayPrice(DayPrice& oneDayPrice, unsigned int index, const StockDataTable& AnaNumber) const
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
	oneDayPrice._frontdate.SetDay(AnaNumber._vTimeDay[index - 1]);
}

bool CRateStatistics::SaveOneGroupFreqData(const string& stockCoed, Group _GroupType, CHistoryGroup& GroupTool)
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

void CRateStatistics::SetIndicatorsData()
{
#define DataFun(HOT,P,TP,IND) IndtData.push_back(NumEvt.IniData(HOT,(P)->_vTableAllIndex[(TP)][(IND)]))
#define DeFun(HOT,P,TP,IND)\
IndtData.push_back(NumEvt.IniData(HOT,P->_vTableAllIndex[TP][IND] - P->_vTableAllIndex[TP][IND-1]))
	NumberEvent NumEvt;
	IndtData.clear();

	DeFun(DeDayDEA, _AnaNumber, _eMACD_DEA, CurrentIndex - 1);
	DeFun(DeDayDIFF, _AnaNumber, _eMACD_DIFF, CurrentIndex - 1);
	DataFun(DayAble, _AnaNumber, _eDataEnable, CurrentIndex - 1);
	DataFun(DayDIFF, _AnaNumber, _eMACD_DIFF, CurrentIndex - 1);
	DataFun(DayVRMA, _AnaNumber, _eVRMA, CurrentIndex - 1);

	DeFun(DeDayKDJ_D,_AnaNumber, _eKDJ_D, CurrentIndex - 1);
	DeFun(DeDayKDJ_K,_AnaNumber, _eKDJ_K, CurrentIndex - 1);
	DeFun(DeDayCRMA,_AnaNumber, _eCRMA1, CurrentIndex - 1);
	DataFun(DayKDJ_K, _AnaNumber, _eKDJ_K, CurrentIndex - 1);
	DataFun(DayKDJ_J, _AnaNumber, _eKDJ_J, CurrentIndex - 1);

	DeFun(DeWeekKDJ_K, _Weeknumber, _eKDJ_K, FrWeekInd);
	DeFun(DeWeekKDJ_D, _Weeknumber, _eKDJ_D, FrWeekInd);
	DeFun(DeWeekDEA, _Weeknumber, _eMACD_DEA, FrWeekInd);
	DeFun(DeWeekVRMA, _Weeknumber, _eVRMA, FrWeekInd);
	DataFun(WeekKDJ_K, _Weeknumber, _eKDJ_K, FrWeekInd);

	DeFun(DeMonthDIFF, _Mounthnumber, _eMACD_DIFF, FroMonInd);
	DeFun(DeMonthDEA, _Mounthnumber, _eMACD_DEA, FroMonInd);
	DeFun(DeMonthKDJ_K, _Mounthnumber, _eKDJ_K, FroMonInd);
	DeFun(DeMonthKDJ_D, _Mounthnumber, _eKDJ_D, FroMonInd);
	DataFun(MonthKDJ_K, _Mounthnumber, _eKDJ_K, FroMonInd);

	DeFun(ShDeDayBAR, _shNumber, _eMACD_BAR, FroShInd);
	DeFun(ShDeDayKDJ_D, _shNumber, _eKDJ_D, FroShInd);
	DeFun(ShDeDayKDJ_K, _shNumber, _eKDJ_K, FroShInd);
	DeFun(ShDeDayMAEMV, _shNumber, _eEMV, FroShInd);
	DeFun(ShDeDayDIFF, _shNumber, _eMACD_DIFF, FroShInd);
	DeFun(ShDeDayDEA, _shNumber, _eMACD_DEA, FroShInd);

	DeFun(ShDeWeekDEA, _shWeekNumber, _eMACD_DEA, FroShWeInd);

}

void CRateStatistics::CurrentDataRecordToGroup()
{
	if (CurrentIndex > 0 && CurrentIndex <= (_AnaNumber->_vTimeDay.size() / 2)){
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_C(oneDayPrice), _eGroup1);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_H(oneDayPrice), _eGroup1);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_L(oneDayPrice), _eGroup1);
	}
	if (CurrentIndex > (_AnaNumber->_vTimeDay.size() / 4) && CurrentIndex <= (_AnaNumber->_vTimeDay.size() / 2))	{
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_C(oneDayPrice), _eGroup2);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_H(oneDayPrice), _eGroup2);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_L(oneDayPrice), _eGroup2);
	}
	if (CurrentIndex > (_AnaNumber->_vTimeDay.size() / 2) && CurrentIndex <= (_AnaNumber->_vTimeDay.size() * 3 / 4))	{
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_C(oneDayPrice), _eGroup3);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_H(oneDayPrice), _eGroup3);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_L(oneDayPrice), _eGroup3);
	}
	if (CurrentIndex > (_AnaNumber->_vTimeDay.size() * 3 / 4))	{
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_C(oneDayPrice), _eGroup4);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_H(oneDayPrice), _eGroup4);
		GroupTool.RecordDataFreq(IndtData, GetReturnRate_L(oneDayPrice), _eGroup4);
	}

}

void CRateStatistics::SaveAllGroupFreqData()
{
	Group keyGroup;
	for (unsigned int i = 0; i < GroupTool.vkeyGroupType.size(); i++)
	{
		keyGroup = GroupTool.vkeyGroupType[i];
		keyGroup.NoteType = _eGroup1;
		SaveOneGroupFreqData(_AnaNumber->_strStockCode, keyGroup, GroupTool);
		keyGroup.NoteType = _eGroup2;
		SaveOneGroupFreqData(_AnaNumber->_strStockCode, keyGroup, GroupTool);
		keyGroup.NoteType = _eGroup3;
		SaveOneGroupFreqData(_AnaNumber->_strStockCode, keyGroup, GroupTool);
		keyGroup.NoteType = _eGroup4;
		SaveOneGroupFreqData(_AnaNumber->_strStockCode, keyGroup, GroupTool);
		keyGroup.NoteType = _eMixed;
		SaveOneGroupFreqData(_AnaNumber->_strStockCode, keyGroup, GroupTool);
	}
	return;
}

void CRateStatistics::GetSigDayTechIndex()
{
	ResourceValueSigDayData._MacdData.dif = _AnaNumber->_vTableAllIndex[_eMACD_DIFF][CurrentIndex];
	ResourceValueSigDayData._MacdData.dea = _AnaNumber->_vTableAllIndex[_eMACD_DEA][CurrentIndex];
	ResourceValueSigDayData._MacdData.bar = _AnaNumber->_vTableAllIndex[_eMACD_BAR][CurrentIndex];
	ResourceValueSigDayData._ArBrVrPsy.VR = _AnaNumber->_vTableAllIndex[_eVRMA][CurrentIndex];
	ResourceValueSigDayData._GI.Dataable = _AnaNumber->_vTableAllIndex[_eDataEnable][CurrentIndex];


	ResourceValueSigDayData._Open = _AnaNumber->_vOpen[CurrentIndex];
	ResourceValueSigDayData._Close = _AnaNumber->_vClose[CurrentIndex];
	ResourceValueSigDayData._High = _AnaNumber->_vHigh[CurrentIndex];
	ResourceValueSigDayData._Low = _AnaNumber->_vLow[CurrentIndex];
}

void CRateStatistics::Inition()
{
	AnaNumberStateTool.Inition(_AnaNumber->_strStockCode);
	PGroupTool.Inition(_AnaNumber->_strStockCode);
	FroShInd = 0;
	FroShWeInd = 0;
	FroShMonInd = 0;
	FroMonInd = 0;
	FrWeekInd = 0;

}
