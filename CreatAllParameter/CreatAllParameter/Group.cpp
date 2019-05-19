#include "stdafx.h"
#include "Group.h"
#include "glog/logging.h"

CHistoryGroup::CHistoryGroup():
FreqTool(ChangeRateStep, MinimumChangeRate / ChangeRateStep, MaximumChangeRate / ChangeRateStep)
{
	Group _GroupType;
// 	_GroupType.Inition(
// 		BasisType(_eBasisDeDayDIFF | _eBasisDeDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDeDeDayDIFF | _eBasisDeDeDayKDJ_K),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDeDeDayDIFF_P | _eBasisDeDeDayKDJ_K_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF | _eBasisShDeDayKDJ_K),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisShDeDayDIFF_P | _eBasisShDeDayKDJ_K_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF | _eBasisShDeDayKDJ_K | _eBasisShDeDayCR),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisShDeDayDIFF_P | _eBasisShDeDayKDJ_K_P | _eBasisShDeDayCR_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF | _eBasisShDeDayKDJ_K | _eBasisDeDayCR),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisShDeDayDIFF_P | _eBasisShDeDayKDJ_K_P | _eBasisDeDayCR_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDeDayCR),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDeDayCR_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);

	_GroupType.Inition(BasisType(_eBasisDeDayDIFF ),
		PosNegType(_eBasisDeDayDIFF_P), _eMixed, 0);
	vkeyGroupType.push_back(_GroupType);
	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisShDeDayDIFF | _eBasisDeMonthDIFF),
		PosNegType(_eBasisDeDayDIFF_P | _eBasisShDeDayDIFF_P | _eBasisDeMonthDIFF_P), _eMixed, 0);
	vkeyGroupType.push_back(_GroupType);
	// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDeDayVR | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDeDayVR_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisMonthKDJ_KJ | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisMonthKDJ_KJ_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDayCR | _eBasisMonthKDJ_KJ | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDayCR_P | _eBasisMonthKDJ_KJ_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDeDayVR | _eBasisMonthKDJ_KJ | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDeDayVR_P | _eBasisMonthKDJ_KJ_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisDeDayVR | _eBasisDeMonthDIFF | _eBasisMonthKDJ_KJ | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisDeDayVR_P | _eBasisDeMonthDIFF | _eBasisMonthKDJ_KJ_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);

	// 	_GroupType.Inition(	BasisType(_eBasisDeDayDIFF | _eBasisDeMonthDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeMonthDIFF_P),_eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(	BasisType(_eBasisDeDayDIFF | _eBasisDeMonthKDJ_K),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeMonthKDJ_K_P),_eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayDIFF | _eBasisDeMonthKDJ_K | _eBasisDeMonthDIFF),
// 		PosNegType(_eBasisDeDayDIFF_P | _eBasisDeMonthKDJ_K_P | _eBasisDeMonthDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
// 	_GroupType.Inition(BasisType(_eBasisDeDayMA1 | _eBasisDeDayMA2 | _eBasisDeDayMA3 | _eBasisDeDayCRMA | _eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF),
// 		PosNegType(_eBasisDeDayMA1_P | _eBasisDeDayMA2_P | _eBasisDeDayMA3_P | _eBasisDeDayCRMA_P | _eBasisDeDayDIFF_P | _eBasisDeDayKDJ_K_P | _eBasisShDeDayDIFF_P), _eMixed, 0);
// 	vkeyGroupType.push_back(_GroupType);
	dateRecordBasisType = BasisType(_eBasisDeDayDIFF | _eBasisDeDayKDJ_K | _eBasisShDeDayDIFF | _eBasisShDeDayKDJ_K);
	dateRecordPosNegType = PosNegType(_eBasisDeDayDIFF_P  | _eBasisDeDayKDJ_K_P  | _eBasisShDeDayDIFF_P  | _eBasisShDeDayKDJ_K_P );
}


CHistoryGroup::~CHistoryGroup()
{
}


