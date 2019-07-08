#include "stdafx.h"
#include "StateInter.h"
#include "glog/logging.h"

CStateAnalysisInter::CStateAnalysisInter(){}
CStateAnalysisInter::~CStateAnalysisInter(){}

//进行状态分析
void CStateAnalysisInter::Inter(StockDataTable& allnumber, const string filename)
{
	allIndexStates.clear();
	VStockData emptydate;
	emptydate.clear();
	_vTimeDay = &allnumber._vTimeDay;
	StateTable _statetable;
	allIndexStates[_eASI_I] = _statetable;
	GetOneNumberState(allnumber._vTableAllIndex[_eASI_I], emptydate, allIndexStates[_eASI_I]);
	return;
}


//单个指标的所有分析结果
bool CStateAnalysisInter::GetOneNumberState(const VStockData& _datafast, const VStockData& _dataslow,StateTable& _statetable)
{
	//输入参数检查
	if (_datafast.size() <= 10 || _vTimeDay->size() != _datafast.size())
		return false;
	//
	bool isTwoPieceOfData = false;
	if (_datafast.size() == _dataslow.size())
		isTwoPieceOfData = true;
	//静态分析
	staticrecord.Inition();
	for (unsigned int i = 0; i < _datafast.size(); i++)
		if (isTwoPieceOfData)
			staticrecord.StaticStateRecordSigPoint((*_vTimeDay)[i], _datafast[i], _dataslow[i]);
		else
			staticrecord.StaticStateRecordSigPoint((*_vTimeDay)[i], _datafast[i], 0);
	_statetable._staticstate = staticrecord.GetStaticState();
	//静态分段积分
	IngTool.Inition();
	IngTool.GetsStaticIntegral(_datafast, _statetable._staticstate, _statetable._staticIng);
	//局部分析
	LocalPartRecord.Inition();
	LocalPartRecord.LocalStateAnalysis(*_vTimeDay, _datafast);
	_statetable._localstate = LocalPartRecord.GetAllLocalResult();
	//趋势分析
	trandrecord.Inition();
	trandrecord.SetTrendStateData(_statetable._localstate);
	_statetable._trendstate = trandrecord.GetComTrendState();

	return true;
}


StateIterationAnalysis::StateIterationAnalysis():
FrontLowLocalSize(0),
FrontLocalSizeVR(0),
StateIsOK(false)
{
}

StateIterationAnalysis::~StateIterationAnalysis()
{
}

bool StateIterationAnalysis::IterationSingleData(const SigDayTechIndex& data, const string& date)
{
	StaticRecordVR.StaticStateRecordSigPoint(date, data._ArBrVrPsy.VR, 0);
	LocalRecordVR.LocalStateRecordSigPoint(date, data._ArBrVrPsy.VR);
	if (FrontLocalSizeVR != LocalRecordVR.GetAllLocalResult().size())
	{
		TrandRecordVR.Inition();
		TrandRecordVR.SetTrendStateData(LocalRecordVR.GetAllLocalResult());
		FrontLocalSizeVR = LocalRecordVR.GetAllLocalResult().size();
	}
	StaticRecordLow.StaticStateRecordSigPoint(date, data._Low,0);
	LocalRecordLow.LocalStateRecordSigPoint(date, data._Low);
	if (FrontLowLocalSize != LocalRecordLow.GetAllLocalResult().size())
	{
		TrandRecordLow.Inition();
		TrandRecordLow.SetTrendStateData(LocalRecordLow.GetAllLocalResult());
		FrontLowLocalSize = LocalRecordLow.GetAllLocalResult().size();
	}
	if ((!TrandRecordLow.GetButtonTrendState().empty())
		&& TrandRecordLow.GetButtonTrendState().back()._IndexType == _eTrendBottomRise
		&& !TrandRecordVR.GetButtonTrendState().empty()
		&& TrandRecordVR.GetButtonTrendState().back()._IndexType == _eTrendBottomRise)
		StateIsOK = true;
	else
		StateIsOK = false;

	return true;
}

void StateIterationAnalysis::Inition(const string & _stockCode)
{
	//静态分析
	StaticRecordVR.Inition();
	StaticRecordLow.Inition();
	//静态分段积分
	IngRecordDiff.Inition();
	IngRecordLow.Inition();
	//局部分析
	LocalRecordVR.Inition();
	LocalRecordLow.Inition();
	//趋势分析
	TrandRecordVR.Inition();
	TrandRecordLow.Inition();
	stockCode = _stockCode;
}

void StateIterationAnalysis::PrintResult()
{
	const StatePointsList& LowPoints = LocalRecordVR.GetLowLocalResult();
	LOG(INFO) << stockCode << " diff Low Points:";
	for (unsigned int i = 0; i < LowPoints.size();i++)
	{
		LOG(INFO) << LowPoints[i]._Date.GetDay();
	}
	const StatePointsList& LowPricePoints = LocalRecordLow.GetLowLocalResult();
	LOG(INFO) << stockCode << " Low Points:";
	for (unsigned int i = 0; i < LowPricePoints.size(); i++)
	{
		LOG(INFO) << LowPricePoints[i]._Date.GetDay();
	}

}

bool StateIterationAnalysis::CheckBeginState()
{
	return StateIsOK;
}
