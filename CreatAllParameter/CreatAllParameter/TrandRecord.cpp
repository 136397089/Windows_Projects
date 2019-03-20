#include "stdafx.h"
#include "TrandRecord.h"


CTrandRecordTool::CTrandRecordTool():
 highminsize(4),
 lowminsize(4),
 compminsize(6),
 frequUplimit(1)
{

}



CTrandRecordTool::~CTrandRecordTool()
{
}

StatePointsList CTrandRecordTool::ComTrendState()
{
	return  comtrend;
}
void CTrandRecordTool::Inition()
{
	comtrend.clear();
	_vIndexRecord_Local.clear();
	_vLowIndexRecord_Local.clear();
	_vHighIndexRecord_Local.clear();
}

void CTrandRecordTool::SetTrendStateData(const StatePointsList& LocalResult)
{
	StatePoint tempFeatPoint;

	for (unsigned int i = 0; i < LocalResult.size(); i++)
	{
		tempFeatPoint._Date = LocalResult[i]._Date;
		tempFeatPoint._Value = LocalResult[i]._Value;
		tempFeatPoint._TimeIndex = LocalResult[i]._TimeIndex;

		_vIndexRecord_Local.push_back(LocalResult[i]);
		if (_eLocalLow == LocalResult[i]._IndexType)
		{
			_vLowIndexRecord_Local.push_back(LocalResult[i]);
			tempFeatPoint._IndexType = GetBottonPointTrandResult();
			bottontrend.push_back(tempFeatPoint);
		}
		if (_eLocalHigh == LocalResult[i]._IndexType)
		{
			_vHighIndexRecord_Local.push_back(LocalResult[i]);
			tempFeatPoint._IndexType = GetTopPointTrandResult();
			toptrend.push_back(tempFeatPoint);
		}

		tempFeatPoint._IndexType = GetCompositeTrandResult();
		comtrend.push_back(tempFeatPoint);
	}

	return;
}

CharaPointType CTrandRecordTool::GetBottonPointTrandResult() const
{
	if (_vLowIndexRecord_Local.size() < lowminsize)
	{
		return _eUndefindType;
	}

	//判断是否出现底部上升趋势
	unsigned int frequency = 0;
	for (unsigned int i = _vLowIndexRecord_Local.size()-1; i != 0;i--)
	{
 		if (frequency > frequUplimit)
 			return _eTrendBottomRise;
		if (_vLowIndexRecord_Local[i - 1]._Value < _vLowIndexRecord_Local[i]._Value)
 		{
 			frequency++;
 			continue;
 		}
		if (_vLowIndexRecord_Local[i - 1]._Value > _vLowIndexRecord_Local[i]._Value)
 			break;
	}
	//判断是否底部下降趋势
	frequency = 0;
	for (unsigned int i = _vLowIndexRecord_Local.size()-1; i != 0; i--)
	{
 		if (frequency > frequUplimit)
 			return _eTrendBottomFall;
		if (_vLowIndexRecord_Local[i - 1]._Value > _vLowIndexRecord_Local[i]._Value)
 		{
 			frequency++;
 			continue;
 		}
		if (_vLowIndexRecord_Local[i - 1]._Value < _vLowIndexRecord_Local[i]._Value)
 			break;
	}


	//无明显趋势
	return _eTrendNoRule;
}

CharaPointType CTrandRecordTool::GetTopPointTrandResult()const
{
	if (_vHighIndexRecord_Local.size() < highminsize)
	{
		return _eUndefindType;
	}

	//判断是否出现顶部上升趋势
	unsigned int frequency = 0;
	for (unsigned int i = _vHighIndexRecord_Local.size()-1; i != 0; i--)
	{
		if (frequency >= frequUplimit)
			return _eTrendTopRise;
		if (_vHighIndexRecord_Local[i - 1]._Value < _vHighIndexRecord_Local[i]._Value)
		{
			frequency++;
			continue;
		}
		if (_vHighIndexRecord_Local[i - 1]._Value > _vHighIndexRecord_Local[i]._Value)
			break;
	}
	//判断是否出现顶部下降趋势
	frequency = 0;
	for (unsigned int i = _vHighIndexRecord_Local.size()-1; i != 0; i--)
	{
		if (frequency >= frequUplimit)
			return _eTrendTopFall;
		if (_vHighIndexRecord_Local[i - 1]._Value > _vHighIndexRecord_Local[i]._Value)
		{
			frequency++;
			continue;
		}
		if (_vHighIndexRecord_Local[i - 1]._Value < _vHighIndexRecord_Local[i]._Value)
			break;
	}
	//无明显趋势
	return _eTrendNoRule;
}

CharaPointType CTrandRecordTool::GetCompositeTrandResult()const
{
	CharaPointType bottomresult = GetBottonPointTrandResult();
	CharaPointType topresult = GetTopPointTrandResult();

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

StatePointsList CTrandRecordTool::ButtonTrendState()
{
	return bottontrend;
}

StatePointsList CTrandRecordTool::TopTrendState()
{
	return toptrend;
}
