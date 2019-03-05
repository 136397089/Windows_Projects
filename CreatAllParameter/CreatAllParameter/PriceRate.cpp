#include "stdafx.h"
#include "PriceRate.h"

CPriceRate::CPriceRate()
{
}


CPriceRate::~CPriceRate()
{
}

bool CPriceRate::GetEveryDayChangeRate(const VStockData& vdatalist, VStockData& chanRate)
{
	//检查输入的参数个数是否太少
	if (vdatalist.size() < 3)
	{
		return false;
	}
	//第一个参数如果是0则无法进行后面的运算
	tySData begindata = vdatalist[0];
	if (begindata < 0.001 && begindata > -0.001)
		return false;
	chanRate.clear();
	//计算每天相对于第一天的变化率
	for (VStockData::const_iterator ite = vdatalist.cbegin(); ite != vdatalist.cend();ite++)
	{
		chanRate.push_back((*ite - begindata)*100 / begindata);
	}
	return true;
}
