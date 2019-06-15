#include "stdafx.h"
#include "TopButRecord.h"


CTopButRecord::CTopButRecord()
{
}


CTopButRecord::~CTopButRecord()
{
}

void CTopButRecord::Inition()
{
	HighIndexs.clear();
	LowIndexs.clear();
	CurrentHigh.HeadDistance = 0;
	CurrentLow.HeadDistance = 0;
}
