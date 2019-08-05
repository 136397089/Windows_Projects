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

//�ⲿ���������,���ڷ��������
struct NumberEvent
{
	CycleType OrderTypeCycle;
	HigherOrderType OrderType;
	StockDataType mNumberData;
	NumberEvent& IniData(const HigherOrderType _NumberType, const StockDataType _Number, const CycleType cycle);
};
//���ڱ��Group
struct CSigDayGroup
{
	HigherOrderType mDayOrderType;
	HigherOrderType mDayNPType;
	ENotesType NoteType;//��ע�������� 
	int pChangeRateGroupIndex;//���ڱ�Ǳ仯�����ڵķ���
	CSigDayGroup& Inition(HigherOrderType _HighOrderNumber, HigherOrderType _NPType,
		ENotesType _noteType, int _pChangeGroupIndex);
	void AddNumberEvent(const NumberEvent& _Event);
	CSigDayGroup() :mDayOrderType(_eDPUnknow), mDayNPType(_eDPUnknow), pChangeRateGroupIndex(0), NoteType(_eCTUnknow)	{}
	bool operator < (const CSigDayGroup & tempdata) const;
	bool operator== (const CSigDayGroup & tempdata) const;
};
struct CycOrderType
{
	CycOrderType() :mIndT(_eDPUnknow), mNPT(_eDPUnknow){}
	bool operator < (const CycOrderType & tempdata) const;
	bool operator == (const CycOrderType & tempdata) const;
	HigherOrderType mIndT;
	HigherOrderType mNPT;
};
//
struct ProcessGroup
{
#define M30m 0
#define M60m 1
#define MDay 2
#define MWek 3
#define MMon 4
#define MarDay 5
#define MarWek 6
#define MarMon 7
	CycOrderType IndT[8];
	ENotesType NoteGroT;
	ProcessGroup& Inition(HigherOrderType _NumberType, HigherOrderType _NPType, CycleType DataCycle);
	bool IsSubset(const ProcessGroup& chileGroup) const ;
	vector<string> GetTypeString() const;
	ProcessGroup() :	NoteGroT(_eCTUnknow){}
	bool operator < (const ProcessGroup &tempdata) const;
	bool operator == (const ProcessGroup &tempdata) const;
};

//���ڶ���ʷ�ϵ��������ݽ��з��飬��Ҫ���ڷ������յ���ʷ����
class CSigDayGrouping
{
public:
	CSigDayGrouping();
	~CSigDayGrouping();

	//��¼���ݣ����з���
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, ENotesType _changeType);
	//��¼���ݶ�Ӧ������
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//����һ��ָ��ͼ۸�仯�ʣ����ط�������
	CSigDayGroup NumberEventToGroup(const NumberEvent& _numType, StockDataType _pChange);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	CSigDayGroup NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	CSigDayGroup NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
	//����������ͣ����ض�Ӧ�����ڲ�ͬ�����ʵ�Ƶ��
	bool GetNumberFreqData(CSigDayGroup _GroupType, vector<unsigned int>& _numdata);
	//����������ͣ����ض�Ӧ�������Ƶ���������ʾ�ֵ�������ʷ���
	bool GetGroupMeanVar(CSigDayGroup _GroupType,MeanVar& returnMeanVar);
	//
	vector<CSigDayGroup> vkeyGroupType;
	//
	CFreqStatistice FreqTool;

	map<CSigDayGroup, MeanVar> meanVarData;
private:
	//��¼���ݶ�Ӧ����Ƶ���������ʾ�ֵ�������ʷ���
	bool SetGroupMeanVar(CSigDayGroup _GroupType, StockDataType _pChange);
	//������Ҫ�ر��ע�ķ���
	void SetEspecialObserveKeyType(const CSigDayGroup& _GroupType);
	//������еķ���Ƶ������
	map<CSigDayGroup, unsigned int> feqData;
	//���ڷ������Ƶ���������ʾ�ֵ�������ʷ���
	//���ڼ�����Ƶ���������ʾ�ֵ�������ʷ���Ĺ��߶���
	CMeanVariance MeanVarTool;
	//
	HigherOrderType dateRecordBasisType;
	HigherOrderType dateRecordPosNegType;
	//��¼��Ҫ��ע��ָ�����ͷ�������
	vector<CDate> dateRecord;
	//
	ENotesType noteType;
};
//
struct ProcessNodes
{
	ProcessNodes() :_high(0),_low(0){}
	DayPrice beginNode;
	StockDataType _high;
	StockDataType _low;
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
	void RecordDataFreq(const vector<NumberEvent>& _data ,
		const DayPrice& _price, ENotesType _SpecGroupType, bool StatueEnable, bool IsLastIndex);
	//
	void CalReturnRate();
	//�����еķ�����Ϣ��ӡ����
	void PrintAllGroupInfor(const DayPrice& beginNode, const DayPrice& endNode);
	//
	void PrintBeginNodeInfor(const ProcessNodes& beginNode, const ProcessGroup& GroupType);
private:
	//����Ŀ�ʼ��������
	bool BeginTrigger(const vector<NumberEvent>& _data);
	//����Ľ�����������
	bool EndTrigger(const vector<NumberEvent>& _data);
	//
	void SetKeyGroupType();
	//
	void SaveProGroupRate(CNumbersToSql& SQlTool, ProcessGroup& _currentGroup, vector<ProcessNodes>& _nodes, ENotesType NoteGroupType);
	//
	ProcessGroup GetCombinGroup(const vector<NumberEvent>& _data);//��_data�����еķ���������ϳ�Ϊһ������
	//
	void GetCompatibleGroupKeyGroup(const ProcessGroup& GroupTypeCombin,vector<ProcessGroup>& CompatibleGroup);

	//���ڼ�¼����Ŀ�ʼ�ڵ㣬�����Ӧ����Ŀ�ʼ�ڵ���ڣ�˵����ǰ�����ڶ�Ӧ�ķ���״̬��
	map<ProcessGroup, ProcessNodes> GroupBeginNodes;
	//��Ҫ��ע�ķ�������
	vector<ProcessGroup> vkeyGroupT;
	//��Ҫ��ע�ķ�������
// 	vector<ProcessGroup> vkeyMarGroup;
	//����Ľ��
	map<ProcessGroup, vector<ProcessNodes>> GroupRecord;
	//��¼��ǰ�����Ĺ�Ʊ���ݴ��룬�ڳ�ʼ��ʱ��¼
	string stockCode;
	HigherOrderType orderType;
	//
	bool BuyAllow;
};

#endif
