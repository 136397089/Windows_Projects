#pragma once
#ifndef LOCALRECORD_H
#define LOCALRECORD_H

#include "StateB.h"

//�ֲ������������
// enum LocalFeaturePointsType
// {
// 	_eLocalMin,
// 	_eLocalMax,
// 	_eUpCloseZero,
// 	_eDownCloseZero,
// 	_eUndefindType_L
// };
//һ���ֲ�������
// struct FeatPoint_Local
// {
// 	unsigned int _TimeIndex;
// 	string _Day;
// 	tySData _Value;
// 	IndexGenre _IndexGenre;
// 	LocalFeaturePointsType _IndexSig;
// 	FeatPoint_Local()
// 	{
// 		_TimeIndex = 0;
// 		_Day = "";
// 		_Value = 0;
// 		_IndexGenre = _eUnDefine;
// 		_IndexSig = _eUndefindType_L;
// 	}
// };
struct index_and_value
{
	int index;
	string date;
	tyStockData value;
	bool ispush;
};
#define  LOCALSTARTINDEX 60

class CLocalRecordTool
{
public:
	CLocalRecordTool();
	~CLocalRecordTool();
	//////////////////////////////////////////////////////////////////////////
	//��ʼ������������ڲ����ƽ��
	//////////////////////////////////////////////////////////////////////////
	void Inition(unsigned int range = 10);
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ������з���
	//////////////////////////////////////////////////////////////////////////
	void SetLocalStateData(const vector<string>& _day, const VStockData& _data);
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ����
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ��͵���
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetLowLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ��ߵ���
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetHighLocalResult();
private:
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����е��վֲ�����
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSigPoint(
		/*const SigDayTechIndex& dayIndex,*/
		const string strday,
		const tyStockData& OneDayData);	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ��ֲ�����
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tyStockData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���͵����
	//////////////////////////////////////////////////////////////////////////
	bool LocalStateRecordSingleStep_Low(
		/*const IndexGenre& valueType,*/
		const string& day	,
		const tyStockData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���ߵ����
	//////////////////////////////////////////////////////////////////////////
	bool LocalStateRecordSingleStep_High(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tyStockData& dayValue);

	/*
	�ֲ������У��ֲ����������ʷ��¼
	*/
 	//
	unsigned int _Parameter_Local;
	//���ڱ���ֲ�������
	StatePointsList _vIndexRecord_Local;
	StatePointsList _vLowIndexRecord_Local;
	StatePointsList _vHighIndexRecord_Local;
	//���ڱ���������������
	list<StatePoint> _lTempValue;
	//���ڼ�¼��ʱ����ߵ����͵�
	//ֻ�е�����һ�����������ݵ㱻�ж�Ϊ���ڻ��ߵ�����ʱ��ʱ�ſ���ȷ��Ϊ�ֲ���߻������
	StatePoint temporaryHighPoint;
	StatePoint temporaryLowPoint;
	bool temporaryHighPointIsPush;
	bool temporaryLowPointIsPush;
	//////////////////////////////////////////////////////////////////////////
	StatePoint _frontPoint;
	//���շ����У���¼��ǰ��Mark
	unsigned int _CurrentMark;

};

#endif
