#include "stdafx.h"
#include "PriChag.h"


CChagRate::CChagRate():
frontValue(0)
{
}


CChagRate::~CChagRate()
{
}

bool CChagRate::GetNextChangeRate(const tyStockData& OneDayData, float& changeRate)
{
	//当frontValue等于0的时候认为当前输入的是第一个数据
	if (frontValue <= 0.001)
	{
		changeRate = 0.0f;
		frontValue = OneDayData;
		return true;
	}
	//frontValue不等于0的时候计算价格变化
	else
	{
		changeRate = (OneDayData - frontValue) / frontValue;
		frontValue = OneDayData;
		return true;
	}
}

bool CChagRate::Inition()
{
	frontValue = 0;
	return true;
}
