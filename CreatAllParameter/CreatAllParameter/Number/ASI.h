#pragma once
#include "IndexNumber.h"
struct ASI/*:public IndexBaseType*/
{
	ASI():_asi(0),_asit(0){}
	tySData _asi;//
	tySData _asit;//¾ùÖµ
// 	ASI& operator = (const ASI& rhs){
// 		_Index1 = rhs._Index1;
// 		_Index2 = rhs._Index2;
// 		_Index3 = rhs._Index3;
// 		_Index4 = rhs._Index4;
// 		_Index5 = rhs._Index5;
// 		return *this;
// 	}
};
class CAsi :
	public CNumberManager
{
public:
	CAsi();
	~CAsi();
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	bool GetNextASI(const DatePriceData& OneDayData, ASI& mFrontMa);
private:
	DatePriceData _FrontDayData;
	list<tySData> _vSIList;
	list<tySData> _vASIList;
	unsigned int _asiDayPara;
	unsigned int _asitDayPara;
};

