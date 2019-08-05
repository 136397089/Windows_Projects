#include "stdafx.h"
#include <algorithm>
#include "Group.h"
#include "glog/logging.h"


CSigDayGrouping::CSigDayGrouping():
FreqTool(ChangeRateStep, MinimumChangeRate / ChangeRateStep, MaximumChangeRate / ChangeRateStep)
{
	CSigDayGroup _GroupType;

	dateRecordBasisType = HigherOrderType();
	dateRecordPosNegType = HigherOrderType();
}


CSigDayGrouping::~CSigDayGrouping()
{
}


void CSigDayGrouping::RecordDataFreq(const vector<NumberEvent>& _numberData, StockDataType _pChange, ENotesType _noteype)
{
	noteType = _noteype;
	CSigDayGroup dataGroup;
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
	CSigDayGroup allTypeCombin;
	allTypeCombin.NoteType = _noteype;
	allTypeCombin.pChangeRateGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	for (unsigned int index = 0; index < _numberData.size();index++)
		allTypeCombin.AddNumberEvent(_numberData[index]);

	//对关键类型进行分组
	for (unsigned int index = 0; index < vkeyGroupType.size();index++)
	{
		CSigDayGroup AnaGroup = vkeyGroupType[index];
		if (   (AnaGroup.mDayOrderType & allTypeCombin.mDayOrderType) != AnaGroup.mDayOrderType
			|| (AnaGroup.mDayOrderType & allTypeCombin.mDayNPType) != AnaGroup.mDayNPType	
			|| (AnaGroup.mDayOrderType & AnaGroup.mDayNPType) != AnaGroup.mDayNPType )
			continue;

		if ((AnaGroup.mDayOrderType & allTypeCombin.mDayOrderType) == AnaGroup.mDayOrderType
			&& ((AnaGroup.mDayOrderType & allTypeCombin.mDayNPType) == AnaGroup.mDayNPType))
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
bool CSigDayGrouping::SetGroupMeanVar(CSigDayGroup _GroupType, StockDataType _pChange)
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
CSigDayGroup CSigDayGrouping::NumberEventToGroup(const NumberEvent& _numType, StockDataType _pChange)
{
	CSigDayGroup tempGroup;
	tempGroup.mDayOrderType = _numType.OrderType;
	if (_numType.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)_numType.OrderType;
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = FreqTool.GetTheGroupIndex(_pChange, ChangeRateStep);
	return tempGroup;
}

CSigDayGroup CSigDayGrouping::NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx)
{
	CSigDayGroup tempGroup;
	tempGroup.mDayOrderType = HigherOrderType(_numType1.OrderType | _numType2.OrderType);
	if (_numType1.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)_numType1.OrderType;
	if (_numType2.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)(tempGroup.mDayNPType | _numType2.OrderType);
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = _Groupinedx;
	return tempGroup;
}

CSigDayGroup CSigDayGrouping::NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx)
{
	CSigDayGroup tempGroup;
	tempGroup.mDayOrderType = HigherOrderType(_numType1.OrderType | _numType2.OrderType | _numType3.OrderType);
	if (_numType1.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)_numType1.OrderType;
	if (_numType2.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)(tempGroup.mDayNPType | _numType2.OrderType);
	if (_numType3.mNumberData >= 0)
		tempGroup.mDayNPType = (HigherOrderType)(tempGroup.mDayNPType | _numType3.OrderType);
	tempGroup.NoteType = noteType;
	tempGroup.pChangeRateGroupIndex = _Groupinedx;
	return tempGroup;
}

