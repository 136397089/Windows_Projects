#include "stdafx.h"
#include "BullMarkBearMark.h"
#include "glog/logging.h"

CBullMarkBearMark::CBullMarkBearMark()
{
}


CBullMarkBearMark::~CBullMarkBearMark()
{
}

#define BULLMARKET 10
MarketTypeList CBullMarkBearMark::GetMarketTypes(const VStockData& pricedata, const vector<string>& day)
{
	MarketTypeList templist;
	templist.clear();
	if (pricedata.size() != day.size())
		return templist;
	if (pricedata.size() == 0)
		return templist;

	tySData fristData = pricedata[0];
	tySData FrontData = pricedata[0];
	string FrontDay = day[0];
	MarketTypeIndex tempmarkindex;
	bool fristIndexFound = false;
	bool findBull = true;
	for (unsigned int i = 0; i < pricedata.size();i++)
	{
		//�жϵ�һ���г�����
		if ((pricedata[i] - FrontData) * 100 / FrontData >= BULLMARKET && (!fristIndexFound))
		{
			fristIndexFound = true;
			findBull = true;
			FrontData = pricedata[i];
			FrontDay = day[i];
			tempmarkindex._markettype = BullMarket;
			tempmarkindex._day.SetDay(day[i]);
			tempmarkindex._data = pricedata[i];
			tempmarkindex._relativeDate.SetDay(day[0]);
			tempmarkindex._relativeData = FrontData;
			templist.push_back(tempmarkindex);
			continue;
		}
		else if ((pricedata[i] - FrontData) * 100 / FrontData <= -BULLMARKET && (!fristIndexFound))
		{
			fristIndexFound = true;
			findBull = false;
			FrontData = pricedata[i];
			FrontDay = day[i];
			tempmarkindex._markettype = BearMarket;
			tempmarkindex._day.SetDay(day[i]);
			tempmarkindex._data = pricedata[i];
			tempmarkindex._relativeDate.SetDay(day[0]);
			tempmarkindex._relativeData = FrontData;
			templist.push_back(tempmarkindex);
			continue;
		}
		//�жϺ�����г�����
		if (fristIndexFound)
		{
			if (!findBull && (pricedata[i] - FrontData) * 100 >= BULLMARKET * FrontData){// ��ţ�е�
				tempmarkindex._markettype = BullMarket;
				tempmarkindex._day.SetDay(day[i]);
				tempmarkindex._data = pricedata[i];
				tempmarkindex._relativeDate.SetDay(FrontDay);
				tempmarkindex._relativeData = FrontData;
				templist.push_back(tempmarkindex);

				findBull = true;
				FrontData = pricedata[i];
			}
			if (!findBull && pricedata[i] < FrontData){
				FrontData = pricedata[i];
				FrontDay = day[i];
			}
			if (findBull && (pricedata[i] - FrontData) * 100 <= -BULLMARKET *FrontData){//�����г���
				tempmarkindex._markettype = BearMarket;
				tempmarkindex._day.SetDay(day[i]);
				tempmarkindex._data = pricedata[i];
				tempmarkindex._relativeDate.SetDay(FrontDay);
				tempmarkindex._relativeData = FrontData;
				templist.push_back(tempmarkindex);

				findBull = false;
				FrontData = pricedata[i];
			}
			if (findBull && pricedata[i] > FrontData){
				FrontData = pricedata[i];
				FrontDay = day[i];
			}
		}

	}
#ifdef _MYDEBUG
	LOG(INFO) << "MarketData";
	for (unsigned int i = 1; i < templist.size();i++)
	{
		LOG(INFO) << "FrontData:" << int(templist[i]._data) << "   \t"
			<< "FrontDay:" << templist[i]._day.GetDay() << "   \t"
			<< "RelativeData:" << int(templist[i]._relativeData) << "   \t"
			<< "RelativeDay:" << templist[i]._relativeDate.GetDay() << "   \t"
			<< "MaxChangeRage:" << (templist[i]._relativeData - templist[i - 1]._relativeData) * 100 / templist[i - 1]._relativeData
			<< endl;
	}
#endif
	return templist;
}
