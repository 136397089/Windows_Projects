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
	StatePointsList ComTrendState();
	//
	StatePointsList ButtonTrendState();
	//
	StatePointsList TopTrendState();
	//
	void SetTrendStateData(const StatePointsList& LocalResult);
	//
	void Inition();
private:
	//��õ��յ͵������--
	CharaPointType GetBottonPointTrandResult() const;
	//��õ��ոߵ������
	CharaPointType GetTopPointTrandResult() const ;
	//��õ����ۺ�����
	CharaPointType GetCompositeTrandResult()const;
	//
	StatePointsList _vIndexRecord_Local;

	StatePointsList _vLowIndexRecord_Local;

	StatePointsList _vHighIndexRecord_Local;

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
	//�����оֲ������compminsizeʱ�ſ�ʼ���е����Ʒ���
	unsigned int compminsize;
	//�������ٴ�
	unsigned int frequUplimit;

};

#endif
