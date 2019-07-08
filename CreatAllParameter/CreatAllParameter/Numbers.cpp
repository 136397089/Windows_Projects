#include "stdafx.h"
#include "Numbers.h"
#include "glog/logging.h"
#include "ComTime.h"
CIndicatorsCalculator::CIndicatorsCalculator():
hMacd(12, 26, 9),
hKDJ(9, 3, 3),
hma(5, 10, 20, 60)
{
}


CIndicatorsCalculator::~CIndicatorsCalculator()
{
}

bool CIndicatorsCalculator::GetAllNumbers(StockDataTable& stockdatas)
{
	stockCode = stockdatas._strStockCode;
	//一天的价格
	SinCyclePriceData thisDayPrice;
	//输入运算的日期，价格，成交量
	vector<string>::const_iterator dayIte = stockdatas._vTimeDay.begin();
	VStockData::const_iterator closeite = stockdatas._vClose.begin();
	VStockData::const_iterator highite = stockdatas._vHigh.begin();
	VStockData::const_iterator lowite = stockdatas._vLow.begin();
	VStockData::const_iterator openite = stockdatas._vOpen.begin();
	VStockData::const_iterator volite = stockdatas._vVolume.begin();
	//1.1计算指标
	while (closeite != stockdatas._vClose.end() && lowite != stockdatas._vLow.end() && highite != stockdatas._vHigh.end())
	{
		//1.1
		thisDayPrice.Inition(*openite, *closeite, *highite, *lowite, *volite, *dayIte);
		//1.2迭代器后移
		++dayIte; ++highite; ++lowite; ++closeite; ++openite; ++volite;
		//1.3计算指标
#ifndef UNBUILT_MACD
		hMacd.GetNextMacd(thisDayPrice, currentIndicators._MacdData);
#endif
#ifndef UNBUILT_MACD
		hemv.GetNextEmv(thisDayPrice, currentIndicators._Emv);
#endif
#ifndef UNBUILT_MACD
		hDMA.GetNextDMA(thisDayPrice, currentIndicators._DMAData);
#endif
#ifndef UNBUILT_MACD
		hTRIX.GetNextTRIX(thisDayPrice, currentIndicators._TrixData);
#endif
#ifndef UNBUILT_MACD
		hKDJ.GetNextKDJ(thisDayPrice, currentIndicators._Kdj);
#endif
#ifndef UNBUILT_MACD
		hAsi.GetNextASI(thisDayPrice, currentIndicators._Asi);
#endif
#ifndef UNBUILT_MACD
		hCdp.GetNextCDP(thisDayPrice, currentIndicators._Cdp);
#endif
#ifndef UNBUILT_MACD
		hDmi.GetNextDMI(thisDayPrice, currentIndicators._Dmi);
#endif
#ifndef UNBUILT_MACD
		hma.GetNextMa(thisDayPrice, currentIndicators._Ma);
#endif
#ifndef UNBUILT_MACD
		ArBrVrPsy.GetNextArBrVrPsy(thisDayPrice, currentIndicators._ArBrVrPsy);
#endif
#ifndef UNBUILT_MACD
 		hGI.GetNextGI(thisDayPrice, currentIndicators._GI);
#endif
#ifndef UNBUILT_MACD
		hbool.GetNextBool(thisDayPrice, currentIndicators._Bool);
#endif
#ifndef UNBUILT_MACD
		hrsi.GetNextRSI(thisDayPrice, currentIndicators._rsi);
#endif
		currentIndicators._CumulativeChangeRate = thisDayPrice._Close / stockdatas._vOpen.front();
		//1.4保存计算完的各类指标数据到对应的Vector当中
		PushBackIndex(currentIndicators, stockdatas);
	}
	stockdatas.SetDate();
	return true;
}


