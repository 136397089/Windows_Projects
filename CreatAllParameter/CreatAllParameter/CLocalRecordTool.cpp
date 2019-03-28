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

void CLocalRecordTool::LocalStateRecordSigPoint(
	const string strday,
	const tyStockData& OneDayData)
{
	//记录下这一天的数据到列表中，用于后期分析
	StatePoint thedata;
	thedata._TimeIndex = _CurrentMark;
	thedata._Value = OneDayData;
	thedata._Date.SetDay(strday);
	_lTempValue.push_back(thedata);

	if (_lTempValue.size() > _Parameter_Local)
	{
		_lTempValue.pop_front();
	}
	//进行局部特征分析
	LocalStateRecordSingleStep( strday, OneDayData);
	//记录当前点，作为下次分析时的前一个点
	_frontPoint._Value = OneDayData;
	_frontPoint._TimeIndex = _CurrentMark;
	_frontPoint._Date.SetDay(strday);

	_CurrentMark++;
	return;
}

void CLocalRecordTool::LocalStateRecordSingleStep(
	const string& day,
	const tyStockData& dayValue)
{
	//输入防错
	if (_lTempValue.size() < 3)
	{
		return;
	}
	LocalStateRecordSingleStep_Low( day, dayValue);
	LocalStateRecordSingleStep_High( day, dayValue);
}
//判断有没有形成局部低点
bool CLocalRecordTool::LocalStateRecordSingleStep_Low(
	const string& day,
	const tyStockData& dayValue)
{
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

		StatePoint LocalLowPoint;
		LocalLowPoint._Value = temporaryLowPoint._Value;
		LocalLowPoint._TimeIndex = temporaryLowPoint._TimeIndex;
		LocalLowPoint._Date.SetDay(temporaryLowPoint._Date.GetDay());
		LocalLowPoint._IndexType = _eLocalLow;

		_vIndexRecord_Local.push_back(LocalLowPoint);
		_vLowIndexRecord_Local.push_back(LocalLowPoint);
	}
	return true;
}

bool CLocalRecordTool::LocalStateRecordSingleStep_High(
	const string& day,
	const tyStockData& dayValue)
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

		StatePoint LocalHighPoint;
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
StatePointsList CLocalRecordTool::GetLocalResult()
{
	return _vIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部低点结果
//////////////////////////////////////////////////////////////////////////
StatePointsList CLocalRecordTool::GetLowLocalResult()
{
	return _vLowIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//返回局部高点结果
//////////////////////////////////////////////////////////////////////////
StatePointsList CLocalRecordTool::GetHighLocalResult()
{
	return _vHighIndexRecord_Local;
}

void CLocalRecordTool::SetLocalStateData(const vector<string>& _day, const VStockData& _inputdata)
{
	if (_inputdata.size() != _day.size())
		return;
	temporaryHighPoint._Value = _inputdata[0];
	temporaryHighPoint._Date.SetDay(_day[0]);
	temporaryHighPoint._TimeIndex = 0;
	temporaryHighPoint._IndexType = _eLocalHigh;

	temporaryLowPoint._Value = _inputdata[0];
	temporaryLowPoint._Date.SetDay(_day[0]);
	temporaryLowPoint._TimeIndex = 0;
	temporaryLowPoint._IndexType = _eLocalHigh;

	for (unsigned int i = 0; i < _inputdata.size(); i++)
	{
		LocalStateRecordSigPoint(_day[i], _inputdata[i]);
	}
}
//将两组局部特征点的数据组合成为一组，用于最高最低点的局部特征组合
StatePointsList CLocalRecordTool::LocalResultCombination(
	const StatePointsList& locallist1
	, const StatePointsList& locallist2)
{
	StatePointsList::const_iterator ite1 = locallist1.cbegin();
	StatePointsList::const_iterator ite2 = locallist2.cbegin();
	StatePointsList tempList;
	while (ite1 != locallist1.cend() && ite2 != locallist2.cend())
	{
		if (ite1 == locallist1.cend())
		{
			tempList.push_back(*ite2);
			ite2++;
			continue;
		}
		if (ite2 == locallist2.cend())
		{
			tempList.push_back(*ite1);
			ite1++;
			continue;
		}
		if (ite1->_TimeIndex <= ite2->_TimeIndex)
		{
			tempList.push_back(*ite1);
			ite1++;
			continue;
		}
		if (ite1->_TimeIndex > ite2->_TimeIndex)
		{
			tempList.push_back(*ite2);
			ite2++;
			continue;
		}
	}
	StatePointsList newList;
	for (unsigned int i = 1; i < tempList.size();i++)
	{
		if (tempList[i]._IndexType == _eLocalHigh
			&& tempList[i-1]._IndexType == _eLocalHigh
			&& tempList[i]._TimeIndex - tempList[i - 1]._TimeIndex < 10)
			if (tempList[i]._Value > tempList[i-1]._Value)
				newList.push_back(tempList[i]);
			else
				newList.push_back(tempList[i - 1]);
		if (tempList[i]._IndexType == _eLocalLow 
			&& tempList[i - 1]._IndexType == _eLocalLow 
			&& tempList[i]._TimeIndex - tempList[i-1]._TimeIndex < 10)
			if (tempList[i]._Value < tempList[i-1]._Value)
				newList.push_back(tempList[i]);
			else
				newList.push_back(tempList[i - 1]);
	}


	return newList;
}




