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

//�ⲿ���������,���ڷ��������
struct NumberEvent
{
	BasisType NumberType;
	BasisCycleType CycleType;
	StockDataType number;
	NumberEvent IniData(BasisType NumberType, StockDataType number);
};

//���ڱ��Group
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

//���ڶ���ʷ�ϵ��������ݽ��з��飬��Ҫ���ڷ������յ���ʷ����
class CHistoryGroup
{
public:
	CHistoryGroup();
	~CHistoryGroup();

	//��¼���ݣ����з���
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, pSpecGroupType _changeType);
	//��¼���ݶ�Ӧ������
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//����һ��ָ��ͼ۸�仯�ʣ����ط�������
	Group GetGroup(const NumberEvent& _numType, StockDataType _pChange);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//��������ָ��ͼ۸�仯�ʣ����ط�������
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
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
	BasisType dateRecordBasisType;
	PosNegType dateRecordPosNegType;
	//��¼��Ҫ��ע��ָ�����ͷ�������
	vector<CDate> dateRecord;
	//
	pSpecGroupType changeRateType;
};


//���ڶ�һ�������ָ����з��飬��Ҫ���ڵ�ǰ�Ĺ�Ʊ���ݽ��з���
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
	//����Ŀ�ʼ��������
	bool BeginTrigger(const vector<NumberEvent>& _data);
	//����Ľ�����������
	bool EndTrigger(const vector<NumberEvent>& _data);

	//��Ҫ��ע�ķ�������
	vector<ProcessGroup> vkeyGroupType;
	//
	//map<ProcessGroup,vector<StockDataType>> ReturnRates;
private:
	//
	void SetKeyGroupType();
	//���ڼ�¼����Ŀ�ʼ�ڵ㣬�����Ӧ����Ŀ�ʼ�ڵ���ڣ�˵����ǰ�����ڶ�Ӧ�ķ���״̬��
	map<ProcessGroup, DayPrice> GroupBeginNodes;
	//����Ľ��
	map<ProcessGroup, vector<ProcessNodes>> GroupRecord;
	//��¼��ǰ�����Ĺ�Ʊ���ݴ��룬�ڳ�ʼ��ʱ��¼
	string stockCode;
};


#endif
