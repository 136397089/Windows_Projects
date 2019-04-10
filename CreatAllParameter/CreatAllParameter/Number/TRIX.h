#pragma once
#include <list>
#include "NumberBase.h"
struct TRIX /*: public IndexBaseType*/
{
	TRIX() :_TRIX(0), _TRMA(0),_AX(0), _BX(0), _TR(0){}
	StockDataType _TRIX;
	StockDataType _TRMA;
	StockDataType _AX;
	StockDataType _BX;
	StockDataType _TR;

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
	float GetEMA(const list<StockDataType>& dataList);
	float _ParameterN;
	float _ParameterM;
	float _EMADenominator;
	list<StockDataType> frontClose;
	list<StockDataType> frontTA;
	list<StockDataType> frontTB;
	list<StockDataType> frontTR;
	// vector<TRIX> _vTrix;
	list<StockDataType> TrixData;
	void StaticSizePush(StockDataType mdata, float msize, list<StockDataType>& mList);
};

