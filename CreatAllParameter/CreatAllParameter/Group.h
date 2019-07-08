#pragma once

#ifndef GROUP_H
#define GROUP_H
#include <map>
#include "number/commonfunction.h"
#include "number/NumberBase.h"
#include "FreqStatistice.h"
#include "MeanVariance.h"
#include "NumbersToSql.h"

// #include <bitset>

using namespace std;


#define  MinimumChangeRate -11
#define  MaximumChangeRate 11
#define  ChangeRateStep 0.1

//外部输入的数据,用于分组的依据
struct NumberEvent
{
	HigherOrderType mHighOrderType;
	StockDataType mNumberData;
	NumberEvent& IniData(const HigherOrderType _NumberType, const StockDataType _Number);
};
//用于标记Group
struct Group
{
	HigherOrderType mHighOrderType;
	HigherOrderType mNPType;
	ENotesType NoteType;//备注分组数据
	int pChangeRateGroupIndex;//用于标记变化率所在的分组
	Group& Inition(HigherOrderType _HighOrderNumber, HigherOrderType _NPType,
		ENotesType _noteType, int _pChangeGroupIndex);
	void AddNumberEvent(const NumberEvent& _Event);
	Group() :mHighOrderType(_eDPUnknow), mNPType(_eDPUnknow), pChangeRateGroupIndex(0), NoteType(_eCTUnknow)	{}
	bool operator < (const Group & tempdata) const;
	bool operator== (const Group & tempdata) const;
};
//
struct ProcessGroup
{
	HigherOrderType mIndexT;
	HigherOrderType mNPType;
	ENotesType NoteGroupType;
	ProcessGroup& Inition(HigherOrderType _NumberType, HigherOrderType _NPType, ENotesType _noteType);
	vector<string> GetTypeString() const;
	ProcessGroup() :mIndexT(_eDPUnknow), mNPType(_eDPUnknow), NoteGroupType(_eCTUnknow){}
	bool operator < (const ProcessGroup &tempdata) const;
	bool operator == (const ProcessGroup &tempdata) const;
};

//用于对历史上的所有数据进行分组，主要用于分析单日的历史数据
class CHistoryGroup
{
public:
	CHistoryGroup();
	~CHistoryGroup();

	//记录数据，进行分组
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, ENotesType _changeType);
	//记录数据对应的日期
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//输入一个指标和价格变化率，返回分组类型
	Group NumberEventToGroup(const NumberEvent& _numType, StockDataType _pChange);
	//输入两个指标和价格变化率，返回分组类型
	Group NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//输入三个指标和价格变化率，返回分组类型
	Group NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
	//输入分组类型，返回对应分组在不同收益率的频次
	bool GetNumberFreqData(Group _GroupType, vector<unsigned int>& _numdata);
	//输入分组类型，返回对应分组的总频数、收益率均值、收益率方差
	bool GetGroupMeanVar(Group _GroupType,MeanVar& returnMeanVar);
	//
	vector<Group> vkeyGroupType;
	//
	CFreqStatistice FreqTool;

	map<Group, MeanVar> meanVarData;
private:
	//记录数据对应的总频数、收益率均值、收益率方差
	bool SetGroupMeanVar(Group _GroupType, StockDataType _pChange);
	//设置需要特别关注的分组
	void SetEspecialObserveKeyType(const Group& _GroupType);
	//存放所有的分组频次数据
	map<Group, unsigned int> feqData;
	//存在分组的总频数、收益率均值、收益率方差
	//用于计算总频数、收益率均值、收益率方差的工具对象
	CMeanVariance MeanVarTool;
	//
	HigherOrderType dateRecordBasisType;
	HigherOrderType dateRecordPosNegType;
	//记录需要关注的指标类型分组日期
	vector<CDate> dateRecord;
	//
	ENotesType noteType;
};
//
struct ProcessNodes
{
	DayPrice beginNode;
	DayPrice endNode;
};
//
class CProcessGrouping
{
public:
	CProcessGrouping();
	~CProcessGrouping();
	//
	void Inition(const string& stockCode);
	//
	void RecordDataFreq(const vector<NumberEvent>& _data, const DayPrice& _price, ENotesType _SpecGroupType, bool StatueEnable);
	//
	void CalReturnRate();
	//
	void PrintGroupInformation(const DayPrice& beginNode, const DayPrice& endNode);
private:
	//分组的开始条件触发
	bool BeginTrigger(const vector<NumberEvent>& _data);
	//分组的结束条件触发
	bool EndTrigger(const vector<NumberEvent>& _data);
	//
	void SetKeyGroupType();
	//
	void SaveProGroupRate(CNumbersToSql& SQlTool, ProcessGroup& _currentGroup, vector<ProcessNodes>& _nodes, ENotesType NoteGroupType);
	//
	ProcessGroup GetCombinGroup(const vector<NumberEvent>& _data);//将_data中所有的分组类型组合成为一个类型
	void GetCompatibleGroupKeyGroup(const ProcessGroup& GroupTypeCombin,vector<ProcessGroup>& CompatibleGroup);
// 	void SetBeginNode(vector<ProcessGroup>& CompatibleGroup);
	//用于记录分组的开始节点，如果对应分组的开始节点存在，说明当前正处在对应的分组状态中
	map<ProcessGroup, DayPrice> GroupBeginNodes;
	//需要关注的分组类型
	vector<ProcessGroup> vkeyGroupType;
	//分组的结果
	map<ProcessGroup, vector<ProcessNodes>> GroupRecord;
	//记录当前分析的股票数据代码，在初始化时记录
	string stockCode;
	HigherOrderType orderType;
	//
	bool BuyAllow;
};

#endif
