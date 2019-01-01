#pragma once
#include <list>
#include "IndexNumber.h"
struct TRIX /*: public IndexBaseType*/
{
	TRIX() :_TRIX(0), _TRMA(0),_AX(0), _BX(0), _TR(0){}
	tySData _TRIX;
	tySData _TRMA;
	tySData _AX;
	tySData _BX;
	tySData _TR;

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
	float GetEMA(const list<tySData>& dataList);
	float _ParameterN;
	float _ParameterM;
	float _EMADenominator;
	list<tySData> frontClose;
	list<tySData> frontTA;
	list<tySData> frontTB;
	list<tySData> frontTR;
	// vector<TRIX> _vTrix;
	list<tySData> TrixData;
	void StaticSizePush(tySData mdata, float msize, list<tySData>& mList);
};

