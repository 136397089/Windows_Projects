#pragma once
#ifndef STATICRECORD_H
#define STATICRECORD_H

#include "StateB.h"
class CStaticRecordTool
{
public:
	CStaticRecordTool();
	~CStaticRecordTool();

	//////////////////////////////////////////////////////////////////////////
	//���ؾ�̬�������
	//////////////////////////////////////////////////////////////////////////
	StatePoint GetNowStaticState();

	const vector<StatePoint>& GetStaticState();
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ�꾲̬����
	//////////////////////////////////////////////////////////////////////////
	bool SetStaticStateData(
		const vector<string>& _day,
		const VStockData& _frontdata,
		const VStockData& _backdata);
	//////////////////////////////////////////////////////////////////////////
	//���е��յ�ָ�꾲̬����
	//////////////////////////////////////////////////////////////////////////
	void StaticStateRecordSigPoint(
		const string& Day,
		const StockDataType& FrontData,
		const StockDataType& BackData);
	//////////////////////////////////////////////////////////////////////////
	//��ʼ������������ڲ����ƽ��
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//��ǰ�ľ�̬����
	StatePoint _StateStatic;// [MaxSizeOfIndex];
	//һ�������Ϊfalse,�����ǰ�ľ�̬�������Ѿ����޸�����Ϊtrue
	//�����̬�����㱻�޸Ĺ������潫��ǰ�ľ�̬��������뵽_vIndexRecord_State,�ٽ�����Ϊfalse
	bool _NowStateChange;
	//
	vector<StatePoint> _vIndexRecord_State;
	//���շ����У���¼��ǰ��Mark
	unsigned int _CurrentInedx;

};

#endif