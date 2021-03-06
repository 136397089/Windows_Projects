#pragma once
#include <queue>
#include <list>
#include "Number/NumberBase.h"
#include "MovingAverage.h"


class CArBrCrVrPsy
{
public:
	CArBrCrVrPsy();
	~CArBrCrVrPsy();
	bool GetNextArBrVrPsy(const SinCyclePriceData& OneDayData, ARBRCRVRPSY& mLastData);

	void Inition();

	StockDataType GetAR();
	StockDataType GetBR();
	StockDataType GetCR();
	StockDataType GetVR();
	StockDataType GetPSY();
	StockDataType GetLimitDownTimes();
	StockDataType GetLimitUpTimes();

	unsigned int CR_MA1Para;
	unsigned int CR_MA2Para;
	unsigned int CR_MA3Para;
	unsigned int CR_MA4Para;

	CMovingAverage VRMA;
	CMovingAverage AR_HighTo_Open;
	CMovingAverage AR_OpenTo_Low;
	CMovingAverage BR_HighTo_Close;
	CMovingAverage BR_CloseTo_Low;
	CMovingAverage VR_AVS;
	CMovingAverage VR_BVS;
	CMovingAverage CR_HighToMid;
	CMovingAverage CR_MidToLow;
	CMovingAverage PSYMA;
	CMovingAverage LimitDownTimesMA;
	CMovingAverage LimitUpTimesMA;
	CMovingAverage CRMA1;
	CMovingAverage CRMA2;
	CMovingAverage CRMA3;
	CRefData RefCRMA1;
	CRefData RefCRMA2;
	CRefData RefCRMA3;


	SinCyclePriceData CurrentData;
	SinCyclePriceData RefData;

};





/*
AR计算公式
AR=((H - O)26天之和/(O - L)26天之和) * 100
H：当天之最高价
L：当天之最低价
O：当天之开盘价

BR计算公式
BR=((H - PC)26天之和/(PC - L)26天之和) * 100
H：当天之最高价
L：当天之最低价
PC：昨天之收盘价

CR计算公式
CR（N日）=P1÷P2×100
式中，P1=Σ（H－YM），表示N日以来多方力量的总和；P2=Σ（YM－L），表示N日以来空方力量的总和。
H表示今日的最高价，L表示今日的最低价YM表示昨日（上一个交易日）的中间价。
CR计算公式中的中间价其实也是一个指标，它是通过对昨日（YM）交易的最高价、最低价、开盘价和收盘价进行加权平均而得到的，其每个价格的权重可以人为地选定。
比较常用地中间价计算方法有四种：
1、M=（2C+H+L）÷4
2、M=（C+H+L+O）÷4
3、M=（C+H+L）÷3
4、M=（H+L）÷2

VR计算公式
1．24天以来凡是股价上涨那一天的成交量都称为AV，将24天内的AV总和相加后称为AVS。
2．24天以来凡是股价下跌那一天的成交量都称为BV，将24天内的BV总和相加后称为BVS。
3．24天以来凡是股价不涨不跌，则那一天的成交量都称为CV，将24天内的CV总和相加后称为CVS。
4． 24天开始计算：
VR=（AVS+1/2CVS）/（BVS+1/2CVS）
5． 计算例参数24天可以修改，但是周期不宜小于12，否则，采样天数不足容易造成偏差。

PSY计算公式
1.PSY=N日内上涨天数/N*100
2.PSYMA=PSY的M日简单移动平均
3.参数N设置为12日，参数M设置为6日
*/