#pragma once
#ifndef LOCALRECORD_H
#define LOCALRECORD_H

#include "StateB.h"

//局部特征点的类型
// enum LocalFeaturePointsType
// {
// 	_eLocalMin,
// 	_eLocalMax,
// 	_eUpCloseZero,
// 	_eDownCloseZero,
// 	_eUndefindType_L
// };
//一个局部特征点
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
	//初始化，清除所有内部趋势结果
	//////////////////////////////////////////////////////////////////////////
	void Inition(unsigned int range = 10);
	///////////////////////////////////////////////////////////////////////////
	//输入数据，并进行分析
	//////////////////////////////////////////////////////////////////////////
	void SetLocalStateData(const vector<string>& _day, const VStockData& _data);
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部结果
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部低点结果
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetLowLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部高点结果
	//////////////////////////////////////////////////////////////////////////
	const StatePointsList& GetHighLocalResult();
private:
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行单日局部分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSigPoint(
		/*const SigDayTechIndex& dayIndex,*/
		const string strday,
		const tyStockData& OneDayData);	//////////////////////////////////////////////////////////////////////////
	//进行单日单指标局部分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStateRecordSingleStep(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tyStockData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最低点分析
	//////////////////////////////////////////////////////////////////////////
	bool LocalStateRecordSingleStep_Low(
		/*const IndexGenre& valueType,*/
		const string& day	,
		const tyStockData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最高点分析
	//////////////////////////////////////////////////////////////////////////
	bool LocalStateRecordSingleStep_High(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tyStockData& dayValue);

	/*
	局部分析中，局部特征点的历史记录
	*/
 	//
	unsigned int _Parameter_Local;
	//用于保存局部特征点
	StatePointsList _vIndexRecord_Local;
	StatePointsList _vLowIndexRecord_Local;
	StatePointsList _vHighIndexRecord_Local;
	//用于保存最近几天的数据
	list<StatePoint> _lTempValue;
	//用于记录临时的最高点和最低点
	//只有当后面一定数量的数据点被判断为高于或者低于临时点时才可以确定为局部最高或者最低
	StatePoint temporaryHighPoint;
	StatePoint temporaryLowPoint;
	bool temporaryHighPointIsPush;
	bool temporaryLowPointIsPush;
	//////////////////////////////////////////////////////////////////////////
	StatePoint _frontPoint;
	//单日分析中，记录当前的Mark
	unsigned int _CurrentMark;

};

#endif
