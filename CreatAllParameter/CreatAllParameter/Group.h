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
	HigherOrderType mHighOrderType;
	StockDataType mNumberData;
	NumberEvent& IniData(const HigherOrderType _NumberType, const StockDataType _Number);
};
//���ڱ��Group
struct Group
{
	HigherOrderType mHighOrderType;
	HigherOrderType mNPType;
	ENotesType NoteType;//��ע��������
	int pChangeRateGroupIndex;//���ڱ�Ǳ仯�����ڵķ���
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

//���ڶ���ʷ�ϵ��������ݽ��з��飬��Ҫ���ڷ������յ���ʷ����
class CHistoryGroup
{
public:
	CHistoryGroup();
	~CHistoryGroup();

	//��¼���ݣ����з���
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, ENotesType _changeType);
	//��¼���ݶ�Ӧ������
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//����һ��ָ��ͼ۸�仯�ʣ����ط�������
	Group NumberEventToGroup(const NumberEvent& _numType, StockDataType _pChange);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	Group NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	Group NumberEventToGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
	//����������ͣ����ض�Ӧ�����ڲ�ͬ�����ʵ�Ƶ��
	bool GetNumberFreqData(Group _GroupType, vector<unsigned int>& _numdata);
	//����������ͣ����ض�Ӧ�������Ƶ���������ʾ�ֵ�������ʷ���
	bool GetGroupMeanVar(Group _GroupType,MeanVar& returnMeanVar);
	//
	vector<Group> vkeyGroupType;
	//
	CFreqStatistice FreqTool;

	map<Group, MeanVar> meanVarData;
private:
	//��¼���ݶ�Ӧ����Ƶ���������ʾ�ֵ�������ʷ���
	bool SetGroupMeanVar(Group _GroupType, StockDataType _pChange);
	//������Ҫ�ر��ע�ķ���
	void SetEspecialObserveKeyType(const Group& _GroupType);
	//������еķ���Ƶ������
	map<Group, unsigned int> feqData;
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
	void GetCompatibleGroupKeyGroup(const ProcessGroup& GroupTypeCombin,vector<ProcessGroup>& CompatibleGroup);
// 	void SetBeginNode(vector<ProcessGroup>& CompatibleGroup);
	//���ڼ�¼����Ŀ�ʼ�ڵ㣬�����Ӧ����Ŀ�ʼ�ڵ���ڣ�˵����ǰ�����ڶ�Ӧ�ķ���״̬��
	map<ProcessGroup, DayPrice> GroupBeginNodes;
	//��Ҫ��ע�ķ�������
	vector<ProcessGroup> vkeyGroupType;
	//����Ľ��
	map<ProcessGroup, vector<ProcessNodes>> GroupRecord;
	//��¼��ǰ�����Ĺ�Ʊ���ݴ��룬�ڳ�ʼ��ʱ��¼
	string stockCode;
	HigherOrderType orderType;
	//
	bool BuyAllow;
};

#endif
