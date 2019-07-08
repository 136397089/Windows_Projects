#include "stdafx.h"
#include <algorithm>
#include "Group.h"
#include "glog/logging.h"


CHistoryGroup::CHistoryGroup():
FreqTool(ChangeRateStep, MinimumChangeRate / ChangeRateStep, MaximumChangeRate / ChangeRateStep)
{
	Group _GroupType;
	vkeyGroupType.push_back(_GroupType.Inition(HigherOrderType(DeDayDIFF),
		HigherOrderType(DeDayDIFF), _eMixed, 0));
	vkeyGroupType.push_back(_GroupType.Inition(HigherOrderType(DeDayDIFF | ShDeDayDIFF),
		HigherOrderType(DeDayDIFF | ShDeDayDIFF), _eMixed, 0));

	dateRecordBasisType = HigherOrderType(DeDayDIFF | DeDayKDJ_K | ShDeDayDIFF | ShDeDayKDJ_K);
	dateRecordPosNegType = HigherOrderType(DeDayDIFF | DeDayKDJ_K | ShDeDayDIFF | ShDeDayKDJ_K);
}


CHistoryGroup::~CHistoryGroup()
{
}


void CHistoryGroup::RecordDataFreq(const vector<NumberEvent>& _numberData, StockDataType _pChange, ENotesType _noteype)
{
	noteType = _noteype;
	Group dataGroup;
	dataGroup.NoteType = _noteype;
	//完成单个数据类型的分组
	for (unsigned int i = 0; i < _numberData.size();i++)
	{
		dataGroup = NumberEventToGroup(_numberData[i] , _pChange);
		if (feqData.count(dataGroup) == 0)
			feqData[dataGroup] = 0;
		feqData[dataGroup]++;
		SetGroupMeanVar(dataGroup , _pChange);
	}
	//将所有的类型组合到一起
	Group allTypeCombin;
	allTypeCombin.NoteType = _noteype;
	allTypeCombin.pChangeRateGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	for (unsigned int index = 0; index < _numberData.size();index++)
		allTypeCombin.AddNumberEvent(_numberData[index]);

	//对关键类型进行分组
	for (unsigned int index = 0; index < vkeyGroupType.size();index++)
	{
		Group AnaGroup = vkeyGroupType[index];
		if (   (AnaGroup.mHighOrderType & allTypeCombin.mHighOrderType) != AnaGroup.mHighOrderType
			|| (AnaGroup.mHighOrderType & allTypeCombin.mNPType) != AnaGroup.mNPType	
			|| (AnaGroup.mHighOrderType & AnaGroup.mNPType) != AnaGroup.mNPType )
			continue;

		if ((AnaGroup.mHighOrderType & allTypeCombin.mHighOrderType) == AnaGroup.mHighOrderType
			&& ((AnaGroup.mHighOrderType & allTypeCombin.mNPType) == AnaGroup.mNPType))
		{
			AnaGroup.pChangeRateGroupIndex = allTypeCombin.pChangeRateGroupIndex;
			AnaGroup.NoteType = allTypeCombin.NoteType;
			if (feqData.count(AnaGroup) == 0)
				feqData[AnaGroup] = 0;
			feqData[AnaGroup]++;
			SetGroupMeanVar(AnaGroup, _pChange);
		}
	}

}
bool CHistoryGroup::SetGroupMeanVar(Group _GroupType, StockDataType _pChange)
{
	_GroupType.pChangeRateGroupIndex = 0;
	if (meanVarData.count(_GroupType) == 0)
	{
		MeanVar emptyMeanVar;
		meanVarData[_GroupType] = emptyMeanVar;
	}
	MeanVarTool.GetNextMeanVar(_pChange, meanVarData[_GroupType]);
	return true;
}
Group CHistoryGroup::NumberEventToGroup(const NumberEvent& _numType, StockDataType _pChange)
{
	Group tempGroup;
	tempGroup.mHighOrderType = _numType.mHighOrderType;
	if (_numType.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)_numType.mHighOrderType;
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	return tempGroup;
}

