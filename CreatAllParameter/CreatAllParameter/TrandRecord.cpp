#include "stdafx.h"
#include "TrandRecord.h"


CTrandRecordTool::CTrandRecordTool():
 highminsize(4),
 lowminsize(4),
 frequUplimit(1){}



CTrandRecordTool::~CTrandRecordTool(){}

const StatePointsList& CTrandRecordTool::GetComTrendState()
{
	return  comtrend;
}
void CTrandRecordTool::Inition()
{
	comtrend.clear();
	bottontrend.clear();
	toptrend.clear();
	_vLowIndexRecord.clear();
	_vHighIndexRecord.clear();
}

void CTrandRecordTool::SetTrendStateData(const StatePointsList& LocalResult)
{
	StatePoint tempFeatPoint;
	for (unsigned int i = 0; i < LocalResult.size(); i++)
	{
		tempFeatPoint._Date = LocalResult[i]._Date;
		tempFeatPoint._Value = LocalResult[i]._Value;
		tempFeatPoint._TimeIndex = LocalResult[i]._TimeIndex;

		if (_eLocalLow == LocalResult[i]._IndexType)
		{
			_vLowIndexRecord.push_back(LocalResult[i]);
			tempFeatPoint._IndexType = GetBottonPointTrandResult(_vLowIndexRecord);
			bottontrend.push_back(tempFeatPoint);
		}
		if (_eLocalHigh == LocalResult[i]._IndexType)
		{
			_vHighIndexRecord.push_back(LocalResult[i]);
			tempFeatPoint._IndexType = GetTopPointTrandResult(_vHighIndexRecord);
			toptrend.push_back(tempFeatPoint);
		}
		tempFeatPoint._IndexType = GetCompositeTrandResult();
		comtrend.push_back(tempFeatPoint);
	}

	return;
}

CharaPointType CTrandRecordTool::GetBottonPointTrandResult(const StatePointsList& IndexRecord) const
{
	if (IndexRecord.size() < lowminsize)
	{
		return _eUndefindType;
	}

	//判断是否出现底部上升趋势
	unsigned int frequency = 0;
	for (unsigned int i = IndexRecord.size() - 1; i != 0; i--)
	{
 		if (frequency > frequUplimit)
 			return _eTrendBottomRise;
		if (IndexRecord[i - 1]._Value < IndexRecord[i]._Value)
 		{
 			frequency++;
 			continue;
 		}
		if (IndexRecord[i - 1]._Value > IndexRecord[i]._Value)
 			break;
	}
	//判断是否底部下降趋势
	frequency = 0;
	for (unsigned int i = IndexRecord.size() - 1; i != 0; i--)
	{
 		if (frequency > frequUplimit)
 			return _eTrendBottomFall;
		if (IndexRecord[i - 1]._Value > IndexRecord[i]._Value)
 		{
 			frequency++;
 			continue;
 		}
		if (IndexRecord[i - 1]._Value < IndexRecord[i]._Value)
 			break;
	}
	//无明显趋势
	return _eTrendNoRule;
}

CharaPointType CTrandRecordTool::GetTopPointTrandResult(const StatePointsList& IndexRecord)const
{
	if (IndexRecord.size() < highminsize)
	{
		return _eUndefindType;
	}

	//判断是否出现顶部上升趋势
	unsigned int frequency = 0;
	for (unsigned int i = IndexRecord.size() - 1; i != 0; i--)
	{
		if (frequency > frequUplimit)
			return _eTrendTopRise;
		if (IndexRecord[i - 1]._Value < IndexRecord[i]._Value)
		{
			frequency++;
			continue;
		}
		if (IndexRecord[i - 1]._Value > IndexRecord[i]._Value)
			break;
	}
	//判断是否出现顶部下降趋势
	frequency = 0;
	for (unsigned int i = IndexRecord.size() - 1; i != 0; i--)
	{
		if (frequency > frequUplimit)
			return _eTrendTopFall;
		if (IndexRecord[i - 1]._Value > IndexRecord[i]._Value)
		{
			frequency++;
			continue;
		}
		if (IndexRecord[i - 1]._Value < IndexRecord[i]._Value)
			break;
	}
	//无明显趋势
	return _eTrendNoRule;
}

CharaPointType CTrandRecordTool::GetCompositeTrandResult()
{
	CharaPointType bottomresult = GetBottonPointTrandResult(_vLowIndexRecord);
	CharaPointType topresult = GetTopPointTrandResult(_vHighIndexRecord);

	if (bottomresult == _eTrendBottomRise && topresult == _eTrendTopRise)
		return _eTrendTopRise_BottomRise;
	else if (bottomresult == _eTrendBottomFall && topresult == _eTrendTopFall)
		return _eTrendTopFall_BottomFall;
	else if (bottomresult == _eTrendBottomRise && topresult == _eTrendTopFall)
		return _eTrendRangeShrink;//范围收缩
	else if (bottomresult == _eTrendBottomFall && topresult == _eTrendTopRise)
		return _eTrendRangeEnlarge;//范围扩张
	else
		return _eUndefindType;
}

const StatePointsList& CTrandRecordTool::GetButtonTrendState()
{
	return bottontrend;
}

const StatePointsList& CTrandRecordTool::GetTopTrendState()
{
	return toptrend;
}
