#pragma once
#include <list>
#include "NumberBase.h"
struct TRIX /*: public IndexBaseType*/
{
	TRIX() :_TRIX(0), _TRMA(0),_AX(0), _BX(0), _TR(0){}
	tyStockData _TRIX;
	tyStockData _TRMA;
	tyStockData _AX;
	tyStockData _BX;
	tyStockData _TR;

// 	TRIX& operator = (const TRIX& rhs){
// 		_Index1 = rhs._Index1;
// 		_Index2 = rhs._Index2;
// 		_Index3 = rhs._Index3;
// 		_Index4 = rhs._Index4;
// 		_Index5 = rhs._Index5;
// 		return *this;
// 	}
};
class CTRIX// :public CNumberManager
{
public:
	CTRIX();
	~CTRIX();
	void GetNextTRIX(const DatePriceData& OneDayData, TRIX& mTrix);
private:
	float GetEMA(const list<tyStockData>& dataList);
	float _ParameterN;
	float _ParameterM;
	float _EMADenominator;
	list<tyStockData> frontClose;
	list<tyStockData> frontTA;
	list<tyStockData> frontTB;
	list<tyStockData> frontTR;
	// vector<TRIX> _vTrix;
	list<tyStockData> TrixData;
	void StaticSizePush(tyStockData mdata, float msize, list<tyStockData>& mList);
};