bool CSigDayGrouping::GetNumberFreqData(CSigDayGroup _GroupType, vector<unsigned int>& _numdata)
{
	_numdata.clear();
	_numdata.push_back((unsigned int)_GroupType.mDayOrderType);
	_numdata.push_back((unsigned int)_GroupType.mDayNPType);
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

void CSigDayGrouping::SetEspecialObserveKeyType(const CSigDayGroup& _GroupType)
{
	vkeyGroupType.push_back(_GroupType);
}

void CSigDayGrouping::RecordDay(const vector<NumberEvent>& _numberData, const CDate& _date)
{
	CSigDayGroup dataGroup;
	for (unsigned int i = 0; i < _numberData.size(); i++)
	{
		dataGroup.mDayOrderType = (HigherOrderType)(dataGroup.mDayOrderType | _numberData[i].OrderType);
		if (_numberData[i].mNumberData > 0)
		{
			dataGroup.mDayNPType = (HigherOrderType)(dataGroup.mDayNPType | _numberData[i].OrderType);
		}
	}
	if ((dataGroup.mDayNPType & dateRecordPosNegType) == dateRecordPosNegType
		&& (dataGroup.mDayOrderType & dateRecordBasisType) == dateRecordBasisType)
		dateRecord.push_back(_date);
}

bool CSigDayGrouping::GetGroupMeanVar(CSigDayGroup _GroupType, MeanVar& returnMeanVar)
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





bool CSigDayGroup::operator<(const CSigDayGroup & tempdata) const
{
	return (mDayOrderType < tempdata.mDayOrderType)
		|| (mDayOrderType == tempdata.mDayOrderType && mDayNPType<tempdata.mDayNPType)
		|| (mDayOrderType == tempdata.mDayOrderType && mDayNPType == tempdata.mDayNPType && pChangeRateGroupIndex < tempdata.pChangeRateGroupIndex)
		|| (mDayOrderType == tempdata.mDayOrderType && mDayNPType == tempdata.mDayNPType && pChangeRateGroupIndex == tempdata.pChangeRateGroupIndex && NoteType < tempdata.NoteType);
}
bool CSigDayGroup::operator == (const CSigDayGroup & tempdata) const
{
	return (mDayOrderType == tempdata.mDayOrderType && mDayNPType == tempdata.mDayNPType && pChangeRateGroupIndex == tempdata.pChangeRateGroupIndex && NoteType == tempdata.NoteType);
}
CSigDayGroup& CSigDayGroup::Inition(HigherOrderType _HighOrderNumber, HigherOrderType _NPType, ENotesType _changeRateType, int _pChangeGroupIndex)
{
	mDayOrderType = _HighOrderNumber;
	mDayNPType = _NPType;
	NoteType = _changeRateType;
	pChangeRateGroupIndex = _pChangeGroupIndex;
	return *this;
}
void CSigDayGroup::AddNumberEvent(const NumberEvent& _Event)
{
	mDayOrderType = HigherOrderType(mDayOrderType | _Event.OrderType);
	if (_Event.mNumberData > 0)
		mDayNPType = HigherOrderType(mDayNPType | _Event.OrderType);
}
NumberEvent& NumberEvent::IniData(const HigherOrderType _NumberType,const StockDataType _number,const CycleType cycle)
{
	OrderType = _NumberType;
	mNumberData = _number;
	OrderTypeCycle = cycle;
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
	vkeyGroupT.clear();
	GroupBeginNodes.clear();
	ProcessNodes emtypeNode;
	for (unsigned int i = 0; i < vkeyGroupT.size(); i++)
	{
		GroupBeginNodes[vkeyGroupT[i]] = emtypeNode;
	}
	BuyAllow = false;
	ProcessGroup _GroupType;

	orderType = HigherOrderType(DeDEA | DeKDJ_D);
	_GroupType.Inition(HigherOrderType(DeDEA | DeKDJ_D), HigherOrderType(DeDEA | DeKDJ_D), eDay);
	_GroupType.Inition(HigherOrderType(DeDEA), HigherOrderType(DeDEA), eMarketDay);
	_GroupType.NoteGroT = _eGroup1;
	vkeyGroupT.push_back(_GroupType);

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
		if (_data[i].OrderType == KDJ_K &&  _data[i].mNumberData <= 0)
			Number1++;
	}
	if (Number1 >= 1/* && BuyAllow*/)
		return true;
	return false;
}

