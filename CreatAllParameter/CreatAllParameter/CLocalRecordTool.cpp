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
	//��¼����һ������ݵ��б��У����ں��ڷ���
	StatePoint thedata;
	thedata._TimeIndex = _CurrentMark;
	thedata._Value = OneDayData;
	thedata._Date.SetDay(strday);
	_lTempValue.push_back(thedata);

	if (_lTempValue.size() > _Parameter_Local)
	{
		_lTempValue.pop_front();
	}
	//���оֲ���������
	LocalStateRecordSingleStep( strday, OneDayData);
	//��¼��ǰ�㣬��Ϊ�´η���ʱ��ǰһ����
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
	//�������
	if (_lTempValue.size() < 3)
	{
		return;
	}
	LocalStateRecordSingleStep_Low( day, dayValue);
	LocalStateRecordSingleStep_High( day, dayValue);
}
//�ж���û���γɾֲ��͵�
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
	//����LOCALSTARTINDEX�������֮��ſ�ʼ�������ݾֲ�����
	if (_CurrentMark < LOCALSTARTINDEX)
		return false;

	//�ж��ǲ��Ǿֲ���͵㣬���������뵽������¼��
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
	//�ж���û���γɾֲ��ߵ�
	if (dayValue > temporaryHighPoint._Value)
	{
		temporaryHighPoint._Value = dayValue;
		temporaryHighPoint._TimeIndex = _CurrentMark;
		temporaryHighPoint._Date.SetDay(day);

	}
	//����LOCALSTARTINDEX�������֮��ſ�ʼ�������ݾֲ�����
	if (_CurrentMark < LOCALSTARTINDEX)
		return false;

	//�ж��ǲ��Ǿֲ���ߵ㣬���������뵽������¼��
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
	//���Ƽ�¼�����У���¼��ǰ��index
	_CurrentMark = 0;
	// 
	_Parameter_Local = range;

	//��������Ϊtrue����ʾtemporaryHighPoint��temporaryLowPoint��û�и���֮ǰ�����Ա�push������¼����
	temporaryHighPointIsPush = false;
	temporaryLowPointIsPush = false;
	//������еļ�¼
	_vIndexRecord_Local.clear();
	_vLowIndexRecord_Local.clear();
	_vHighIndexRecord_Local.clear();
	_lTempValue.clear();
}

///////////////////////////////////////////////////////////////////////////
//�����ܽ��
//////////////////////////////////////////////////////////////////////////
StatePointsList CLocalRecordTool::GetLocalResult()
{
	return _vIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//���ؾֲ��͵���
//////////////////////////////////////////////////////////////////////////
StatePointsList CLocalRecordTool::GetLowLocalResult()
{
	return _vLowIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//���ؾֲ��ߵ���
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
//������ֲ��������������ϳ�Ϊһ�飬���������͵�ľֲ��������
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




