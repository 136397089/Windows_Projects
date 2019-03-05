#pragma once
#ifndef STATICINTEGRAL_H
#define  STATICINTEGRAL_H
#include "StatusB.h"

class CStaticIntegral
{
public:
	CStaticIntegral();
	~CStaticIntegral();
	void Inition();

	bool GetsStaticIng(
		const VStockData& _data ,
		const StatusPointsList& _staticPoint,
		StatusPointsList& _staticIng);
	

private:
	string _LastError;
};

#endif