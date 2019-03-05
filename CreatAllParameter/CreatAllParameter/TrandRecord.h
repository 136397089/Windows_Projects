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
	//输入数据，进行趋势分析
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
	//获得单日低点的趋势
	CharaPointType GetBottonPointTrandResult() const;
	//获得单日高点的趋势
	CharaPointType GetTopPointTrandResult() const ;
	//获得单日综合趋势
	CharaPointType GetCompositeTrandResult()const;
	//
	StatusPointsList _vIndexRecord_Local;

	StatusPointsList _vLowIndexRecord_Local;

	StatusPointsList _vHighIndexRecord_Local;

	//记录综合趋势结果
	StatusPointsList  comtrend;
	//记录底部趋势结果
	StatusPointsList  bottontrend;
	//记录顶部趋势结果
	StatusPointsList  toptrend;

	//当局部高点大于highminsize时才开始高点的趋势分析
	unsigned int highminsize;
	//当局部低点大于lowminsize时才开始低点的趋势分析
	unsigned int lowminsize;
	//当所有局部点大于compminsize时才开始所有的趋势分析
	unsigned int compminsize;
	//连续多少次
	unsigned int frequUplimit;

};

#endif
