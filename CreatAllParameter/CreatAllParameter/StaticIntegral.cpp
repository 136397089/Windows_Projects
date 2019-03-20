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
	const StatePointsList& _staticPoint,
	StatePointsList& _staticIng)
{
	//输入参数检查
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
	StatePoint LastStaticpoint = _staticPoint[_staticPoint.size() - 1];
	unsigned int dataMaxIndex = _data.size() - 1;
	if (LastStaticpoint._TimeIndex > dataMaxIndex)
	{
		_LastError = "Static point index error.";
		return false;
	}
	//开始对数据做积分
	_staticIng.clear();
	unsigned int _FrontIndex = 0;
	unsigned int _BackIndex = 0;
	StatePoint tempstatePoint;
	for (unsigned int i = 0; i < _staticPoint.size();i++)
	{
		_BackIndex = _FrontIndex;
		_FrontIndex = _staticPoint[i]._TimeIndex;

		tempstatePoint._Value = 0;
		tempstatePoint._Date = _staticPoint[i]._Date;
		tempstatePoint._TimeIndex = _FrontIndex;
		tempstatePoint._OtherSideIndex = _BackIndex;

		if (_BackIndex > dataMaxIndex || _FrontIndex > dataMaxIndex || _BackIndex > _FrontIndex)
		{//下标出错:输入的静态特征点index超出了_data的最大index,或者后面的静态特征点index大于前面静态特征点的index
			_LastError = "Frontindex or backinedx error.";
			return false;
		}
		for (unsigned int j = _BackIndex; j < _FrontIndex; j++)
			tempstatePoint._Value = tempstatePoint._Value + _data[j];
		_staticIng.push_back(tempstatePoint);
	}
	return true;
}
