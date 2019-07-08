#include "stdafx.h"


#include "CLocalRecordTool.h"


CLocalRecordTool::CLocalRecordTool():
temporaryLowPointIsPush(false),
temporaryHighPointIsPush(false),
_Parameter_Local(3),
HighIndex(0),
LowIndex(0){}
CLocalRecordTool::~CLocalRecordTool(){}

void CLocalRecordTool::LocalStateRecordSigPoint(	const string& strday,const StockDataType& OneDayData)
{
	//记录第一个点作为局部分析的起点
	if (_CurrentInedxMark == 0)
	{
		temporaryHighPoint._Value = OneDayData;
		temporaryHighPoint._Date.SetDay(strday);
		temporaryHighPoint._TimeIndex = 0;
		temporaryHighPoint._IndexType = _eLocalHigh;
		temporaryLowPoint._Value = OneDayData;
		temporaryLowPoint._Date.SetDay(strday);
		temporaryLowPoint._TimeIndex = 0;
		temporaryLowPoint._IndexType = _eLocalHigh;
	}
	//记录数据到缓期中，用于后期分析
	StatePoint thedata;
	thedata._TimeIndex = _CurrentInedxMark;
	thedata._Value = OneDayData;
	thedata._Date.SetDay(strday);
	_lTempValue.push_back(thedata);
	if (_lTempValue.size() > _Parameter_Local)
	{
		_lTempValue.pop_front();
	}
	//进行局部特征分析
	if (_lTempValue.size() >= 3)
	{
		LocalStateRecordSinglePoint_Low(strday, OneDayData);
		LocalStateRecordSinglePoint_High(strday, OneDayData);
	}
	_CurrentInedxMark++;
	return;
}
//判断有没有形成局部低点
bool CLocalRecordTool::LocalStateRecordSinglePoint_Low(const string& day,const StockDataType& dayValue)
{
	//判断当前点是否小于记录的低点
	if (dayValue < temporaryLowPoint._Value)
	{
		temporaryLowPoint._Value = dayValue;
		temporaryLowPoint._TimeIndex = _CurrentInedxMark;
		temporaryLowPoint._Date.SetDay(day);
		temporaryLowPoint._IndexType = _eLocalLow;
		return true;
	}
	//超过LOCALSTARTINDEX多个数据之后才开始分析数据局部特征
	if (_CurrentInedxMark < LOCALSTARTINDEX)
		return false;

	//判断是不是局部最低点，如果是则加入到特征记录后方
	if (_CurrentInedxMark - temporaryLowPoint._TimeIndex > _Parameter_Local && !temporaryLowPointIsPush)
	{
		temporaryLowPointIsPush = true;
		temporaryHighPointIsPush = false;

		temporaryLowPoint._IndexType = _eLocalLow;
		_vIndexRecord_Local.push_back(temporaryLowPoint);
		_vLowIndexRecord_Local.push_back(temporaryLowPoint);
		//在历史记录中找新的高点
		temporaryHighPoint = _lTempValue.front();
		for (list<StatePoint>::iterator ite = _lTempValue.begin(); ite != _lTempValue.end(); ite++)
			if (temporaryHighPoint._Value < ite->_Value)
				temporaryHighPoint = *ite;

	}
	return true;
}

bool CLocalRecordTool::LocalStateRecordSinglePoint_High(const string& day,const StockDataType& dayValue)
{
	//判断当前点是否大于记录的高点
	if (dayValue > temporaryHighPoint._Value)
	{
		temporaryHighPoint._Value = dayValue;
		temporaryHighPoint._TimeIndex = _CurrentInedxMark;
		temporaryHighPoint._Date.SetDay(day);
		temporaryHighPoint._IndexType = _eLocalHigh;
		return true;
	}
	//超过LOCALSTARTINDEX多个数据之后才开始分析数据局部特征
	if (_CurrentInedxMark < LOCALSTARTINDEX)
		return false;

	//判断是不是局部最高点，如果是则加入到特征记录后方
	if (_CurrentInedxMark - temporaryHighPoint._TimeIndex > _Parameter_Local && !temporaryHighPointIsPush)
	{
		temporaryHighPointIsPush = true;
		temporaryLowPointIsPush = false;

		temporaryHighPoint._IndexType = _eLocalHigh;
		_vIndexRecord_Local.push_back(temporaryHighPoint);
		_vHighIndexRecord_Local.push_back(temporaryHighPoint);

		//找在历史记录中新的低点
		temporaryLowPoint = _lTempValue.front();
		for (list<StatePoint>::iterator ite = _lTempValue.begin(); ite != _lTempValue.end(); ite++)
			if (temporaryLowPoint._Value > ite->_Value)
				temporaryLowPoint = *ite;
	}
	return true;
}

void CLocalRecordTool::Inition(unsigned int range)
{
	//趋势记录过程中，记录当前的index
	_CurrentInedxMark = 0;
	HighIndex = 0;
	LowIndex = 0;
	// 
	_Parameter_Local = range;
	//这两个设为true，表示temporaryHighPoint和temporaryLowPoint在没有更新之前不可以被push进到记录当中
	temporaryHighPointIsPush = false;
	temporaryLowPointIsPush = false;
	//清除所有的记录
	_vIndexRecord_Local.clear();
	_vLowIndexRecord_Local.clear();
	_vHighIndexRecord_Local.clear();
	_lTempValue.clear();
}

///////////////////////////////////////////////////////////////////////////
//返回总结果
//////////////////////////////////////////////////////////////////////////
const StatePointsList& CLocalRecordTool::GetAllLocalResult()
{
	return _vIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部低点结果
//////////////////////////////////////////////////////////////////////////
const StatePointsList& CLocalRecordTool::GetLowLocalResult()
{
	return _vLowIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部高点结果
//////////////////////////////////////////////////////////////////////////
const StatePointsList& CLocalRecordTool::GetHighLocalResult()
{
	return _vHighIndexRecord_Local;
}

void CLocalRecordTool::LocalStateAnalysis(const vector<string>& _day, const VStockData& _inputdata)
{
	if (_inputdata.size() != _day.size())
		return;
	for (unsigned int i = 0; i < _inputdata.size(); i++)
	{
		LocalStateRecordSigPoint(_day[i], _inputdata[i]);
	}
}


