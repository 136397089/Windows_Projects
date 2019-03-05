#pragma once

#ifndef REANDRECORD_H
#define REANDRECORD_H
#include "StatusB.h"

class CTrandRecordTool
{
public:
	CTrandRecordTool();
	~CTrandRecordTool();
	///////////////////////////////////////////////////////////////////////////
	//�������ݣ��������Ʒ���
	///////////////////////////////////////////////////////////////////////////
	StatusPointsList ComTrendState();
	//
	StatusPointsList ButtonTrendState();
	//
	StatusPointsList TopTrendState();
	//
	void SetTrendStatusData(const StatusPointsList& LocalResult);
	//
	void Inition();
private:
	//��õ��յ͵������
	CharaPointType GetBottonPointTrandResult() const;
	//��õ��ոߵ������
	CharaPointType GetTopPointTrandResult() const ;
	//��õ����ۺ�����
	CharaPointType GetCompositeTrandResult()const;
	//
	StatusPointsList _vIndexRecord_Local;

	StatusPointsList _vLowIndexRecord_Local;

	StatusPointsList _vHighIndexRecord_Local;

	//��¼�ۺ����ƽ��
	StatusPointsList  comtrend;
	//��¼�ײ����ƽ��
	StatusPointsList  bottontrend;
	//��¼�������ƽ��
	StatusPointsList  toptrend;

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
