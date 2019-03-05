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
	//�������Ĳ��������Ƿ�̫��
	if (vdatalist.size() < 3)
	{
		return false;
	}
	//��һ�����������0���޷����к��������
	tySData begindata = vdatalist[0];
	if (begindata < 0.001 && begindata > -0.001)
		return false;
	chanRate.clear();
	//����ÿ������ڵ�һ��ı仯��
	for (VStockData::const_iterator ite = vdatalist.cbegin(); ite != vdatalist.cend();ite++)
	{
		chanRate.push_back((*ite - begindata)*100 / begindata);
	}
	return true;
}
