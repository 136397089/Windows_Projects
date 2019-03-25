#include "stdafx.h"
#include "StateInter.h"
#include "glog/logging.h"

CStateInter::CStateInter()
{
}


CStateInter::~CStateInter()
{
}

//������ָ��״̬���з���
void CStateInter::Inter(const StockDataTable& allnumber,const string filename)
{
	StockDataPointer pointers = allnumber.GetNumberPointer();
	allIndexStates.clear();
	VStockData emptydate;
	emptydate.clear();
	_vTimeDay.clear();
	_vTimeDay = allnumber._vTimeDay;
	allIndexStates[_eFile_High_INDEX] = GetOneNumberState(allnumber._vHigh, emptydate);
	allIndexStates[_eFile_Low_INDEX] = GetOneNumberState(allnumber._vLow, emptydate);

	allIndexStates[_eMACD_DIFF] = GetOneNumberState(allnumber._vDiff, emptydate);
	allIndexStates[_eMACD_DEA] = GetOneNumberState(allnumber._vDiff, emptydate);
	allIndexStates[_eMACD_BAR] = GetOneNumberState( allnumber._vMACDValue, emptydate);
	allIndexStates[_eKDJ_J] = GetOneNumberState( allnumber._vJ, allnumber._vD);
	allIndexStates[_eASI_I] = GetOneNumberState( allnumber._vAsi_i, emptydate);
	allIndexStates[_eMA_MA5] = GetOneNumberState( allnumber._vMa1, allnumber._vMa2);

	return;
}


//����ָ������з������
StateTable CStateInter::GetOneNumberState( const VStockData& _datafast, const VStockData& _dataslow)
{
	StateTable _statetable;
	bool isTwoPieceOfData = false;
	if (_datafast.size() <= 10 || _vTimeDay.size() != _datafast.size())
		return _statetable;
	if (_datafast.size() == _dataslow.size())
		isTwoPieceOfData = true;
	//��̬����
	CStaticRecordTool staticrecord;
	staticrecord.Inition();
	for (unsigned int i = 0; i < _datafast.size(); i++)
	{
		if (isTwoPieceOfData)
			staticrecord.StaticStateRecordSigPoint(_vTimeDay[i], _datafast[i], _dataslow[i]);
		else
			staticrecord.StaticStateRecordSigPoint(_vTimeDay[i], _datafast[i], 0);
	}
	_statetable._staticstate = staticrecord.GetStaticState();
	//��̬�ֶλ���
	CStaticIntegral IngTool;
	IngTool.Inition();
	IngTool.GetsStaticIng(_datafast, _statetable._staticstate, _statetable._staticIng);
	//�ֲ�����
	CLocalRecordTool localRecord;
	localRecord.Inition();
	localRecord.SetLocalStateData(_vTimeDay, _datafast);
	_statetable._localstate = localRecord.GetLocalResult();
	//���Ʒ���
	CTrandRecordTool trandrecord;
	trandrecord.Inition();
	trandrecord.SetTrendStateData(_statetable._localstate);
	_statetable._trendstate = trandrecord.ComTrendState();

	return _statetable;
}

void CStateInter::GetStateByIndex(unsigned int index)
{

}

