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
	//返回静态分析结果
	//////////////////////////////////////////////////////////////////////////
	StatePoint GetNowStaticState();
	const vector<StatePoint>& GetStaticState();
	//////////////////////////////////////////////////////////////////////////
	//进行单日单指标静态分析
	//////////////////////////////////////////////////////////////////////////
	void StaticStateRecordSigPoint(const string& Day,const StockDataType& FrontData,const StockDataType& BackData);
	//////////////////////////////////////////////////////////////////////////
	//初始化，清除所有内部趋势结果
	//////////////////////////////////////////////////////////////////////////
	void Inition();
	//当前的静态特征
	StatePoint _StateStatic;
	//一般情况下为false,如果当前的静态特征点已经被修改则设为true
	//如果静态特征点被修改过则会后面将当前的静态特征点加入到_vIndexRecord_State,再将其置为false
	bool _NowStateChange;
	//
	vector<StatePoint> _vIndexRecord_State;
	//单日分析中，记录当前的Mark
	unsigned int _CurrentInedx;

};

#endif