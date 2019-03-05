#include "stdafx.h"


#include "CLocalRecordTool.h"


CLocalRecordTool::CLocalRecordTool():
temporaryLowPointIsPush(false),
temporaryHighPointIsPush(false)
{
}


CLocalRecordTool::~CLocalRecordTool()
{
}

void CLocalRecordTool::LocalStatusRecordSigPoint(
	const string strday,
	const tySData& OneDayData)
{
	_CurrentMark++;
	//记录下这一天的数据到列表中，用于后期分析
	StatusPoint thedata;
	thedata._TimeIndex = _CurrentMark;
	thedata._Value = OneDayData;
	thedata._Date.SetDay(strday);
	_lTempValue.push_back(thedata);



	if (_lTempValue.size() > _Parameter_Local)
	{
		_lTempValue.pop_front();
	}
	//进行局部特征分析
	LocalStatusRecordSingleStep( strday, OneDayData);
	//记录当前点，作为下次分析时的前一个点
	_frontPoint._Value = OneDayData;
	_frontPoint._TimeIndex = _CurrentMark;
	_frontPoint._Date.SetDay(strday);

	return;
}

void CLocalRecordTool::LocalStatusRecordSingleStep(
	const string& day,
	const tySData& dayValue)
{
	//输入防错
	if (_lTempValue.size() < 3)
	{
		return;
	}
	LocalStatusRecordSingleStep_Low( day, dayValue);
	LocalStatusRecordSingleStep_High( day, dayValue);
}

bool CLocalRecordTool::LocalStatusRecordSingleStep_Low(
	const string& day,
	const tySData& dayValue)
{
	//判断有没有形成局部低点
	if (dayValue < temporaryLowPoint._Value)
	{
		temporaryLowPoint._Value = dayValue;
		temporaryLowPoint._TimeIndex = _CurrentMark;
		temporaryLowPoint._Date.SetDay(day);
	}
	//超过LOCALSTARTINDEX多个数据之后才开始分析数据局部特征
	if (_CurrentMark < LOCALSTARTINDEX)
		return false;

	//判断是不是局部最低点，如果是则加入到特征记录后方
	if (_CurrentMark - temporaryLowPoint._TimeIndex > _Parameter_Local && !temporaryLowPointIsPush)
	{
		temporaryLowPointIsPush = true;
		temporaryHighPointIsPush = false;
		temporaryHighPoint._Value = dayValue;
		temporaryHighPoint._TimeIndex = _CurrentMark;
		temporaryHighPoint._Date.SetDay(day);

		StatusPoint LocalLowPoint;
		LocalLowPoint._Value = temporaryLowPoint._Value;
		LocalLowPoint._TimeIndex = temporaryLowPoint._TimeIndex;
		LocalLowPoint._Date.SetDay(temporaryLowPoint._Date.GetDay());
		LocalLowPoint._IndexType = _eLocalLow;

		_vIndexRecord_Local.push_back(LocalLowPoint);
		_vLowIndexRecord_Local.push_back(LocalLowPoint);
	}
	return true;
}

bool CLocalRecordTool::LocalStatusRecordSingleStep_High(
	const string& day,
	const tySData& dayValue)
{
	//判断有没有形成局部高点
	if (dayValue > temporaryHighPoint._Value)
	{
		temporaryHighPoint._Value = dayValue;
		temporaryHighPoint._TimeIndex = _CurrentMark;
		temporaryHighPoint._Date.SetDay(day);

	}
	//超过LOCALSTARTINDEX多个数据之后才开始分析数据局部特征
	if (_CurrentMark < LOCALSTARTINDEX)
		return false;

	//判断是不是局部最高点，如果是则加入到特征记录后方
	if (_CurrentMark - temporaryHighPoint._TimeIndex > _Parameter_Local && !temporaryHighPointIsPush)
	{
		temporaryHighPointIsPush = true;
		temporaryLowPointIsPush = false;
		temporaryLowPoint._Value = dayValue;
		temporaryLowPoint._TimeIndex = _CurrentMark;
		temporaryLowPoint._Date.SetDay(day);

		StatusPoint LocalHighPoint;
		LocalHighPoint._Value = temporaryHighPoint._Value;
		LocalHighPoint._TimeIndex = temporaryHighPoint._TimeIndex;
		LocalHighPoint._Date.SetDay(temporaryHighPoint._Date.GetDay());
		LocalHighPoint._IndexType = _eLocalHigh;

		_vIndexRecord_Local.push_back(LocalHighPoint);
		_vHighIndexRecord_Local.push_back(LocalHighPoint);

	}
	return true;
}

void CLocalRecordTool::Inition(unsigned int range)
{
	//趋势记录过程中，记录当前的index
	_CurrentMark = 0;
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
const StatusPointsList& CLocalRecordTool::GetLocalResult()
{
	return _vIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部低点结果
//////////////////////////////////////////////////////////////////////////
const StatusPointsList& CLocalRecordTool::GetLowLocalResult()
{
	return _vLowIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部高点结果
//////////////////////////////////////////////////////////////////////////
const StatusPointsList& CLocalRecordTool::GetHighLocalResult()
{
	return _vHighIndexRecord_Local;
}

void CLocalRecordTool::SetLocalStatusData(const vector<string>& _day, const VStockData& _data)
{
	if (_data.size() != _day.size())
		return;
	temporaryHighPoint._Value = _data[0];
	temporaryHighPoint._Date.SetDay(_day[0]);
	temporaryHighPoint._TimeIndex = 0;
	temporaryHighPoint._IndexType = _eLocalHigh;

	temporaryLowPoint._Value = _data[0];
	temporaryLowPoint._Date.SetDay(_day[0]);
	temporaryLowPoint._TimeIndex = 0;
	temporaryLowPoint._IndexType = _eLocalHigh;

	for (unsigned int i = 0; i < _data.size(); i++)
	{
		LocalStatusRecordSigPoint(_day[i], _data[i]);
	}
}




