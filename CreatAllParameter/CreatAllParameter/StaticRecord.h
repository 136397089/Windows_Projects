#pragma once
#ifndef STATICRECORD_H
#define STATICRECORD_H

#include "StatusB.h"
class CStaticRecordTool
{
public:
	CStaticRecordTool();
	~CStaticRecordTool();

	//////////////////////////////////////////////////////////////////////////
	//���ؾ�̬�������
	//////////////////////////////////////////////////////////////////////////
	StatusPoint GetNowStaticStatus();

	const vector<StatusPoint>& GetStaticStatus();
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ�꾲̬����
	//////////////////////////////////////////////////////////////////////////
	bool SetStaticStatusData(
		const vector<string>& _day,
		const VStockData& _frontdata,
		const VStockData& _backdata);
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ�꾲̬����
	//////////////////////////////////////////////////////////////////////////
	void StaticStatusRecordSigPoint(
		const string& Day,
		const tySData& FrontData,
		const tySData& BackData);
	//////////////////////////////////////////////////////////////////////////
	//��ʼ������������ڲ����ƽ��
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//��ǰ�ľ�̬����
	StatusPoint _StatusStatic;// [MaxSizeOfIndex];
	//һ�������Ϊfalse,�����ǰ�ľ�̬�������Ѿ����޸�����Ϊtrue
	//�����̬�����㱻�޸Ĺ������潫��ǰ�ľ�̬��������뵽_vIndexRecord_State,�ٽ�����Ϊfalse
	bool _NowStateChange;
	//
	vector<StatusPoint> _vIndexRecord_State;
	//���շ����У���¼��ǰ��Mark
	unsigned int _CurrentMark;

};

#endif