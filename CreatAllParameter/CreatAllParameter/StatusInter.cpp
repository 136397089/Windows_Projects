#include "stdafx.h"
#include "StatusInter.h"
#include "glog/logging.h"

CStatusInter::CStatusInter()
{
}


CStatusInter::~CStatusInter()
{
}

//对所有指标状态进行分析
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


//单个指标的所有分析结果
AllStatus CStatusInter::GetOneNumberStatus(const vector<string>& _day, const VStockData& _data)
{
	AllStatus tempstatus;
	//静态分析
	CStaticRecordTool staticrecord;
	staticrecord.Inition();
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		staticrecord.StaticStatusRecordSigPoint(_day[i], _data[i], 0);
	}
	tempstatus._staticstatus = staticrecord.GetStaticStatus();
	//静态分段积分
	CStaticIntegral IngTool;
	IngTool.Inition();
	IngTool.GetsStaticIng(_data, tempstatus._staticstatus, tempstatus._staticIng);
	//局部分析
	CLocalRecordTool localrecord;
	localrecord.Inition();
	localrecord.SetLocalStatusData(_day, _data);
	tempstatus._localstatus = localrecord.GetLocalResult();
	//趋势分析
	CTrandRecordTool trandrecord;
	trandrecord.Inition();
	trandrecord.SetTrendStatusData(tempstatus._localstatus);
	tempstatus._trendstatus = trandrecord.ButtonTrendState();

	return tempstatus;
}

void CStatusInter::GetStatusByIndex(unsigned int index)
{

}

