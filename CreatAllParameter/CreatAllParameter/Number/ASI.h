#pragma once
#include "NumberBase.h"
struct ASI/*:public IndexBaseType*/
{
	ASI() :_asi(0), _asit(0){}
	tyStockData _asi;//
	tyStockData _asit;//¾ùÖµ
// 	ASI& operator = (const ASI& rhs){
// 		_Index1 = rhs._Index1;
// 		_Index2 = rhs._Index2;
// 		_Index3 = rhs._Index3;
// 		_Index4 = rhs._Index4;
// 		_Index5 = rhs._Index5;
// 		return *this;
// 	}
};
class CAsi// :public CNumberManager
{
public:
	CAsi();
	~CAsi();
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	bool GetNextASI(const DatePriceData& OneDayData, ASI& mFrontMa);

	bool Inition();
private:
	DatePriceData _YesterdayData;
	list<tyStockData> _vSIList;
	list<tyStockData> _vASIList;

	unsigned int _M1;
	unsigned int _M2;
};