bool CProcessGrouping::EndTrigger(const vector<NumberEvent>& _data)
{
	return false;
	int number = 0;
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		if (_data[i].OrderType == DeKDJ_D &&  _data[i].mNumberData < 0)
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
	const DayPrice& _price, ENotesType _NoteGroupType, bool StatueEnable,bool IsLastIndex)
{
	//将输入的所有数据组合到一个ProcessGroup
	ProcessGroup GroupTypeCombin = GetCombinGroup(_numberData);
	GroupTypeCombin.NoteGroT = _NoteGroupType;
	//从KeyGroup里面找出从属于GroupTypeCombin的分组类型，
	vector<ProcessGroup> CompatibleGroup;
	GetCompatibleGroupKeyGroup(GroupTypeCombin, CompatibleGroup);

	//GroupBeginNodes中找出CompatibleGroup没有出现的分组，表示当日数据出现卖出信号
	ProcessNodes empProNode;
	for (map<ProcessGroup, ProcessNodes>::iterator ite = GroupBeginNodes.begin(); ite != GroupBeginNodes.end(); ite++)
	{
		vector<ProcessGroup>::iterator pite = find(CompatibleGroup.begin(), CompatibleGroup.end(), ite->first);
		if ((CompatibleGroup.end() == pite || EndTrigger(_numberData)) && ite->second.beginNode._openData >= FLT_EPSILON)
		{
			ProcessNodes tempNotes = GroupBeginNodes[ite->first];
			tempNotes.endNode = _price;
			GroupBeginNodes[ite->first] = empProNode;
			if (GroupRecord.count(ite->first) == 0)
			{
				vector<ProcessNodes> emptyNodes;
				GroupRecord[ite->first] = emptyNodes;
			}
			GroupRecord[ite->first].push_back(tempNotes);
		}
	}
	//
	for (vector<ProcessGroup>::iterator ite = CompatibleGroup.begin(); ite != CompatibleGroup.end();ite++)
	{
		//之前的GroupBeginNodes没有记录到,表示出现新的KeyGroup
		if (GroupBeginNodes[*ite].beginNode._openData <= FLT_EPSILON && BeginTrigger(_numberData) && StatueEnable)
		{
			GroupBeginNodes[*ite].beginNode = _price;
			GroupBeginNodes[*ite]._high = _price._highData;
			GroupBeginNodes[*ite]._low = _price._lowData;
			if (IsLastIndex)
				PrintBeginNodeInfor(GroupBeginNodes[*ite],*ite);
		}

		if (GroupBeginNodes[*ite].beginNode._openData >= FLT_EPSILON && GroupBeginNodes[*ite]._high < _price._highData)
			GroupBeginNodes[*ite]._high = _price._highData;
		if (GroupBeginNodes[*ite].beginNode._openData >= FLT_EPSILON && GroupBeginNodes[*ite]._low < _price._lowData)
			GroupBeginNodes[*ite]._high = _price._lowData;

		if (_numberData.front().mNumberData < 0.5f && _numberData.front().OrderType == Able)
		{
			LOG(INFO) << "Warning: Data unable ,clear all BeginNodes";
			GroupBeginNodes.clear();
		}
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
			ENotesType(_Mid2MidType | _BFClose | _BHigh| _BLow| _EFClose | _EHigh | _ELow | _Max | _Min));
	}
}

void CProcessGrouping::PrintAllGroupInfor(const DayPrice& beginNode,const DayPrice& endNode)
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
		_currentGroup.NoteGroT = (NT);\
		SQlTool.SaveProGroupRate(tableName, stockCode, currentReturnRate[(IND)], _currentGroup.GetTypeString());\
		}
#define GetGroupRateFun(NT,IND,PuData)\
	if ((NoteGroupType & (NT)) == (NT))\
	 currentReturnRate[(IND)].push_back((PuData));

	vector<StockDataType> currentReturnRate[15];
	DayPrice beginNode;
	DayPrice endNode;
	StockDataType beginMid ,endMid = 0.0;
	for (unsigned int index = 0; index < _nodes.size(); index++)
	{
		endNode = _nodes[index].endNode;
		beginNode = _nodes[index].beginNode;
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

		GetGroupRateFun(_Max, 13, _nodes[index]._high / endMid);
		GetGroupRateFun(_Min, 14, _nodes[index]._low / endMid);
		PrintAllGroupInfor(beginNode,endNode);
	}
	ostringstream dataOss;
	dataOss << "ProGroupRate" << (unsigned int )orderType;
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
	SaveGroupRateFun(_Max, 13);
	SaveGroupRateFun(_Min, 14);
	return;
}

