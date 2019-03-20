#pragma once
#ifndef STATICINTEGRAL_H
#define  STATICINTEGRAL_H
#include "StateB.h"

class CStaticIntegral
{
public:
	CStaticIntegral();
	~CStaticIntegral();
	void Inition();

	bool GetsStaticIng(
		const VStockData& _data ,
		const StatePointsList& _staticPoint,
		StatePointsList& _staticIng);
	

private:
	string _LastError;
};

#endif