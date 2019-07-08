#pragma once

#ifndef REANDRECORD_H
#define REANDRECORD_H
#include "StateB.h"

class CTrandRecordTool
{
public:
	CTrandRecordTool();
	~CTrandRecordTool();
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ��������Ʒ���
	///////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetComTrendState();
	//
	const StatePointsList& GetButtonTrendState();
	//
	const StatePointsList& GetTopTrendState();
	//
	void SetTrendStateData(const StatePointsList& LocalResult);
	//
	void Inition();
private:
	//��õ��յ͵������
	CharaPointType GetBottonPointTrandResult(const StatePointsList& IndexRecord) const;
	//��õ��ոߵ������
	CharaPointType GetTopPointTrandResult(const StatePointsList& IndexRecord) const;
	//��õ����ۺ�����
	CharaPointType GetCompositeTrandResult();
	//
	StatePointsList _vLowIndexRecord;
	//
	StatePointsList _vHighIndexRecord;
	//��¼�ۺ����ƽ��
	StatePointsList  comtrend;
	//��¼�ײ����ƽ��
	StatePointsList  bottontrend;
	//��¼�������ƽ��
	StatePointsList  toptrend;
	//���ֲ��ߵ����highminsizeʱ�ſ�ʼ�ߵ�����Ʒ���
	unsigned int highminsize;
	//���ֲ��͵����lowminsizeʱ�ſ�ʼ�͵�����Ʒ���
	unsigned int lowminsize;
	//�������ٴ�
	unsigned int frequUplimit;
};

#endif