Group CHistoryGroup::NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx)
{
	Group tempGroup;
	tempGroup.mHighOrderType = HigherOrderType(_numType1.mHighOrderType | _numType2.mHighOrderType);
	if (_numType1.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)_numType1.mHighOrderType;
	if (_numType2.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)(tempGroup.mNPType | _numType2.mHighOrderType);
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = _Groupinedx;
	return tempGroup;
}

Group CHistoryGroup::NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx)
{
	Group tempGroup;
	tempGroup.mHighOrderType = HigherOrderType(_numType1.mHighOrderType | _numType2.mHighOrderType | _numType3.mHighOrderType);
	if (_numType1.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)_numType1.mHighOrderType;
	if (_numType2.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)(tempGroup.mNPType | _numType2.mHighOrderType);
	if (_numType3.mNumberData >= 0)
		tempGroup.mNPType = (HigherOrderType)(tempGroup.mNPType | _numType3.mHighOrderType);
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = _Groupinedx;
	return tempGroup;
}

bool CHistoryGroup::GetNumberFreqData(Group _GroupType, vector<unsigned int>& _numdata)
{
	_numdata.clear();
	_numdata.push_back((unsigned int)_GroupType.mHighOrderType);
	_numdata.push_back((unsigned int)_GroupType.mNPType);
	_numdata.push_back((unsigned int)_GroupType.NoteType);
	for (int i = FreqTool.beginGroup; i < FreqTool.endGroup;i++)
	{
		_GroupType.pChangeRateGroupIndex = i;
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
		dataGroup.mHighOrderType = (HigherOrderType)(dataGroup.mHighOrderType | _numberData[i].mHighOrderType);
		if (_numberData[i].mNumberData > 0)
		{
			dataGroup.mNPType = (HigherOrderType)(dataGroup.mNPType | _numberData[i].mHighOrderType);
		}
	}
	if ((dataGroup.mNPType & dateRecordPosNegType) == dateRecordPosNegType
		&& (dataGroup.mHighOrderType & dateRecordBasisType) == dateRecordBasisType)
		dateRecord.push_back(_date);
}

bool CHistoryGroup::GetGroupMeanVar(Group _GroupType, MeanVar& returnMeanVar)
{
	_GroupType.pChangeRateGroupIndex = 0;
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
	return (mHighOrderType < tempdata.mHighOrderType)
		|| (mHighOrderType == tempdata.mHighOrderType && mNPType<tempdata.mNPType)
		|| (mHighOrderType == tempdata.mHighOrderType && mNPType == tempdata.mNPType && pChangeRateGroupIndex < tempdata.pChangeRateGroupIndex)
		|| (mHighOrderType == tempdata.mHighOrderType && mNPType == tempdata.mNPType && pChangeRateGroupIndex == tempdata.pChangeRateGroupIndex && NoteType < tempdata.NoteType);
}
bool Group::operator == (const Group & tempdata) const
{
	return (mHighOrderType == tempdata.mHighOrderType && mNPType == tempdata.mNPType && pChangeRateGroupIndex == tempdata.pChangeRateGroupIndex && NoteType == tempdata.NoteType);
}
Group& Group::Inition(HigherOrderType _HighOrderNumber, HigherOrderType _NPType, ENotesType _changeRateType, int _pChangeGroupIndex)
{
	mHighOrderType = _HighOrderNumber;
	mNPType = _NPType;
	NoteType = _changeRateType;
	pChangeRateGroupIndex = _pChangeGroupIndex;
	return *this;
}
void Group::AddNumberEvent(const NumberEvent& _Event)
{
	mHighOrderType = HigherOrderType(mHighOrderType | _Event.mHighOrderType);
	if (_Event.mNumberData > 0)
		mNPType = HigherOrderType(mNPType | _Event.mHighOrderType);
}
NumberEvent& NumberEvent::IniData(const HigherOrderType _NumberType,const StockDataType _number)
{
	mHighOrderType = _NumberType;
	mNumberData = _number;
	return *this;
}





CProcessGrouping::CProcessGrouping():
BuyAllow(false)
{
}

CProcessGrouping::~CProcessGrouping()
{
}