void CHistoryGroup::RecordDataFreq(const vector<NumberEvent>& _numberData, StockDataType _pChange, pChangeType _changeRateType)
{
	changeRateType = _changeRateType;
	Group dataGroup;
	dataGroup.changeRateType = _changeRateType;
	//完成单个数据类型的分组
	for (unsigned int i = 0; i < _numberData.size();i++)
	{
		dataGroup = GetGroup(_numberData[i],_pChange);
		if (feqData.count(dataGroup) == 0)
			feqData[dataGroup] = 0;
		feqData[dataGroup]++;
		SetGroupMeanVar(dataGroup, _pChange);
	}
	//将所有的类型组合到一起
	Group allTypeGroup;
	allTypeGroup.changeRateType = _changeRateType;
	allTypeGroup.pChangeGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	for (unsigned int index = 0; index < _numberData.size();index++)
	{
		allTypeGroup.NumberType = (BasisType)(allTypeGroup.NumberType | _numberData[index].NumberType);
		if (_numberData[index].number > 0)
		{
			allTypeGroup.NPType = (PosNegType)(allTypeGroup.NPType | _numberData[index].NumberType);
		}
	}
	//
	for (unsigned int index = 0; index < vkeyGroupType.size();index++)
	{
		Group AnaGroup = vkeyGroupType[index];
		if ((AnaGroup.NumberType & allTypeGroup.NumberType) != AnaGroup.NumberType)
			continue;
// 		if ((allTypeGroup.changeRateType | AnaGroup.changeRateType) != _changeRateType)
// 			continue;
		if ((PosNegType(AnaGroup.NumberType & AnaGroup.NPType)) != AnaGroup.NPType)
			continue;
		if ((PosNegType(AnaGroup.NumberType & allTypeGroup.NPType)) != AnaGroup.NPType)
			continue;
		if ((AnaGroup.NumberType & allTypeGroup.NumberType) == AnaGroup.NumberType
			&& ((AnaGroup.NumberType & allTypeGroup.NPType) == AnaGroup.NPType))
		{
			AnaGroup.pChangeGroupIndex = allTypeGroup.pChangeGroupIndex;
			AnaGroup.changeRateType = allTypeGroup.changeRateType;
			if (feqData.count(AnaGroup) == 0)
				feqData[AnaGroup] = 0;
			feqData[AnaGroup]++;
			SetGroupMeanVar(AnaGroup, _pChange);
		}
	}

}
bool CHistoryGroup::SetGroupMeanVar(Group _GroupType, StockDataType _pChange)
{
	_GroupType.pChangeGroupIndex = 0;
	if (meanVarData.count(_GroupType) == 0)
	{
		MeanVar emptyMeanVar;
		meanVarData[_GroupType] = emptyMeanVar;
	}
	MeanVarTool.GetNextMeanVar(_pChange, meanVarData[_GroupType]);
	return true;
}
Group CHistoryGroup::GetGroup(const NumberEvent& _numType, StockDataType _pChange)
{
	Group tempGroup;
	tempGroup.NumberType = _numType.NumberType;
	if (_numType.number >= 0)
		tempGroup.NPType = (PosNegType)_numType.NumberType;
	tempGroup.changeRateType = changeRateType;
	tempGroup.pChangeGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	return tempGroup;
}

Group CHistoryGroup::GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx)
{
	Group tempGroup;
	tempGroup.NumberType = BasisType(_numType1.NumberType | _numType2.NumberType);
	if (_numType1.number >= 0)
		tempGroup.NPType = (PosNegType)_numType1.NumberType;
	if (_numType2.number >= 0)
		tempGroup.NPType = (PosNegType)(tempGroup.NPType|_numType2.NumberType);
	tempGroup.changeRateType = changeRateType;
	tempGroup.pChangeGroupIndex = _Groupinedx;
	return tempGroup;
}

Group CHistoryGroup::GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx)
{
	Group tempGroup;
	tempGroup.NumberType = BasisType(_numType1.NumberType | _numType2.NumberType | _numType3.NumberType);
	if (_numType1.number >= 0)
		tempGroup.NPType = (PosNegType)_numType1.NumberType;
	if (_numType2.number >= 0)
		tempGroup.NPType = (PosNegType)(tempGroup.NPType | _numType2.NumberType);
	if (_numType3.number >= 0)
		tempGroup.NPType = (PosNegType)(tempGroup.NPType | _numType3.NumberType);
	tempGroup.changeRateType = changeRateType;
	tempGroup.pChangeGroupIndex = _Groupinedx;
	return tempGroup;
}