void CIndicatorsCalculator::PushBackIndex(const SigDayTechIndex& AllIndex, StockDataTable& datas)
{
	//累计变化率
	datas._vCumulativeChangerate.push_back(AllIndex._CumulativeChangeRate);
	//MACD
#ifndef UNBUILT_MACD
	datas._vTableAllIndex[_ePriceChangeRate].push_back(AllIndex._GI.PriceChangeRate);
	datas._vTableAllIndex[_eVolChangeRate].push_back(AllIndex._GI.VolChangerate);
	datas._vTableAllIndex[_eDataEnable].push_back(AllIndex._GI.Dataable);
#endif
#ifndef UNBUILT_MACD
	datas._vTableAllIndex[_eMACD_BAR].push_back(AllIndex._MacdData.bar);
	datas._vTableAllIndex[_eMACD_MA12].push_back(AllIndex._MacdData.m12);
	datas._vTableAllIndex[_eMACD_MA26].push_back(AllIndex._MacdData.m26);
	datas._vTableAllIndex[_eMACD_DIFF].push_back(AllIndex._MacdData.dif);
	datas._vTableAllIndex[_eMACD_DEA].push_back(AllIndex._MacdData.dea);
#endif
#ifndef UNBUILT_DMA
	datas._vTableAllIndex[_eDMA_D].push_back(AllIndex._DMAData._DDD);
	datas._vTableAllIndex[_eDMA_A].push_back(AllIndex._DMAData._AMA);
#endif
#ifndef UNBUILT_TRIX
	datas._vTableAllIndex[_eTRIX_TRIX].push_back(AllIndex._TrixData._TRIX);
	datas._vTableAllIndex[_eTRIX_MA].push_back(AllIndex._TrixData._TRMA);
#endif
#ifndef UNBUILT_KDJ
	datas._vTableAllIndex[_eKDJ_K].push_back(AllIndex._Kdj.K_OF_KDJ);
	datas._vTableAllIndex[_eKDJ_D].push_back(AllIndex._Kdj.D_OF_KDJ);
	datas._vTableAllIndex[_eKDJ_J].push_back(AllIndex._Kdj.J_OF_KDJ);
#endif
#ifndef UNBUILT_ASI
	datas._vTableAllIndex[_eASI_I].push_back(AllIndex._Asi._asi);
	datas._vTableAllIndex[_eASI_T].push_back(AllIndex._Asi._asit);
#endif
#ifndef UNBUILT_MA
	datas._vTableAllIndex[_eMA_MA1].push_back(AllIndex._Ma.Ma1);
	datas._vTableAllIndex[_eMA_MA2].push_back(AllIndex._Ma.Ma2);
	datas._vTableAllIndex[_eMA_MA3].push_back(AllIndex._Ma.Ma3);
	datas._vTableAllIndex[_eMA_MA4].push_back(AllIndex._Ma.Ma4);
	datas._vTableAllIndex[_eMA_UPRATE].push_back(AllIndex._Ma.upRate);
#endif
#ifndef UNBUILT_CDP
	datas._vTableAllIndex[_eCDP_CDP].push_back(AllIndex._Cdp._CDP);
	datas._vTableAllIndex[_eCDP_AH].push_back(AllIndex._Cdp._AH_High);
	datas._vTableAllIndex[_eCDP_NH].push_back(AllIndex._Cdp._NH_NormalHigh);
	datas._vTableAllIndex[_eCDP_NL].push_back(AllIndex._Cdp._NL_NormalLow);
	datas._vTableAllIndex[_eCDP_AL].push_back(AllIndex._Cdp._AL_Low);
	datas._vTableAllIndex[_eCDP_LOWEST].push_back(AllIndex._Cdp._Lowest);
#endif
#ifndef UNBUILT_XR
	datas._vTableAllIndex[_eAR].push_back(AllIndex._ArBrVrPsy.AR);
	datas._vTableAllIndex[_eBR].push_back(AllIndex._ArBrVrPsy.BR);
	datas._vTableAllIndex[_eCR].push_back(AllIndex._ArBrVrPsy.CR);
	datas._vTableAllIndex[_eCRMA1].push_back(AllIndex._ArBrVrPsy.CRMa1);
	datas._vTableAllIndex[_eCRMA2].push_back(AllIndex._ArBrVrPsy.CRMa2);
	datas._vTableAllIndex[_eCRMA3].push_back(AllIndex._ArBrVrPsy.CRMa3);
	datas._vTableAllIndex[_eCRMA4].push_back(AllIndex._ArBrVrPsy.CRMa4);
	datas._vTableAllIndex[_eVR].push_back(AllIndex._ArBrVrPsy.VR);
	datas._vTableAllIndex[_eVRMA].push_back(AllIndex._ArBrVrPsy.VRMA);
	datas._vTableAllIndex[_ePSY].push_back(AllIndex._ArBrVrPsy.PSY);
	datas._vTableAllIndex[_eLimitUpTime].push_back(AllIndex._ArBrVrPsy.LimitUpTimes);
	datas._vTableAllIndex[_eLimitDownTime].push_back(AllIndex._ArBrVrPsy.LimitDownTimes);
#endif
#ifndef UNBUILT_EMV
	datas._vTableAllIndex[_eEMV].push_back(AllIndex._Emv.emv);
	datas._vTableAllIndex[_eEMVMA].push_back(AllIndex._Emv.emvma);
#endif
#ifndef UNBUILT_BOOL
	datas._vTableAllIndex[_eBOOLMid].push_back(AllIndex._Bool.MidPrice);
	datas._vTableAllIndex[_eBOOLSTD].push_back(AllIndex._Bool.PriceSTD);
	datas._vTableAllIndex[_eBOOLUp].push_back(AllIndex._Bool.Uper);
	datas._vTableAllIndex[_eBOOLDown].push_back(AllIndex._Bool.Downer);
	datas._vTableAllIndex[_eBOOLCDP].push_back(AllIndex._Bool.cdp);
	datas._vTableAllIndex[_eBOOLCDPAUP].push_back(AllIndex._Bool.cdpAUp);
	datas._vTableAllIndex[_eBOOLCDPNUP].push_back(AllIndex._Bool.cdpNUp);
	datas._vTableAllIndex[_eBOOLCDPNDown].push_back(AllIndex._Bool.cdpNDown);
	datas._vTableAllIndex[_eBOOLCDPADown].push_back(AllIndex._Bool.cdpADown);
#endif
#ifndef UNBUILT_DMI
	datas._vTableAllIndex[_eDMI_DIP].push_back(AllIndex._Dmi._DIP);
	datas._vTableAllIndex[_eDMI_DIN].push_back(AllIndex._Dmi._DIN);
	datas._vTableAllIndex[_eDMI_ADX].push_back(AllIndex._Dmi._ADX);
	datas._vTableAllIndex[_eDMI_ADXR].push_back(AllIndex._Dmi._ADXR);
#endif
#ifndef UNBUILT_RSI
	datas._vTableAllIndex[_eRSI1].push_back(AllIndex._rsi.RSI1);
	datas._vTableAllIndex[_eRSI2].push_back(AllIndex._rsi.RSI2);
	datas._vTableAllIndex[_eRSI3].push_back(AllIndex._rsi.RSI3);
#endif
}

