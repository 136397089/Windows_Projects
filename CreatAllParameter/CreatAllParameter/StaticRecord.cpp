#include "stdafx.h"
#include "StaticRecord.h"


CStaticRecordTool::CStaticRecordTool()
{
	//静态记录中使用的参数
	_NowStateChange = false;
	_vIndexRecord_State.clear();
}


CStaticRecordTool::~CStaticRecordTool()
{
}

//////////////////////////////////////////////////////////////////////////
//返回静态分析结果
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
	//上穿判断
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
	//下穿判断
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
	//静态记录中使用的参数
	_NowStateChange = false;
	_vIndexRecord_State.clear();
	_CurrentInedx = 0;

}

const vector<StatePoint>& CStaticRecordTool::GetStaticState()
{
	return _vIndexRecord_State;
}



