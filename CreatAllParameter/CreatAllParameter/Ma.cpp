#include "stdafx.h"
#include "Ma.h"
#include<numeric>



CMa::CMa(int M1, int M2, int M3, int M4)
	:M1_Par(M1), M2_Par(M2), M3_Par(M3), M4_Par(M4)
{
	day = 0;
	closedatas.clear();
}

CMa::CMa()
{
}


CMa::~CMa()
{
}

bool CMa::GetNextMa(const DatePriceData& OneDayData, MA& mFrontMa)
{
	MA TempMa = mFrontMa;
	closedatas.push_back(OneDayData._Close);
	if (closedatas.size() > M1_Par 
		&&closedatas.size() > M2_Par
		&&closedatas.size() > M3_Par
		&&closedatas.size() > M4_Par)//由于所有可以比较的数据都是大于0，忽略类型不匹配的警告
	{
		closedatas.pop_front();
	}
	day ++;
	list<tyStockData>::iterator  Ma1Begin = closedatas.end();
	list<tyStockData>::iterator  Ma2Begin = closedatas.end();
	list<tyStockData>::iterator  Ma3Begin = closedatas.end();
	list<tyStockData>::iterator  Ma4Begin = closedatas.end();
	if (day < M1_Par)
		Ma1Begin = closedatas.begin();
	else
		advance(Ma1Begin, - M1_Par);

	if (day < M2_Par)
		Ma2Begin = closedatas.begin();
	else
		advance(Ma2Begin, - M2_Par);

	if (day < M3_Par)
		Ma3Begin = closedatas.begin();
	else
		advance(Ma3Begin, - M3_Par);

	if (day < M4_Par)
		Ma4Begin = closedatas.begin();
	else
		advance(Ma4Begin, - M4_Par);

	mFrontMa.Ma1 = accumulate(Ma1Begin, closedatas.end(), 0) / (tyStockData)M1_Par;
	mFrontMa.Ma2 = accumulate(Ma2Begin, closedatas.end(), 0) / (tyStockData)M2_Par;
	mFrontMa.Ma3 = accumulate(Ma3Begin, closedatas.end(), 0) / (tyStockData)M3_Par;
	mFrontMa.Ma4 = accumulate(Ma4Begin, closedatas.end(), 0) / (tyStockData)M4_Par;
	return true;
}

bool CMa::Inition()
{
	day = 0;
	closedatas.clear();
	return true;
}