bool CHistoryGroup::GetNumberFreqData(Group _GroupType, vector<unsigned int>& _numdata)
{
	_numdata.clear();
	_numdata.push_back((unsigned int)_GroupType.NumberType);
	_numdata.push_back((unsigned int)_GroupType.NPType);
	_numdata.push_back((unsigned int)_GroupType.changeRateType);
	for (int i = FreqTool.beginGroup; i < FreqTool.endGroup;i++)
	{
		_GroupType.pChangeGroupIndex = i;
		if (feqData.count(_GroupType) > 0)
			_numdata.push_back(feqData[_GroupType]);
		else
			_numdata.push_back(0);
	}
	return true;
}

void CHistoryGroup::SetEspecialObserveKeyType(const Group& _GroupType)
{
	vkeyGroupType.push_back(_GroupType);
}

void CHistoryGroup::RecordDay(const vector<NumberEvent>& _numberData, const CDate& _date)
{
	Group dataGroup;
	for (unsigned int i = 0; i < _numberData.size(); i++)
	{
		dataGroup.NumberType = (BasisType)(dataGroup.NumberType | _numberData[i].NumberType);
		if (_numberData[i].number > 0)
		{
			dataGroup.NPType = (PosNegType)(dataGroup.NPType | _numberData[i].NumberType);
		}
	}
	if ((dataGroup.NPType & dateRecordPosNegType) == dateRecordPosNegType
		&& (dataGroup.NumberType & dateRecordBasisType) == dateRecordBasisType)
		dateRecord.push_back(_date);
}

bool CHistoryGroup::GetGroupMeanVar(Group _GroupType, MeanVar& returnMeanVar)
{
	_GroupType.pChangeGroupIndex = 0;
	if (meanVarData.count(_GroupType) > 0)
	{
		returnMeanVar =  meanVarData[_GroupType];
		return true;
	}
	LOG(ERROR) << "Get Meanvar error.";
	returnMeanVar.clear();
	return false;
}





bool Group::operator<(const Group & tempdata) const
{
	return (NumberType < tempdata.NumberType)
		|| (NumberType == tempdata.NumberType && NPType<tempdata.NPType)
		|| (NumberType == tempdata.NumberType && NPType == tempdata.NPType && pChangeGroupIndex < tempdata.pChangeGroupIndex)
		|| (NumberType == tempdata.NumberType && NPType == tempdata.NPType && pChangeGroupIndex == tempdata.pChangeGroupIndex && changeRateType < tempdata.changeRateType);
}
bool Group::operator == (const Group & tempdata) const
{
	return (NumberType == tempdata.NumberType && NPType == tempdata.NPType && pChangeGroupIndex == tempdata.pChangeGroupIndex && changeRateType == tempdata.changeRateType);
}

void Group::Inition(BasisType _NumberType, PosNegType _NPType, pChangeType _changeRateType, int _pChangeGroupIndex)
{
	NumberType = _NumberType;
	NPType = _NPType;
	changeRateType = _changeRateType;
	pChangeGroupIndex = _pChangeGroupIndex;
}

NumberEvent NumberEvent::IniData(BasisType _NumberType, StockDataType _number)
{
	NumberType = _NumberType;
	number = _number;
	return *this;
}

void COnedayGrouping::Inition()
{
	stockGroupResult.clear();
}

bool COnedayGrouping::GroupingNumbers(
	const string& stockCode, 
	const vector<NumberEvent>& _numberData,
	pChangeType _changeType)
{
	//将所有的类型组合到一起
	Group allTypeGroup;
	allTypeGroup.changeRateType = _changeType;
	allTypeGroup.pChangeGroupIndex = 0;

	for (unsigned int index = 0; index < _numberData.size(); index++)
	{
		allTypeGroup.NumberType = (BasisType)(allTypeGroup.NumberType | _numberData[index].NumberType);
		if (_numberData[index].number > 0)
		{
			allTypeGroup.NPType = (PosNegType)(allTypeGroup.NPType | _numberData[index].NumberType);
		}
	}
	stockGroupResult[stockCode] = allTypeGroup;
	return true;
}

bool COnedayGrouping::GetGroup(BasisType NumberType, PosNegType NPType, vector<string>& TageGroup)
{
	TageGroup.clear();
	for (map<string, Group>::const_iterator ite = stockGroupResult.begin(); ite != stockGroupResult.end();ite++)
	{
		if ((ite->second.NPType & NPType) == NPType && (ite->second.NumberType & NumberType) == NumberType)
			TageGroup.push_back(ite->first);
	}
	return true;
}

