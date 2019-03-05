#pragma once
#ifndef LOCALRECORD_H
#define LOCALRECORD_H

#include "StatusB.h"

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
	tySData value;
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
	void SetLocalStatusData(const vector<string>& _day, const VStockData& _data);
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ����
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ��͵���
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetLowLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//��ȡ��������ؾֲ��ߵ���
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetHighLocalResult();
private:
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ����е��վֲ�����
	//////////////////////////////////////////////////////////////////////////
	void LocalStatusRecordSigPoint(
		/*const SigDayTechIndex& dayIndex,*/
		const string strday,
		const tySData& OneDayData);	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ��ֲ�����
	//////////////////////////////////////////////////////////////////////////
	void LocalStatusRecordSingleStep(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tySData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���͵����
	//////////////////////////////////////////////////////////////////////////
	bool LocalStatusRecordSingleStep_Low(
		/*const IndexGenre& valueType,*/
		const string& day	,
		const tySData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//���յ�ָ��ֲ������еľֲ���ߵ����
	//////////////////////////////////////////////////////////////////////////
	bool LocalStatusRecordSingleStep_High(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tySData& dayValue);

	/*
	�ֲ������У��ֲ����������ʷ��¼
	*/
 	//
	unsigned int _Parameter_Local;
	//���ڱ���ֲ�������
	StatusPointsList _vIndexRecord_Local;
	StatusPointsList _vLowIndexRecord_Local;
	StatusPointsList _vHighIndexRecord_Local;
	//���ڱ���������������
	list<StatusPoint> _lTempValue;
	//���ڼ�¼��ʱ����ߵ����͵�
	//ֻ�е�����һ�����������ݵ㱻�ж�Ϊ���ڻ��ߵ�����ʱ��ʱ�ſ���ȷ��Ϊ�ֲ���߻������
	StatusPoint temporaryHighPoint;
	StatusPoint temporaryLowPoint;
	bool temporaryHighPointIsPush;
	bool temporaryLowPointIsPush;
	//////////////////////////////////////////////////////////////////////////
	StatusPoint _frontPoint;
	//���շ����У���¼��ǰ��Mark
	unsigned int _CurrentMark;

};

#endif
