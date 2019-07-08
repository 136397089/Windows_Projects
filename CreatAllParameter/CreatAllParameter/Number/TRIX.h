#pragma once
#include <list>
#include "NumberBase.h"
#include "../MovingAverage.h"

class CTRIXCal// :public CNumberManager
{
public:
	CTRIXCal();
	~CTRIXCal();
	void GetNextTRIX(const SinCyclePriceData& OneDayData, TRIX& mTrix);
	void Inition();
private:
	CMovingAverage TRMA;
	float _EMAParameter;
};