ProcessGroup CProcessGrouping::GetCombinGroup(const vector<NumberEvent>& _data)
{
	ProcessGroup GroupTypeCombin;
	for (unsigned int index = 0; index < _data.size(); index++)	{
		if (_data[index].OrderTypeCycle == eMarketDay)	{
			GroupTypeCombin.IndT[MarDay].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MarDay].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MarDay].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MarDay].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eMarketWeek)	{
			GroupTypeCombin.IndT[MWek].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MWek].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MWek].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MWek].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eMarketMonth)	{
			GroupTypeCombin.IndT[MarMon].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MarMon].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MarMon].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MarMon].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eMinute30)	{
			GroupTypeCombin.IndT[M30m].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[M30m].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[M30m].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[M30m].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eMinute60)	{
			GroupTypeCombin.IndT[M60m].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[M60m].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[M60m].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[M60m].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eDay)	{
			GroupTypeCombin.IndT[MDay].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MDay].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MDay].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MDay].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eWeek)	{
			GroupTypeCombin.IndT[MWek].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MWek].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MWek].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MWek].mNPT | _data[index].OrderType);
		}
		else if (_data[index].OrderTypeCycle == eMonth)	{
			GroupTypeCombin.IndT[MMon].mIndT = (HigherOrderType)(GroupTypeCombin.IndT[MMon].mIndT | _data[index].OrderType);
			if (_data[index].mNumberData > 0)
				GroupTypeCombin.IndT[MMon].mNPT = (HigherOrderType)(GroupTypeCombin.IndT[MMon].mNPT | _data[index].OrderType);
		}
	}
	return GroupTypeCombin;
}

void CProcessGrouping::GetCompatibleGroupKeyGroup(const ProcessGroup& GroupTypeCombin, vector<ProcessGroup>& CompatibleGroup)
{
	for (vector<ProcessGroup>::iterator ite = vkeyGroupT.begin(); ite != vkeyGroupT.end(); ite++)
	{
		if (GroupTypeCombin.IsSubset(*ite))
		{
			CompatibleGroup.push_back(*ite);
		}
	}
	return;

}

void CProcessGrouping::PrintBeginNodeInfor(const ProcessNodes& beginNode, const ProcessGroup& GroupType)
{
	LOG(INFO) << "Get Begin Node stock Code:" << stockCode <<" "<< beginNode.beginNode._date.GetDay()<<" Group Type:" << (unsigned int)GroupType.IndT->mIndT;
}




