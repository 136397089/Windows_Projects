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
	//��frontValue����0��ʱ����Ϊ��ǰ������ǵ�һ������
	if (frontValue <= 0.001)
	{
		changeRate = 0.0f;
		frontValue = OneDayData;
		return true;
	}
	//frontValue������0��ʱ�����۸�仯
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
