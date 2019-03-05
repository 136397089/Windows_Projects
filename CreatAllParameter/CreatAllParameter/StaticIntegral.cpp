#include "stdafx.h"
#include "StaticIntegral.h"


CStaticIntegral::CStaticIntegral()
{
}


CStaticIntegral::~CStaticIntegral()
{
}

void CStaticIntegral::Inition()
{
	_LastError = "";
}


bool CStaticIntegral::GetsStaticIng(
	const VStockData& _data,
	const StatusPointsList& _staticPoint,
	StatusPointsList& _staticIng)
{
	//����������
	if (_staticPoint.size() < 1)
	{
		_LastError = "static point size error.";
		return false;
	}
	if (_data.size() < 2)
	{
		_LastError = "data size is less then 2.";
		return false;
	}
	StatusPoint LastStaticpoint = _staticPoint[_staticPoint.size() - 1];
	unsigned int dataMaxIndex = _data.size() - 1;
	if (LastStaticpoint._TimeIndex > dataMaxIndex)
	{
		_LastError = "Static point index error.";
		return false;
	}
	//��ʼ������������
	_staticIng.clear();
	unsigned int _FrontIndex = 0;
	unsigned int _BackIndex = 0;
	StatusPoint tempstatusPoint;
	for (unsigned int i = 0; i < _staticPoint.size();i++)
	{
		_BackIndex = _FrontIndex;
		_FrontIndex = _staticPoint[i]._TimeIndex;

		tempstatusPoint._Value = 0;
		tempstatusPoint._Date = _staticPoint[i]._Date;
		tempstatusPoint._TimeIndex = _FrontIndex;
		tempstatusPoint._OtherSideIndex = _BackIndex;

		if (_BackIndex > dataMaxIndex || _FrontIndex > dataMaxIndex || _BackIndex > _FrontIndex)
		{//�±����:����ľ�̬������index������_data�����index,���ߺ���ľ�̬������index����ǰ�澲̬�������index
			_LastError = "Frontindex or backinedx error.";
			return false;
		}
		for (unsigned int j = _BackIndex; j < _FrontIndex; j++)
			tempstatusPoint._Value = tempstatusPoint._Value + _data[j];
		_staticIng.push_back(tempstatusPoint);
	}
	return true;
}
