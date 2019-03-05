 #include "stdafx.h"
#include "StatusB.h"
#include "glog/logging.h"

// CStatisticsB::CStatisticsB()
// {
// }
// 
// 
// CStatisticsB::~CStatisticsB()
// {
// }


VStockData AllStatus::GetLocalStatusPointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _localstatus.size(); i++)
	{
		tempdate.push_back(_localstatus[i]._Value);
	}
	return tempdate;

}
VStockData AllStatus::GetStaticIntegPointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempdate.push_back(_staticIng[i]._Value);
	}
	return tempdate;
}
VStockData AllStatus::GetStaticStatusPointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticstatus.size(); i++)
	{
		tempdate.push_back(_staticstatus[i]._Value);
	}
	return tempdate;

}
VStockData AllStatus::GetTrendStatusPointsValue() const
{
	VStockData tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _trendstatus.size(); i++)
	{
		tempdate.push_back(_trendstatus[i]._Value);
	}
	return tempdate;
}

vector<string> AllStatus::GetLocalDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _localstatus.size(); i++)
	{
		tempdate.push_back(_localstatus[i]._Date.GetDay());
	}
	return tempdate;
}

vector<string> AllStatus::GetStaticDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticstatus.size(); i++)
	{
		tempdate.push_back(_staticstatus[i]._Date.GetDay());
	}
	return tempdate;

}

vector<string> AllStatus::GetStaticIntegDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempdate.push_back(_staticIng[i]._Date.GetDay());
	}
	return tempdate;
}


vector<string> AllStatus::GetTrendDay() const
{
	vector<string> tempdate;
	tempdate.clear();
	for (unsigned int i = 0; i < _trendstatus.size(); i++)
	{
		tempdate.push_back(_trendstatus[i]._Date.GetDay());
	}
	return tempdate;

}


IndexList AllStatus::GetStaticStausIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _staticstatus.size(); i++)
	{
		tempInedx.push_back(_staticstatus[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList AllStatus::GetStaticIngIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _staticIng.size(); i++)
	{
		tempInedx.push_back(_staticIng[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList AllStatus::GetLocalStausIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _localstatus.size(); i++)
	{
		tempInedx.push_back(_localstatus[i]._TimeIndex);
	}
	return tempInedx;
}

IndexList AllStatus::GetTrendStausIndex() const
{
	IndexList tempInedx;
	tempInedx.clear();
	for (unsigned int i = 0; i < _trendstatus.size(); i++)
	{
		tempInedx.push_back(_trendstatus[i]._TimeIndex);
	}
	return tempInedx;
}

StatusPoint AllStatus::CloselyStaticPoint(unsigned int index) const
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _staticstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_staticstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _staticstatus[frontIndex]._TimeIndex)//Index在第一个数据之前
		return frontPoint;
	if (_staticstatus[laterIndex]._TimeIndex <= index)//Index在最后一个数据之后
		return _staticstatus[laterIndex];
	if (_staticstatus.size() == 1)//只有一个数据可以返回
		return _staticstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _staticstatus[i - 1];
		laterPoint = _staticstatus[i];
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

StatusPoint AllStatus::CloselyStaticPoint(CDate index)
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _staticstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_staticstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _staticstatus[frontIndex]._Date)//Index在第一个数据之前
		return frontPoint;
	if (_staticstatus[laterIndex]._Date <= index)//Index在最后一个数据之后
		return _staticstatus[laterIndex];
	if (_staticstatus.size() == 1)//只有一个数据可以返回
		return _staticstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _staticstatus[i - 1];
		laterPoint = _staticstatus[i];
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

StatusPoint AllStatus::CloselyLocalPoint(unsigned int index) const
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _localstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_localstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _localstatus[frontIndex]._TimeIndex)//Index在第一个数据之前
		return frontPoint;
	if (_localstatus[laterIndex]._TimeIndex <= index)//Index在最后一个数据之后
		return _localstatus[laterIndex];
	if (_localstatus.size() == 1)//只有一个数据可以返回
		return _localstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _localstatus[i - 1];
		laterPoint = _localstatus[i];
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

StatusPoint AllStatus::CloselyLocalPoint(CDate index) const
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _localstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_localstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _localstatus[frontIndex]._Date)//Index在第一个数据之前
		return frontPoint;
	if (_localstatus[laterIndex]._Date <= index)//Index在最后一个数据之后
		return _localstatus[laterIndex];
	if (_localstatus.size() == 1)//只有一个数据可以返回
		return _localstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _localstatus[i - 1];
		laterPoint = _localstatus[i];
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

StatusPoint AllStatus::CloselyTrendPoint(unsigned int index) const
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _trendstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_trendstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _trendstatus[frontIndex]._TimeIndex)//Index在第一个数据之前
		return frontPoint;
	if (_trendstatus[laterIndex]._TimeIndex <= index)//Index在最后一个数据之后
		return _trendstatus[laterIndex];
	if (_trendstatus.size() == 1)//只有一个数据可以返回
		return _trendstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _trendstatus[i - 1];
		laterPoint = _trendstatus[i];
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

StatusPoint AllStatus::CloselyTrendPoint(CDate index) const
{
	StatusPoint frontPoint;
	StatusPoint laterPoint;
	unsigned int frontIndex = 0;
	unsigned int laterIndex = _trendstatus.size() - 1;
	unsigned int tempIndex = 0;

	if (_trendstatus.size() == 0)//没有数据可以返回
		return frontPoint;
	if (index <= _trendstatus[frontIndex]._Date)//Index在第一个数据之前
		return frontPoint;
	if (_trendstatus[laterIndex]._Date <= index)//Index在最后一个数据之后
		return _trendstatus[laterIndex];
	if (_trendstatus.size() == 1)//只有一个数据可以返回
		return _trendstatus[0];

	for (unsigned int i = 1; i < laterIndex; i++)
	{
		frontPoint = _trendstatus[i - 1];
		laterPoint = _trendstatus[i];
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