void CProcessGrouping::Inition(const string& stockCode)
{
	this->stockCode = stockCode;
	vkeyGroupType.clear();
	GroupBeginNodes.clear();
	BuyAllow = false;
	ProcessGroup _GroupType;

	orderType = HigherOrderType(DeWeekDEA | DeDayDEA);
	vkeyGroupType.push_back(_GroupType.Inition(orderType, orderType, _eGroup1));
}
bool CProcessGrouping::BeginTrigger(const vector<NumberEvent>& _data)
{
	int Number1 = 0;
	int Number2 = 0;
	int Number3 = 0;
	int Number4 = 0;
	int Othernumber = 0;
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		BuyAllow = true;
// 		if (_data[i].mHighOrderType == _eBasisDayKDJ_K &&  _data[i].mNumberData > 80)
// 			BuyAllow = true;
// 		if (_data[i].mHighOrderType == _eBasisDayKDJ_K &&  _data[i].mNumberData < 50)
// 			BuyAllow = false;
// 		if (_data[i].mHighOrderType == _eBasisDayKDJ_K &&  _data[i].mNumberData < 25)
// 			Number3++;
// 		if (_data[i].mHighOrderType == _eBasisDeDayKDJ_D &&  _data[i].mNumberData > 0)
// 			Number3++;
// 		if (_data[i].mHighOrderType == _eBasisWeekKDJ_K &&  _data[i].mNumberData < 20)
// 			Number2++;
// 		if (_data[i].mHighOrderType == _eBasisDeWeekKDJ_D &&  _data[i].mNumberData > 0)
// 			Number2++;
// 		if (_data[i].mHighOrderType == _eBasisDeWeekVRMA &&  _data[i].mNumberData > 0)
// 			Othernumber++;
// 		if (_data[i].mHighOrderType == _eBasisDeWeekCRMA &&  _data[i].mNumberData > 0)
// 			Othernumber++;
// 		if (_data[i].mHighOrderType == _eBasisDeWeekKDJ_D &&  _data[i].mNumberData > 0)
// 			Othernumber++;
// 		if (_data[i].mHighOrderType == _eBasisDeWeekDEA &&  _data[i].mNumberData > 0)
// 			Othernumber++;
// 		if (_data[i].mHighOrderType == _eBasisDeDayDEA &&  _data[i].mNumberData > 0)
// 			Othernumber++;
// 		if (_data[i].mHighOrderType == _eBasisDeDayDIFF &&  _data[i].mNumberData > 0)
// 			Othernumber++;
	}
	if (Othernumber >= 1 && Number3 >= 2 && Number2 >= 1/* && BuyAllow*/)
		return true;
	return true;
}

bool CProcessGrouping::EndTrigger(const vector<NumberEvent>& _data)
{
	int number = 0;
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		if (_data[i].mHighOrderType == DeDayCRMA &&  _data[i].mNumberData < -40)
		{
			BuyAllow = false;
			number++;
		}
	}
	if (number >= 1)
		return false;
	return false;
}
void CProcessGrouping::RecordDataFreq(const vector<NumberEvent>& _numberData, 
	const DayPrice& _price, ENotesType _SpecGroupType, bool StatueEnable)
{
	//将所有的类型组合到一起
	ProcessGroup GroupTypeCombin = GetCombinGroup(_numberData);
	GroupTypeCombin.NoteGroupType = _SpecGroupType;

	vector<ProcessGroup> CompatibleGroup;
	GetCompatibleGroupKeyGroup(GroupTypeCombin, CompatibleGroup);

	for (vector<ProcessGroup>::iterator ite = CompatibleGroup.begin(); ite != CompatibleGroup.end();ite++)
	{
		//之前的GroupBeginNodes没有记录到,表示出现新的关注点
		if (GroupBeginNodes.count(*ite) == 0 && BeginTrigger(_numberData) && StatueEnable)
			GroupBeginNodes[*ite] = _price;
		if (_numberData.front().mNumberData < 0.5f && _numberData.front().mHighOrderType == DayAble)
		{
			LOG(INFO) << "Warning: Data unable ,clear all BeginNodes";
			GroupBeginNodes.clear();
		}
	}
	vector<ProcessGroup> eraseGroups;
	for (map<ProcessGroup, DayPrice>::iterator ite = GroupBeginNodes.begin(); ite != GroupBeginNodes.end();ite++)
	{
		vector<ProcessGroup>::iterator pite = find(CompatibleGroup.begin(), CompatibleGroup.end(), ite->first);
		if (CompatibleGroup.end() == pite || EndTrigger(_numberData))
		{
			ProcessNodes tempNotes;
			tempNotes.beginNode = GroupBeginNodes[ite->first];
			tempNotes.endNode = _price;
			if (GroupRecord.count(ite->first) == 0)
			{
				vector<ProcessNodes> emptyNodes;
				GroupRecord[ite->first] = emptyNodes;
			}
			BuyAllow = false;
			GroupRecord[ite->first].push_back(tempNotes);
			eraseGroups.push_back(ite->first);
		}
	}
	for (unsigned int i = 0; i < eraseGroups.size();i++)
	{
		GroupBeginNodes.erase(eraseGroups[i]);
	}
}

