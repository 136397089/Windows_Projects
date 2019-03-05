#pragma once
#ifndef BULLMARKBEARMARK_H
#define BULLMARKBEARMARK_H
#include "DateTool.h"
#include "Number/commonfunction.h"
enum MarketType
{
	BullMarket,
	BearMarket
};
struct MarketTypeIndex
{
	MarketType _markettype;
	CDate _day;
	tySData _data;
	CDate _relativeDate;
	tySData _relativeData;
};
typedef vector<MarketTypeIndex> MarketTypeList;

class CBullMarkBearMark
{
public:
	CBullMarkBearMark();
	~CBullMarkBearMark();
	void Inition();

	MarketTypeList GetMarketTypes(const VStockData& pricedata, const vector<string>& date);
};

#endif