bool ProcessGroup::operator<(const ProcessGroup & tempdata) const
{
#define IsLes(Data) ((Data) < (tempdata.Data))
#define IsEqu(Data) ((Data) == (tempdata.Data))
	return (IsLes(IndT[MMon]))
		|| (IsEqu(IndT[MMon]) && IsLes(IndT[MWek]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsLes(IndT[MDay]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsLes(IndT[M60m]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsEqu(IndT[M60m]) && IsLes(IndT[M30m]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsEqu(IndT[M60m]) && IsEqu(IndT[M30m]) && IsLes(IndT[MarDay]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsEqu(IndT[M60m]) && IsEqu(IndT[M30m]) && IsEqu(IndT[MarDay]) && IsLes(IndT[MarWek]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsEqu(IndT[M60m]) && IsEqu(IndT[M30m]) && IsEqu(IndT[MarDay]) && IsEqu(IndT[MarWek]) && IsLes(IndT[MMon]))
		|| (IsEqu(IndT[MMon]) && IsEqu(IndT[MWek]) && IsEqu(IndT[MDay]) && IsEqu(IndT[M60m]) && IsEqu(IndT[M30m]) && IsEqu(IndT[MarDay]) && IsEqu(IndT[MarWek]) && IsEqu(IndT[MMon]) && IsLes(NoteGroT))
		;
}
bool ProcessGroup::operator == (const ProcessGroup & tempdata) const
{
	return (IndT[MMon] == tempdata.IndT[MMon] && IndT[MWek] == tempdata.IndT[MWek]
		&& IndT[MDay] == tempdata.IndT[MDay] && IndT[M60m] == tempdata.IndT[M60m]
		&& IndT[M30m] == tempdata.IndT[M30m] && IndT[MarDay] == tempdata.IndT[MarDay]
		&& IndT[MarWek] == tempdata.IndT[MarWek] && IndT[MarMon] == tempdata.IndT[MarMon]
		&& NoteGroT == tempdata.NoteGroT)
		;
}
ProcessGroup& ProcessGroup::Inition(HigherOrderType _NumberType, HigherOrderType _NPType, CycleType DataCycle)
{
	if (DataCycle == eMarketDay){
		IndT[MarDay].mIndT = _NumberType;
		IndT[MarDay].mNPT = _NPType;
	}
	else if (DataCycle == eMarketWeek){
		IndT[MarWek].mIndT = _NumberType;
		IndT[MarWek].mNPT = _NPType;
	}
	else if (DataCycle == eMarketMonth){
		IndT[MarMon].mIndT = _NumberType;
		IndT[MarMon].mNPT = _NPType;
	}
	else if (DataCycle == eMinute30){
		IndT[M30m].mIndT = _NumberType;
		IndT[M30m].mNPT = _NPType;
	}
	else if (DataCycle == eMinute60){
		IndT[M60m].mIndT = _NumberType;
		IndT[M60m].mNPT = _NPType;
	}
	else if (DataCycle == eDay){
		IndT[MDay].mIndT = _NumberType;
		IndT[MDay].mNPT = _NPType;
	}
	else if (DataCycle == eWeek){
		IndT[MWek].mIndT = _NumberType;
		IndT[MWek].mNPT = _NPType;
	}
	else if (DataCycle == eMonth){
		IndT[MMon].mIndT = _NumberType;
		IndT[MMon].mNPT = _NPType;
	}
	else if (DataCycle == eUnkCycle)
	{
		IndT[MarDay].mIndT = _eDPUnknow;
		IndT[MarDay].mNPT = _eDPUnknow;
		IndT[MarWek].mIndT = _eDPUnknow;
		IndT[MarWek].mNPT = _eDPUnknow;
		IndT[MarMon].mIndT = _eDPUnknow;
		IndT[MarMon].mNPT = _eDPUnknow;
		IndT[M30m].mIndT = _eDPUnknow;
		IndT[M30m].mNPT = _eDPUnknow;
		IndT[M60m].mIndT = _eDPUnknow;
		IndT[M60m].mNPT = _eDPUnknow;
		IndT[MDay].mIndT = _eDPUnknow;
		IndT[MDay].mNPT = _eDPUnknow;
		IndT[MWek].mIndT = _eDPUnknow;
		IndT[MWek].mNPT = _eDPUnknow;
		IndT[MMon].mIndT = _eDPUnknow;
		IndT[MMon].mNPT = _eDPUnknow;
	}
	return *this;
}
vector<string> ProcessGroup::GetTypeString() const
{
	ostringstream ossData;
	vector<string> typeString;
	ossData.str("");
	ossData << (unsigned int)IndT[MMon].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MMon].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[MWek].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MWek].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[MDay].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MDay].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[M60m].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[M60m].mIndT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[M30m].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[M30m].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[MarDay].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MarDay].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[MarWek].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MarWek].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)IndT[MarMon].mIndT;
	typeString.push_back(ossData.str());
	ossData.str("");
	ossData << (unsigned int)IndT[MarMon].mNPT;
	typeString.push_back(ossData.str());

	ossData.str("");
	ossData << (unsigned int)NoteGroT;
	typeString.push_back(ossData.str());
	return typeString;
}

bool ProcessGroup::IsSubset(const ProcessGroup& chileGroup) const
{
#define CheckChileFun(CheData) (CheData & chileGroup.CheData) == chileGroup.CheData
	if ((CheckChileFun(IndT[M30m].mIndT)) && CheckChileFun(IndT[M30m].mNPT)
		&& (CheckChileFun(IndT[M60m].mIndT)) && CheckChileFun(IndT[M60m].mNPT)
		&& (CheckChileFun(IndT[MDay].mIndT)) && CheckChileFun(IndT[MDay].mNPT)
		&& (CheckChileFun(IndT[MWek].mIndT)) && CheckChileFun(IndT[MWek].mNPT)
		&& (CheckChileFun(IndT[MMon].mIndT)) && CheckChileFun(IndT[MMon].mNPT)
		&& (CheckChileFun(IndT[MarDay].mIndT)) && CheckChileFun(IndT[MarDay].mNPT)
		&& (CheckChileFun(IndT[MarWek].mIndT)) && CheckChileFun(IndT[MarWek].mNPT)
		&& (CheckChileFun(IndT[MarMon].mIndT)) && CheckChileFun(IndT[MarMon].mNPT)
		&& NoteGroT == chileGroup.NoteGroT
		)
	{
		return true;
	}
	return false;
}

bool CycOrderType::operator<(const CycOrderType & tempdata) const
{
	return (mIndT < tempdata.mIndT || (mIndT == tempdata.mIndT && mNPT < tempdata.mNPT));
}
bool CycOrderType::operator==(const CycOrderType & tempdata) const
{
	return (mIndT == tempdata.mIndT && mNPT == tempdata.mNPT);
}