void CProcessGrouping::CalReturnRate()
{
	if (GroupRecord.size() == 0)
	{
		LOG(INFO) << "Group Record data is empty.";
		return;
	}
	vector<StockDataType> currentReturnRate;
	currentReturnRate.clear();
	CNumbersToSql SQlTool;
	SQlTool.IniMysqlTool();
	ProcessGroup currentGroup;
	for (map<ProcessGroup, vector<ProcessNodes>>::iterator ite = GroupRecord.begin();
		ite != GroupRecord.end();ite++)
	{
		currentGroup = ite->first;
		DayPrice beginNode;
		DayPrice endNode;
		//第一组
		SaveProGroupRate(SQlTool, currentGroup, ite->second,
			ENotesType(_Mid2MidType | _BFClose  | _BHigh| _BLow| _EFClose | _EHigh | _ELow));
	}
}

void CProcessGrouping::PrintGroupInformation(const DayPrice& beginNode,const DayPrice& endNode)
{
	if (beginNode._date.IsOnTheSameDay(endNode._date))
		LOG(INFO) << stockCode << " is on the same day ,begin at " << beginNode._frontdate.GetDateTime() << " end at " << endNode._date.GetDateTime();
	else
		LOG(INFO) << stockCode << " begin at " << beginNode._frontdate.GetDateTime() << " end at " << endNode._date.GetDateTime();
}


