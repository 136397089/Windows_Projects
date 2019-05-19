 #include "stdafx.h"
#include "StateB.h"
#include "glog/logging.h"

// CStatisticsB::CStatisticsB()
// {
// }
// 
// 
// CStatisticsB::~CStatisticsB()
// {
// }


VStockData StateTable::GetLocalStatePointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _localstate.size(); i++)
	{
		tempdate.push_back(_localstate[i]._Value);
	}
	return tempdate;

}
VStockData StateTable::GetStaticIntegPointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempdate.push_back(_staticIng[i]._Value);
	}
	return tempdate;
}
VStockData StateTable::GetStaticStatePointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticstate.size(); i++)
	{
		tempdate.push_back(_staticstate[i]._Value);
	}
	return tempdate;

}
VStockData StateTable::GetTrendStatePointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _trendstate.size(); i++)
	{
		tempdate.push_back(_trendstate[i]._Value);
	}
	return tempdate;
}

vector<string> StateTable::GetLocalDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _localstate.size(); i++)
	{
		tempdate.push_back(_localstate[i]._Date.GetDay());
	}
	return tempdate;
}

vector<string> StateTable::GetStaticDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticstate.size(); i++)
	{
		tempdate.push_back(_staticstate[i]._Date.GetDay());
	}
	return tempdate;

}

vector<string> StateTable::GetStaticIntegDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempdate.push_back(_staticIng[i]._Date.GetDay());
	}
	return tempdate;
}


vector<string> StateTable::GetTrendDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _trendstate.size(); i++)
	{
		tempdate.push_back(_trendstate[i]._Date.GetDay());
	}
	return tempdate;

}


IndexList StateTable::GetStaticStateIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _staticstate.size(); i++)
	{
		tempInedx.push_back(_staticstate[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList StateTable::GetStaticIngIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempInedx.push_back(_staticIng[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList StateTable::GetLocalStateIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _localstate.size(); i++)
	{
		tempInedx.push_back(_localstate[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList StateTable::GetTrendStateIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _trendstate.size(); i++)
	{
		tempInedx.push_back(_trendstate[i]._TimeIndex);
	}
	return tempInedx;
}

StatePoint StateTable::CloselyStaticPoint(unsigned int index) const
{
	return CloselyPoint(index,_staticstate);
}

StatePoint StateTable::CloselyStaticPoint(CDate index) const
{
	return CloselyPoint(index, _staticstate);
}

StatePoint StateTable::CloselyLocalPoint(unsigned int index) const
{
	return CloselyPoint(index, _localstate);
}

StatePoint StateTable::CloselyLocalPoint(CDate index) const
{
	return CloselyPoint(index, _localstate);
}

StatePoint StateTable::CloselyTrendPoint(unsigned int index) const
{
	return CloselyPoint(index, _trendstate);
}

StatePoint StateTable::CloselyTrendPoint(CDate index) const
{
	return CloselyPoint(index, _trendstate);
}

StatePoint StateTable::CloselyIntegralPoint(unsigned int index) const
{
	return CloselyPoint(index, _staticIng);
}

StatePoint StateTable::CloselyIntegralPoint(CDate index) const
{
	return CloselyPoint(index, _staticIng);
}

StatePoint StateTable::CloselyPoint(CDate index, const StatePointsList& PointList) const
{
	StatePoint frontPoint;
	StatePoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = PointList.size() - 1;
	unsigned int tempIndex = 0;

	if (PointList.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= PointList[frontIndex]._Date)//Index在第一个数据之前
		return frontPoint;
	if (PointList[laterIndex]._Date <= index)//Index在最后一个数据之后
		return PointList[laterIndex];
	if (PointList.size() == 1)//只有一个数据可以返回
		return PointList[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = PointList[i - 1];
		laterPoint = PointList[i];
		if (index >= frontPoint._Date && index < laterPoint._Date)
			break;
		if (frontPoint._Date > laterPoint._Date)
		{
			LOG(ERROR) << "Function CloselyStaticPoint(), Time Index Error.";
			exit(0);
		}
	}
	return frontPoint;
}

StatePoint StateTable::CloselyPoint(unsigned int index, const StatePointsList& PointList) const
{
	StatePoint frontPoint;
	StatePoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = PointList.size() - 1;
	unsigned int tempIndex = 0;

	if (PointList.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= PointList[frontIndex]._TimeIndex)//Index在第一个数据之前
		return frontPoint;
	if (PointList[laterIndex]._TimeIndex <= index)//Index在最后一个数据之后
		return PointList[laterIndex];
	if (PointList.size() == 1)//只有一个数据可以返回
		return PointList[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = PointList[i - 1];
		laterPoint = PointList[i];
		if (index >= frontPoint._TimeIndex && index < laterPoint._TimeIndex)
			break;
		if (frontPoint._TimeIndex > laterPoint._TimeIndex)
		{
			LOG(ERROR) << "Function CloselyStaticPoint(), Time Index Error.";
			exit(0);
		}
	}
	return frontPoint;
}

