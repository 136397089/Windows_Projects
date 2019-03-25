#include "stdafx.h"
#include "Numbers.h"

CNumbersCalculator::CNumbersCalculator()
{
}


CNumbersCalculator::~CNumbersCalculator()
{
}

bool CNumbersCalculator::GetAllNumbers(StockDataTable& datas)
{
	//计算类
	CChagRate hPriChaRate;
	CChagRate hVolChaRate;
	CMacdManager hMacd(12, 26, 9);
	CDMA hDMA;
	CTRIX hTRIX;
	CKDJ hKDJ(9, 3, 3);
	CAsi hAsi;
	CCDP hCdp;
	CDMI hDmi;
	CMa hma(5, 10, 20, 60);
	CArBrCrVrPsy ArBrVrPsy;
	//结果输出
	SigDayTechIndex currentIndex;
	//一天的价格
	DatePriceData thisDayPrice;
	//要遍历的所有数据
	vector<string>::const_iterator dayIte = datas._vTimeDay.begin();
	VStockData::const_iterator closeite = datas._vClose.begin();
	VStockData::const_iterator highite = datas._vHigh.begin();
	VStockData::const_iterator lowite = datas._vLow.begin();
	VStockData::const_iterator openite = datas._vOpen.begin();
	VStockData::const_iterator volite = datas._vVolume.begin();
	//1.1计算指标
	while (closeite != datas._vClose.end() && lowite != datas._vLow.end() && highite != datas._vHigh.end())
	{
		//1.1
		thisDayPrice._Close = *closeite;
		thisDayPrice._High = *highite;
		thisDayPrice._Low = *lowite;
		thisDayPrice._Open = *openite;
		thisDayPrice._Volume = *volite;
		thisDayPrice.mDate.SetDay(*dayIte);
		//1.2迭代器后移
		++dayIte;
		++highite;
		++lowite;
		++closeite;
		++openite;
		++volite;
		//1.3计算指标
		hMacd.GetNextMacd(thisDayPrice, currentIndex._MacdData);
		hDMA.GetNextDMA(thisDayPrice, currentIndex._DMAData);
		hTRIX.GetNextTRIX(thisDayPrice, currentIndex._TrixData);
		hKDJ.GetNextKDJ(thisDayPrice, currentIndex._Kdj);
		hAsi.GetNextASI(thisDayPrice, currentIndex._Asi);
		hCdp.GetNextCDP(thisDayPrice, currentIndex._Cdp);
		hDmi.GetNextDMI(thisDayPrice, currentIndex._Dmi);
		hma.GetNextMa(thisDayPrice, currentIndex._Ma);
		ArBrVrPsy.GetNextArBrVrPsy(thisDayPrice, currentIndex._ArBrVrPsy);
		hPriChaRate.GetNextChangeRate(thisDayPrice._Close, currentIndex._Pchangerate);
		hVolChaRate.GetNextChangeRate(thisDayPrice._Volume, currentIndex._Volchagrate);
		//1.4保存计算完的各类指标数据到对应的Vector当中
		PushBackIndex(currentIndex, datas);
	}
	datas.SetDate();
	return true;
}


void CNumbersCalculator::PushBackIndex(const SigDayTechIndex& AllIndex, StockDataTable& datas)
{
	//MACD
	datas._vMACDValue.push_back(AllIndex._MacdData.bar);
	datas._vMACDMa12.push_back(AllIndex._MacdData.m12);
	datas._vMACDMa26.push_back(AllIndex._MacdData.m26);
	datas._vDiff.push_back(AllIndex._MacdData.diff);
	datas._vDEA.push_back(AllIndex._MacdData.dea);
	//DMA
	datas._vDMAValue.push_back(AllIndex._DMAData._DDD);
	datas._vAMAValue.push_back(AllIndex._DMAData._AMA);
	//TRIX
	datas._vTRIX.push_back(AllIndex._TrixData._TRIX);
	datas._vTRMA.push_back(AllIndex._TrixData._TRMA);
	//KDJ
	datas._vK.push_back(AllIndex._Kdj.K_OF_KDJ);
	datas._vD.push_back(AllIndex._Kdj.D_OF_KDJ);
	datas._vJ.push_back(AllIndex._Kdj.J_OF_KDJ);
	//ASI
	datas._vAsi_i.push_back(AllIndex._Asi._asi);
	datas._vAsit.push_back(AllIndex._Asi._asit);
	//MA
	datas._vMa1.push_back(AllIndex._Ma.Ma1);
	datas._vMa2.push_back(AllIndex._Ma.Ma2);
	datas._vMa3.push_back(AllIndex._Ma.Ma3);
	datas._vMa4.push_back(AllIndex._Ma.Ma4);
	//Change rate
	datas._vPriChaRate.push_back(AllIndex._Pchangerate);
	datas._vVolChaRate.push_back(AllIndex._Volchagrate);
	//
	datas._vCDP.push_back(AllIndex._Cdp._CDP);
	datas._vAH_High.push_back(AllIndex._Cdp._AH_High);
	datas._vNH_NormalHigh.push_back(AllIndex._Cdp._NH_NormalHigh);
	datas._vNL_NormalLow.push_back(AllIndex._Cdp._NL_NormalLow);
	datas._vAL_Low.push_back(AllIndex._Cdp._AL_Low);
	//
	datas._vAR.push_back(AllIndex._ArBrVrPsy.AR);
	datas._vBR.push_back(AllIndex._ArBrVrPsy.BR);
	datas._vCR.push_back(AllIndex._ArBrVrPsy.CR);
	datas._vCRMA1.push_back(AllIndex._ArBrVrPsy.CRMa1);
	datas._vCRMA2.push_back(AllIndex._ArBrVrPsy.CRMa2);
	datas._vCRMA3.push_back(AllIndex._ArBrVrPsy.CRMa3);
	datas._vCRMA4.push_back(AllIndex._ArBrVrPsy.CRMa4);
	datas._vVR.push_back(AllIndex._ArBrVrPsy.VR);
	datas._vPSY.push_back(AllIndex._ArBrVrPsy.PSY);
}