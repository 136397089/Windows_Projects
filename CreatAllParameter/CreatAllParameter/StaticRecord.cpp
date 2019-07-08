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
	const string& Day,
	const StockDataType& FrontData,
	const StockDataType& BackData)
{
	StatePoint& mfrontStatic = _StateStatic;// [valueType];
	//�ϴ��ж�
	if (mfrontStatic._Value < 0 && (FrontData - BackData) > 0)
	{
		_NowStateChange = true;
		mfrontStatic._Value = FrontData - BackData;
		mfrontStatic._TimeIndex = _CurrentInedx;
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
		mfrontStatic._TimeIndex = _CurrentInedx;
		mfrontStatic._Date.SetDay(Day);
		if (FrontData >= 0)
			mfrontStatic._IndexType = _eStaticPositiveDownClose;
		else
			mfrontStatic._IndexType = _eStaticNegaTiveDownClose;
		_vIndexRecord_State.push_back(mfrontStatic);
	}
	_CurrentInedx++;
	return;
}

void CStaticRecordTool::Inition()
{
	//��̬��¼��ʹ�õĲ���
	_NowStateChange = false;
	_vIndexRecord_State.clear();
	_CurrentInedx = 0;

}

const vector<StatePoint>& CStaticRecordTool::GetStaticState()
{
	return _vIndexRecord_State;
}



