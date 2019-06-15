#pragma once

#ifndef GROUP_H
#define GROUP_H
#include <map>
#include "number/commonfunction.h"
#include "number/NumberBase.h"
#include "FreqStatistice.h"
#include "MeanVariance.h"
using namespace std;


#define  MinimumChangeRate -11
#define  MaximumChangeRate 11
#define  ChangeRateStep 0.1

//外部输入的数据,用于分组的依据
struct NumberEvent
{
	BasisType NumberType;
	BasisCycleType CycleType;
	StockDataType number;
	NumberEvent IniData(BasisType NumberType, StockDataType number);
};

//用于标记Group
struct Group
{
	BasisType NumberType;
	PosNegType NPType;
	pSpecGroupType changeRateType;
	int pChangeGroupIndex;
	void Inition(BasisType _NumberType, PosNegType _NPType, pSpecGroupType _changeRateType, int _pChangeGroupIndex);
	Group()
		:NumberType(_eDPUnknow),
		NPType(_eNegative),
		pChangeGroupIndex(0),
		changeRateType(_eCTUnknow)
	{}
	bool operator < (const Group & tempdata) const;
	bool operator== (const Group & tempdata) const;
};

//用于对历史上的所有数据进行分组，主要用于分析单日的历史数据
class CHistoryGroup
{
public:
	CHistoryGroup();
	~CHistoryGroup();

	//记录数据，进行分组
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, pSpecGroupType _changeType);
	//记录数据对应的日期
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//输入一个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType, StockDataType _pChange);
	//输入两个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//输入三个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
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
	BasisType dateRecordBasisType;
	PosNegType dateRecordPosNegType;
	//记录需要关注的指标类型分组日期
	vector<CDate> dateRecord;
	//
	pSpecGroupType changeRateType;
};


//用于对一天的数据指标进行分组，主要用于当前的股票数据进行分析
class CCurrentGrouping
{
public:
	CCurrentGrouping(){}
	~CCurrentGrouping(){}
public:
	void Inition();
	bool GroupingNumbers(
		const string& stockCode,
		const vector<NumberEvent>& _data,
		pSpecGroupType _changeType);

	bool GetGroup(BasisType NumberType, PosNegType NPType, vector<string>& TageGroup);
	map<string, Group> stockGroupResult;
};




struct ProcessGroup
{
	BasisType NumberType;
	PosNegType NPType;
	pSpecGroupType SpecGroupType;
	void Inition(BasisType _NumberType, PosNegType _NPType, pSpecGroupType _SpecGroupType);
	vector<string> GetTypeString() const;
	ProcessGroup()
		:NumberType(_eDPUnknow),
		NPType(_eNegative),
		SpecGroupType(_eCTUnknow)
	{}
	bool operator < (const ProcessGroup & tempdata) const;
	bool operator == (const ProcessGroup & tempdata) const;
};
struct ProcessNodes
{
	DayPrice beginNode;
	DayPrice endNode;
};
class CProcessGrouping
{
public:
	CProcessGrouping();
	~CProcessGrouping();
	//
	void Inition(const string& stockCode);

	//
	void RecordDataFreq(
		const vector<NumberEvent>& _data,
		const DayPrice& _price,
		pSpecGroupType _SpecGroupType);
	void CalReturnRate();
	//分组的开始条件触发
	bool BeginTrigger(const vector<NumberEvent>& _data);
	//分组的结束条件触发
	bool EndTrigger(const vector<NumberEvent>& _data);

	//需要关注的分组类型
	vector<ProcessGroup> vkeyGroupType;
	//
	//map<ProcessGroup,vector<StockDataType>> ReturnRates;
private:
	//
	void SetKeyGroupType();
	//用于记录分组的开始节点，如果对应分组的开始节点存在，说明当前正处在对应的分组状态中
	map<ProcessGroup, DayPrice> GroupBeginNodes;
	//分组的结果
	map<ProcessGroup, vector<ProcessNodes>> GroupRecord;
	//记录当前分析的股票数据代码，在初始化时记录
	string stockCode;
};


#endif
