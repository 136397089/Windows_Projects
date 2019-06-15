#include "stdafx.h"
#include "ComTime.h"


CComTime::CComTime()
{
}


CComTime::~CComTime()
{
}

void CComTime::TimeBegin()
{
	GetLocalTime(&beginTime);
}

void CComTime::TimeEnd(const string& indexname)
{
	GetLocalTime(&endTime);
	LOG(INFO) << indexname << " " << (endTime.wSecond - beginTime.wSecond) * 1000 + (endTime.wMilliseconds - beginTime.wMilliseconds) << "mS";
}
