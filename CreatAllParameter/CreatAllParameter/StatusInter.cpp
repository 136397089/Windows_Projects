#include "stdafx.h"
#include "StatusInter.h"
#include "glog/logging.h"

CStatusInter::CStatusInter()
{
}


CStatusInter::~CStatusInter()
{
}

//������ָ��״̬���з���
void CStatusInter::Inter(const AllStockData& allnumber,const string filename)
{
	StockDataPointer pointers = allnumber.GetIndexPointer();
	allIndexStatus.clear();
	allIndexStatus[_eMACD_DEA] = GetOneNumberStatus(allnumber._vTimeDay, allnumber._vDEA);
	allIndexStatus[_eMACD_DIFF] = GetOneNumberStatus(allnumber._vTimeDay, allnumber._vDiff);
	// 	for (map<IndexType, const VStockData*>::const_iterator ite = pointers.begin(); ite != pointers.end(); ite++)
// 	{
// 		allIndexStatus[ite->first] = GetOneNumberStatus(allnumber._vTimeDay, *(ite->second));
// 	}

	return;
}


//����ָ������з������
AllStatus CStatusInter::GetOneNumberStatus(const vector<string>& _day, const VStockData& _data)
{
	AllStatus tempstatus;
	//��̬����
	CStaticRecordTool staticrecord;
	staticrecord.Inition();
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		staticrecord.StaticStatusRecordSigPoint(_day[i], _data[i], 0);
	}
	tempstatus._staticstatus = staticrecord.GetStaticStatus();
	//��̬�ֶλ���
	CStaticIntegral IngTool;
	IngTool.Inition();
	IngTool.GetsStaticIng(_data, tempstatus._staticstatus, tempstatus._staticIng);
	//�ֲ�����
	CLocalRecordTool localrecord;
	localrecord.Inition();
	localrecord.SetLocalStatusData(_day, _data);
	tempstatus._localstatus = localrecord.GetLocalResult();
	//���Ʒ���
	CTrandRecordTool trandrecord;
	trandrecord.Inition();
	trandrecord.SetTrendStatusData(tempstatus._localstatus);
	tempstatus._trendstatus = trandrecord.ButtonTrendState();

	return tempstatus;
}

void CStatusInter::GetStatusByIndex(unsigned int index)
{

}

