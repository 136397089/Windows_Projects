#include "stdafx.h"
#include "StaticRecord.h"


CStaticRecordTool::CStaticRecordTool()
{
	//��̬��¼��ʹ�õĲ���
	_NowStateChange = false;
	_vIndexRecord_State.clear();
}


CStaticRecordTool::~CStaticRecordTool()
{
}

//////////////////////////////////////////////////////////////////////////
//���ؾ�̬�������
//////////////////////////////////////////////////////////////////////////
StatePoint CStaticRecordTool::GetNowStaticState()
{
	return _StateStatic;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CStaticRecordTool::StaticStateRecordSigPoint(
//	const IndexGenre& valueType,
	const string& Day,
	const tyStockData& FrontData,
	const tyStockData& BackData)
{
	//unsigned int& mFrontIndex = _FrontCloseMark_Static[valueType];
	StatePoint& mfrontStatic = _StateStatic;// [valueType];
	//�ϴ��ж�
	if (mfrontStatic._Value < 0 && (FrontData - BackData) > 0)
	{
		_NowStateChange = true;
		mfrontStatic._Value = FrontData - BackData;
		mfrontStatic._TimeIndex = _NextInedx;
		mfrontStatic._Date.SetDay(Day);
		if (FrontData >= 0)
			mfrontStatic._IndexType = _eStaticPositiveUpClose;
		else
			mfrontStatic._IndexType = _eStaticNegaTiveUpClose;
		_vIndexRecord_State.push_back(mfrontStatic);
	}
	//�´��ж�
	else if (mfrontStatic._Value >= 0 && (FrontData - BackData) < 0)
	{
		_NowStateChange = true;
		mfrontStatic._Value = FrontData - BackData;
		mfrontStatic._TimeIndex = _NextInedx;
		mfrontStatic._Date.SetDay(Day);
		if (FrontData >= 0)
			mfrontStatic._IndexType = _eStaticPositiveDownClose;
		else
			mfrontStatic._IndexType = _eStaticNegaTiveDownClose;
		_vIndexRecord_State.push_back(mfrontStatic);
	}
	_NextInedx++;
	return;
}

void CStaticRecordTool::Inition()
{
	//��̬��¼��ʹ�õĲ���
	_NowStateChange = false;
	_vIndexRecord_State.clear();
	_NextInedx = 0;

}

const vector<StatePoint>& CStaticRecordTool::GetStaticState()
{
	return _vIndexRecord_State;
}

bool CStaticRecordTool::SetStaticStateData(
	const vector<string>& _day,
	const VStockData& _frontdata,
	const VStockData& _backdata)
{
	if (_frontdata.size() != _backdata.size())
	{
		return false;
	}
	for (unsigned int i = 0; i < _frontdata.size(); i++)
	{
		StaticStateRecordSigPoint(_day[i], _frontdata[i], _backdata[i]);
	}
	return true;
}