void CProcessGrouping::SaveProGroupRate(CNumbersToSql& SQlTool,	ProcessGroup& _currentGroup,
	vector<ProcessNodes>& _nodes, ENotesType NoteGroupType)
{

#define SaveGroupRateFun(NT,IND)\
	if ((NoteGroupType & (NT)) == (NT))\
		{\
		_currentGroup.NoteGroupType = (NT);\
		SQlTool.SaveProGroupRate(tableName, stockCode, currentReturnRate[(IND)], _currentGroup.GetTypeString());\
		}
#define GetGroupRateFun(NT,IND,PuData)\
	if ((NoteGroupType & (NT)) == (NT))\
	 currentReturnRate[(IND)].push_back((PuData));

	vector<StockDataType> currentReturnRate[13];
	DayPrice beginNode;
	DayPrice endNode;
	StockDataType beginMid ,endMid = 0.0;
	for (unsigned int i = 0; i < _nodes.size(); i++)
	{
		endNode = _nodes[i].endNode;
		beginNode = _nodes[i].beginNode;
		endMid = (endNode._closeData + endNode._highData + endNode._lowData) / 3;
		beginMid = (beginNode._closeData + beginNode._highData + beginNode._lowData) / 3;
		GetGroupRateFun(_Mid2MidType, 0, endMid / beginMid);

		GetGroupRateFun(_BClose, 1, beginMid / beginNode._closeData);
		GetGroupRateFun(_BHigh, 2, beginMid / beginNode._highData);
		GetGroupRateFun(_BLow, 3, beginMid / beginNode._lowData);
		GetGroupRateFun(_BFClose, 4, beginMid / beginNode._frontClose);
		GetGroupRateFun(_BFHigh, 5, beginMid / beginNode._frontHigh);
		GetGroupRateFun(_BFLow, 6, beginMid / beginNode._frontLow);

		GetGroupRateFun(_EClose, 7, endNode._closeData / endMid);
		GetGroupRateFun(_EHigh, 8, endNode._highData / endMid);
		GetGroupRateFun(_ELow, 9, endNode._lowData / endMid);
		GetGroupRateFun(_EFClose, 10, endNode._frontClose / endMid);
		GetGroupRateFun(_EFHigh, 11, endNode._frontHigh / endMid);
		GetGroupRateFun(_EFLow, 12, endNode._frontLow / endMid);
	}
	ostringstream dataOss;
	dataOss << "ProGroupRate" << orderType;
	string tableName = dataOss.str();
	SaveGroupRateFun(_Mid2MidType, 0);
	SaveGroupRateFun(_BClose, 1);
	SaveGroupRateFun(_BHigh, 2);
	SaveGroupRateFun(_BLow, 3);
	SaveGroupRateFun(_BFClose, 4);
	SaveGroupRateFun(_BFHigh, 5);
	SaveGroupRateFun(_BFLow, 6);
	SaveGroupRateFun(_EClose, 7);
	SaveGroupRateFun(_EHigh, 8);
	SaveGroupRateFun(_ELow, 9);
	SaveGroupRateFun(_EFClose, 10);
	SaveGroupRateFun(_EFHigh, 11);
	SaveGroupRateFun(_EFLow, 12);
	return;
}

ProcessGroup CProcessGrouping::GetCombinGroup(const vector<NumberEvent>& _data)
{
	ProcessGroup GroupTypeCombin;
	for (unsigned int index = 0; index < _data.size(); index++)
	{
		GroupTypeCombin.mIndexT = (HigherOrderType)(GroupTypeCombin.mIndexT | _data[index].mHighOrderType);
		if (_data[index].mNumberData > 0)
			GroupTypeCombin.mNPType = (HigherOrderType)(GroupTypeCombin.mNPType | _data[index].mHighOrderType);
	}
	return GroupTypeCombin;
}

void CProcessGrouping::GetCompatibleGroupKeyGroup(const ProcessGroup& GroupTypeCombin, vector<ProcessGroup>& CompatibleGroup)
{
	for (vector<ProcessGroup>::iterator ite = vkeyGroupType.begin(); ite != vkeyGroupType.end(); ite++)
	{
		if ((ite->mNPType & GroupTypeCombin.mNPType) == ite->mNPType
			&& (ite->mIndexT & GroupTypeCombin.mIndexT) == ite->mIndexT
			&& ite->NoteGroupType == GroupTypeCombin.NoteGroupType)
		{
			CompatibleGroup.push_back(*ite);
		}
	}
	return;

}




bool ProcessGroup::operator<(const ProcessGroup & tempdata) const
{
	return (mIndexT < tempdata.mIndexT)
		|| (mIndexT == tempdata.mIndexT && mNPType < tempdata.mNPType)
		|| (mIndexT == tempdata.mIndexT && mNPType == tempdata.mNPType && NoteGroupType < tempdata.NoteGroupType);

}
bool ProcessGroup::operator == (const ProcessGroup & tempdata) const
{
	return (mIndexT == tempdata.mIndexT && mNPType == tempdata.mNPType && NoteGroupType == tempdata.NoteGroupType);
}
ProcessGroup& ProcessGroup::Inition(HigherOrderType _NumberType, HigherOrderType _NPType, ENotesType _SpecGroupType)
{
	mIndexT = _NumberType;
	mNPType = _NPType;
	NoteGroupType = _SpecGroupType;
	return *this;
}
vector<string> ProcessGroup::GetTypeString() const
{
	ostringstream ossData;
	vector<string> typeString;
	ossData.str("");
	ossData << (unsigned int)mIndexT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)mNPType;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)NoteGroupType;
	typeString.push_back(ossData.str());
	return typeString;
}
