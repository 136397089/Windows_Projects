#pragma once
#ifndef LOCALRECORD_H
#define LOCALRECORD_H

#include "StatusB.h"

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
	//初始化，清除所有内部趋势结果
	//////////////////////////////////////////////////////////////////////////
	void Inition(unsigned int range = 10);
	///////////////////////////////////////////////////////////////////////////
	//输入数据，并进行分析
	//////////////////////////////////////////////////////////////////////////
	void SetLocalStatusData(const vector<string>& _day, const VStockData& _data);
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部结果
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部低点结果
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetLowLocalResult();
	///////////////////////////////////////////////////////////////////////////
	//获取结果，返回局部高点结果
	//////////////////////////////////////////////////////////////////////////
	const StatusPointsList& GetHighLocalResult();
private:
	///////////////////////////////////////////////////////////////////////////
	//输入数据，进行单日局部分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStatusRecordSigPoint(
		/*const SigDayTechIndex& dayIndex,*/
		const string strday,
		const tySData& OneDayData);	//////////////////////////////////////////////////////////////////////////
	//进行单日单指标局部分析
	//////////////////////////////////////////////////////////////////////////
	void LocalStatusRecordSingleStep(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tySData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最低点分析
	//////////////////////////////////////////////////////////////////////////
	bool LocalStatusRecordSingleStep_Low(
		/*const IndexGenre& valueType,*/
		const string& day	,
		const tySData& dayValue);
	//////////////////////////////////////////////////////////////////////////
	//单日单指标局部分析中的局部最高点分析
	//////////////////////////////////////////////////////////////////////////
	bool LocalStatusRecordSingleStep_High(
		/*const IndexGenre& valueType,*/
		const string& day,
		const tySData& dayValue);

	/*
	局部分析中，局部特征点的历史记录
	*/
 	//
	unsigned int _Parameter_Local;
	//用于保存局部特征点
	StatusPointsList _vIndexRecord_Local;
	StatusPointsList _vLowIndexRecord_Local;
	StatusPointsList _vHighIndexRecord_Local;
	//用于保存最近几天的数据
	list<StatusPoint> _lTempValue;
	//用于记录临时的最高点和最低点
	//只有当后面一定数量的数据点被判断为高于或者低于临时点时才可以确定为局部最高或者最低
	StatusPoint temporaryHighPoint;
	StatusPoint temporaryLowPoint;
	bool temporaryHighPointIsPush;
	bool temporaryLowPointIsPush;
	//////////////////////////////////////////////////////////////////////////
	StatusPoint _frontPoint;
	//单日分析中，记录当前的Mark
	unsigned int _CurrentMark;

};

#endif
