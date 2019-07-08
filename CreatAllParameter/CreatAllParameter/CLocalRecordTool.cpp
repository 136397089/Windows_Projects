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
	//��¼��һ������Ϊ�ֲ����������
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
	//��¼���ݵ������У����ں��ڷ���
	StatePoint thedata;
	thedata._TimeIndex = _CurrentInedxMark;
	thedata._Value = OneDayData;
	thedata._Date.SetDay(strday);
	_lTempValue.push_back(thedata);
	if (_lTempValue.size() > _Parameter_Local)
	{
		_lTempValue.pop_front();
	}
	//���оֲ���������
	if (_lTempValue.size() >= 3)
	{
		LocalStateRecordSinglePoint_Low(strday, OneDayData);
		LocalStateRecordSinglePoint_High(strday, OneDayData);
	}
	_CurrentInedxMark++;
	return;
}
//�ж���û���γɾֲ��͵�
bool CLocalRecordTool::LocalStateRecordSinglePoint_Low(const string& day,const StockDataType& dayValue)
{
	//�жϵ�ǰ���Ƿ�С�ڼ�¼�ĵ͵�
	if (dayValue < temporaryLowPoint._Value)
	{
		temporaryLowPoint._Value = dayValue;
		temporaryLowPoint._TimeIndex = _CurrentInedxMark;
		temporaryLowPoint._Date.SetDay(day);
		temporaryLowPoint._IndexType = _eLocalLow;
		return true;
	}
	//����LOCALSTARTINDEX�������֮��ſ�ʼ�������ݾֲ�����
	if (_CurrentInedxMark < LOCALSTARTINDEX)
		return false;

	//�ж��ǲ��Ǿֲ���͵㣬���������뵽������¼��
	if (_CurrentInedxMark - temporaryLowPoint._TimeIndex > _Parameter_Local && !temporaryLowPointIsPush)
	{
		temporaryLowPointIsPush = true;
		temporaryHighPointIsPush = false;

		temporaryLowPoint._IndexType = _eLocalLow;
		_vIndexRecord_Local.push_back(temporaryLowPoint);
		_vLowIndexRecord_Local.push_back(temporaryLowPoint);
		//����ʷ��¼�����µĸߵ�
		temporaryHighPoint = _lTempValue.front();
		for (list<StatePoint>::iterator ite = _lTempValue.begin(); ite != _lTempValue.end(); ite++)
			if (temporaryHighPoint._Value < ite->_Value)
				temporaryHighPoint = *ite;

	}
	return true;
}

bool CLocalRecordTool::LocalStateRecordSinglePoint_High(const string& day,const StockDataType& dayValue)
{
	//�жϵ�ǰ���Ƿ���ڼ�¼�ĸߵ�
	if (dayValue > temporaryHighPoint._Value)
	{
		temporaryHighPoint._Value = dayValue;
		temporaryHighPoint._TimeIndex = _CurrentInedxMark;
		temporaryHighPoint._Date.SetDay(day);
		temporaryHighPoint._IndexType = _eLocalHigh;
		return true;
	}
	//����LOCALSTARTINDEX�������֮��ſ�ʼ�������ݾֲ�����
	if (_CurrentInedxMark < LOCALSTARTINDEX)
		return false;

	//�ж��ǲ��Ǿֲ���ߵ㣬���������뵽������¼��
	if (_CurrentInedxMark - temporaryHighPoint._TimeIndex > _Parameter_Local && !temporaryHighPointIsPush)
	{
		temporaryHighPointIsPush = true;
		temporaryLowPointIsPush = false;

		temporaryHighPoint._IndexType = _eLocalHigh;
		_vIndexRecord_Local.push_back(temporaryHighPoint);
		_vHighIndexRecord_Local.push_back(temporaryHighPoint);

		//������ʷ��¼���µĵ͵�
		temporaryLowPoint = _lTempValue.front();
		for (list<StatePoint>::iterator ite = _lTempValue.begin(); ite != _lTempValue.end(); ite++)
			if (temporaryLowPoint._Value > ite->_Value)
				temporaryLowPoint = *ite;
	}
	return true;
}

void CLocalRecordTool::Inition(unsigned int range)
{
	//���Ƽ�¼�����У���¼��ǰ��index
	_CurrentInedxMark = 0;
	HighIndex = 0;
	LowIndex = 0;
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
const StatePointsList& CLocalRecordTool::GetAllLocalResult()
{
	return _vIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//���ؾֲ��͵���
//////////////////////////////////////////////////////////////////////////
const StatePointsList& CLocalRecordTool::GetLowLocalResult()
{
	return _vLowIndexRecord_Local;
}

///////////////////////////////////////////////////////////////////////////
//���ؾֲ��ߵ���
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


