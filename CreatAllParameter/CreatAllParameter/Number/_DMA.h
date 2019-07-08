
#pragma once
#include "NumberBase.h"
#include "../MovingAverage.h"
#include <list>
#ifndef _DMA_H
#define _DMA_H





class CDMACal// : public CNumberManager
{
public:
	CDMACal();
	~CDMACal();
	//////////////////////////////////////////////////////////////////////////
	//根据当天的数据获得当天的DMA值
	//
	//////////////////////////////////////////////////////////////////////////
	void GetNextDMA(const SinCyclePriceData& OneDayData, DMA_DATA& _Dma);
	void Inition();
private:
	CMovingAverage CloseMA1;
	CMovingAverage CloseMA2;
	CMovingAverage DDDMA;
// 	list<StockDataType> closelyData50;
// 	list<StockDataType> frontDMAData;
// 	unsigned int Paremeter50;
// 	unsigned int Paremeter10;
};



#endif