void CIndicatorsCalculator::Inition()
{
	currentIndicators.Inition();
	hGI.Inition();
	hMacd.Inition();
	hDMA.Inition();
	hTRIX.Inition();
	hKDJ.Inition();
	hAsi.Inition();
	hDmi.Inition();
	hma.Inition();
	hemv.Inition();
	hbool.Inition();
	hrsi.Inition();
	ArBrVrPsy.Inition();
	SQLTool.IniMysqlTool();
}

bool CIndicatorsCalculator::SaveTempIndicators(string& _stockName, BasisCycleType cycle, CDate _date)
{
	DataTypeToSave IndicatorsType;
	vector<StockDataType> datasToSave;

	IndicatorsType.Inition(_stockName, cycle, _date,"MACD");
	hMacd.CurrentDataToVecter(datasToSave, currentIndicators._MacdData);
	SQLTool.SaveTemporaryIndicatorsData(IndicatorsType, datasToSave);

	IndicatorsType.Inition(_stockName, cycle, _date, "KDJ");
	hKDJ.CurrentDataToVecter(datasToSave, currentIndicators._Kdj);
	SQLTool.SaveTemporaryIndicatorsData(IndicatorsType, datasToSave);

	return true;
}

bool CIndicatorsCalculator::GetTempIndicators(StockDataTable& mValue,const string& _stockName, BasisCycleType cycle, const CDate& _date)
{
	CComTime Timeer;
	DataTypeToSave IndicatorsType;
	vector<StockDataType> datasReadSQL;

	IndicatorsType.Inition(_stockName, cycle, _date, "MACD");
	Timeer.TimeBegin();
	SQLTool.GetTemporaryNumberData(IndicatorsType, datasReadSQL);
	if (datasReadSQL.empty()){
		LOG(ERROR) << "Can not get data from mysql.";
		return false;
	}
	Timeer.TimeEnd("GetTemporaryNumberData");

	hMacd.RecoveryDataFromVecter(datasReadSQL, currentIndicators._MacdData);
	IndicatorsType.Inition(_stockName, cycle, _date, "KDJ");
	SQLTool.GetTemporaryNumberData(IndicatorsType, datasReadSQL);
	if (datasReadSQL.empty()){
		LOG(ERROR) << "Can not get data from mysql.";
		return false;
	}
	hKDJ.RecoveryDataFromVecter(datasReadSQL, currentIndicators._Kdj);


	PushBackIndex(currentIndicators,mValue);
	return true;
}
