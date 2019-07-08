#include "stdafx.h"
#include <numeric>
#include "_DMA.h"

CDMACal::CDMACal():
CloseMA1(10),
CloseMA2(50),
DDDMA(10)
{
}

CDMACal::~CDMACal()
{
}


void CDMACal::GetNextDMA(const SinCyclePriceData& OneDayData, DMA_DATA& Dma)
{
	Dma._DDD = CloseMA1.GetNextMA(OneDayData._Close) - CloseMA2.GetNextMA(OneDayData._Close);
	Dma._AMA = DDDMA.GetNextMA(Dma._DDD);
	return;
}

void CDMACal::Inition()
{
	CloseMA1.Inition();
	CloseMA2.Inition();
	DDDMA.Inition();
}
