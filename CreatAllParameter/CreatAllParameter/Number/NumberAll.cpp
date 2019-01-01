#include "stdafx.h"
#include "NumberAll.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "..\glog\logging.h"

CNumberAll::CNumberAll()
{
	//FileNumber = 0;
}


CNumberAll::~CNumberAll()
{
}





////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CNumberAll::RaedDateFromFile(const string& strFilePath)
{
	bool IsLongitudinal  = false;//标志数据是否横向排列
	CNumberManager mCSVFileTool;

	mAll._vCloseData.clear();
	mAll._vHigh.clear();
	mAll._vLow.clear();
	mAll._vOpenData.clear();
	mAll._vTimeDay.clear();
	mAll._vVolume.clear();
	mAll._vMa5.clear();
	mAll._vMa10.clear();
	mAll._vMa20.clear();
	StringList lineString = mCSVFileTool.ReadDataFromCSVFileRanks(strFilePath);
	StringBlock AllString;
	if (lineString.size() < 3)
	{
		return;
	}
	//第一列当中有open的话则认为打开文件数据日期在第一行
	if (lineString.begin()->find("open") < 0)
	{
		IsLongitudinal  = true;
	}
	StringList vOneLinedata;
	for (StringList::iterator ite = lineString.begin(); ite != lineString.end(); ite++)
	{
		vOneLinedata = CutString(*ite, ",");
		AllString.push_back(vOneLinedata);
		vOneLinedata.clear();
	}
	//只处理日期是横向排列的文件
	if (!IsLongitudinal )
		ProcessingTransverseData(AllString);
	else
	{
		cout << "Error:Can not do.";
	}
}


bool CNumberAll::AnalyseTheFile(const string& filename, const string& FilePath)
{
	//清除上次分析的文件所有指标数据
	mAll.clear();
	//从文件中读出需要的数据
	RaedDateFromFile(FilePath + "\\" + filename);
	if (!ChackDataSizeRaedFromFile())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	LOG(INFO)
		<< "************************************"
		<< filename
		<< "************************************" << endl;
	//计算类
	CMacdManager hMacd;
	CDMA hDMA;
	CTRIX hTRIX;
	CKDJ hKDJ;
	CAsi hAsi;
	CCDP hCdp;
	CDMI hDmi;
	//结果输出
	SigDayTechIndex AllIndex;
	//状态记录
	StateRecords hStateRecode;
	hStateRecode.Inition();
	//一天的价格
	DatePriceData OneDayPrice;
	//账号
	CStockAccount hAccount(filename,10000);
	index = 0;
	//用于标记是否可以卖出，刚开始没有买入任何股票，所以不可以卖出
	_IsInSale = false;
	//要遍历的所有数据
	vector<string>::const_iterator dayIte = mAll._vTimeDay.begin();
	VStockData::const_iterator closeite = mAll._vCloseData.begin();
	VStockData::const_iterator highite = mAll._vHigh.begin();
	VStockData::const_iterator lowite = mAll._vLow.begin();
	VStockData::const_iterator openite = mAll._vOpenData.begin();
	//1.1计算指标
	while (closeite != mAll._vCloseData.end() && lowite != mAll._vLow.end() && highite != mAll._vHigh.end())
	{
		OneDayPrice._Close = *closeite;
		OneDayPrice._High = *highite;
		OneDayPrice._Low = *lowite;
		OneDayPrice._Open = *openite;
		OneDayPrice.mDate.dateTime = *dayIte;
		hMacd.GetNextMacd(OneDayPrice, AllIndex._MacdData);
		hDMA.GetNextDMA(OneDayPrice, AllIndex._DMAData);
		hTRIX.GetNextTRIX(OneDayPrice, AllIndex._TrixData);
		hKDJ.GetNextKDJ(OneDayPrice, AllIndex._Kdj);
		hAsi.GetNextASI(OneDayPrice, AllIndex._Asi);
		hCdp.GetNextCDP(OneDayPrice, AllIndex._Cdp);
		hDmi.GetNextDMI(OneDayPrice, AllIndex._Dmi);
		//保存计算完的各类指标数据到对应的Vector当中
		PushBackIndex(AllIndex);
		//对每日的状态进行判断
// 		hStateRecode.DailyStateRecord(AllIndex, OneDayPrice);
// 		//
// 		if (index > 200)
// 			ToSimulationSL1(OneDayPrice, hStateRecode, hAccount);
		//迭代器后移
		++index;
		++dayIte;
		++highite;
		++lowite;
		++closeite;
		++openite;
	}

	//所有指标都已经计算完成
	//下面只有分析
// 	StockDataPointer dataPointer = GetDataPointers();
// 	hStateRecode.ResultPrint();
// 	hStateRecode.GetFinalTrend(dataPointer);
// 
// 	CheckAndPrintKDJMin(filename);
// 	
// 	resultFileCsv << filename <<","<< hAccount.GetProceedsRate(*_vCloseData.rbegin()) << endl;
// 	FileIndex++;
// 	cout << FilePath << ":" << FileIndex * 100 / FileNumber << "%" << endl;
// 	hAccount.PrintAllBusiness("D:\\StockFile\\Log\\Allbusiness.txt");

	return true;
}


void CNumberAll::ToSimulationSL1(const DatePriceData& data, StateRecords& hStateRecode, CStockAccount& _Account)
{
	VStockData::reverse_iterator iteKDJ1 = mAll._vJ.rbegin();
	VStockData::reverse_iterator iteKDJ2 = iteKDJ1 + 1;
	VStockData::reverse_iterator iteMACD1 = mAll._vMACDValue.rbegin();
	VStockData::reverse_iterator iteMACD2 = iteMACD1 + 1;
	VStockData::reverse_iterator iteASI1 = mAll._vAsi.rbegin();
	VStockData::reverse_iterator iteASI2 = iteASI1 + 1;
	static double tempmax = 0.0f;
	double nowProceedsRate = 0.0f;
	if (_Account.GetLastPrice() == 0)
		nowProceedsRate = 0.0f;
	else 
		nowProceedsRate = (data._Close - _Account.GetLastPrice()) / _Account.GetLastPrice();
	if (*iteKDJ1 > 0 && *iteKDJ2 <= 0
		&& *iteMACD1 > *iteMACD2
		&& *iteASI1 > *iteASI2
		&& _Account.GetPosition() < 0.01
		&& data._Ma5 > data._Ma20)
	{
		_Account.ChangeStockPosition(data._Close, data.mDate.dateTime,1);
		return;
	}
	if (_Account.GetPosition() > 0.0f && nowProceedsRate > 0.05)
	{
		_Account.ChangeStockPosition(data._Close, data.mDate.dateTime, 1);
		return;
	}

	if (_Account.GetPosition() > 0.0f && nowProceedsRate < -0.1)
	{
		_Account.SellOutAll(data._Close, data.mDate.dateTime);
		return;
	}
	return;
}

StockDataPointer CNumberAll::GetDataPointers()
{
	StockDataPointer allDataPointer;
	allDataPointer._pDay = &mAll._vTimeDay;
	allDataPointer._pCloseData = &mAll._vCloseData;
	allDataPointer._pOpenData = &mAll._vOpenData;
	allDataPointer._pHigh = &mAll._vHigh;
	allDataPointer._pLow = &mAll._vLow;
	allDataPointer._pAMAValue = &mAll._vAMAValue;
	allDataPointer._pMa12 = &mAll._vMa12;
	allDataPointer._pMa26 = &mAll._vMa26;
	allDataPointer._pDiff = &mAll._vDiff;
	allDataPointer._pDEA = &mAll._vDEA;
	allDataPointer._pDMAValue = &mAll._vDMAValue;
	allDataPointer._pAMAValue = &mAll._vAMAValue;
	allDataPointer._pTRIX = &mAll._vTRIX;
	allDataPointer._pTRMA = &mAll._vTRMA;
	allDataPointer._pK = &mAll._vK;
	allDataPointer._pD = &mAll._vD;
	allDataPointer._pJ = &mAll._vJ;
	allDataPointer._pAsi = &mAll._vAsi;
	allDataPointer._pAsit = &mAll._vAsit;
	return allDataPointer;
}

void CNumberAll::ToSimulationSL2(const float close, const string& date, StateRecords& hStateRecode, CStockAccount& _Account)
{

}

void CNumberAll::CheckAndPrintKDJMin(const string& filename)
{
	VStockData::reverse_iterator iteKDJ1 = mAll._vJ.rbegin();
	VStockData::reverse_iterator iteKDJ2 = iteKDJ1 + 1;
	VStockData::reverse_iterator iteMACD1 = mAll._vMACDValue.rbegin();
	VStockData::reverse_iterator iteMACD2 = iteMACD1 + 1;
	VStockData::reverse_iterator iteASI1 = mAll._vAsi.rbegin();
	VStockData::reverse_iterator iteASI2 = iteASI1 + 1;
	if (*iteKDJ1 <= 0 && *iteKDJ2 <= 0)
		resultFile.get() << filename << "  KDJMin" << endl;
}




void CNumberAll::ProcessingTransverseData(const StringBlock& AllString)
{
	VStockData* plist = nullptr;
	for (StringBlock::const_iterator iteB = AllString.begin(); iteB != AllString.end(); iteB++)
	{
		if (iteB->size() < 10)
			continue;
		plist = nullptr;
		if (*(iteB->begin()) == "open")
			plist = &mAll._vOpenData;
		if (*(iteB->begin()) == "close")
			plist = &mAll._vCloseData;
		if (*(iteB->begin()) == "high")
			plist = &mAll._vHigh;
		if (*(iteB->begin()) == "low")
			plist = &mAll._vLow;
		if (*(iteB->begin()) == "volume")
			plist = &mAll._vVolume;
		if (*(iteB->begin()) == "v_ma5")
			plist = &mAll._vMa5;

		if (*(iteB->begin()) == "")//开头是空字符串的为日期行
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				mAll._vTimeDay.push_back(*iteL);
			}
			//reverse(_vTimeDay.begin(), _vTimeDay.end());//反转顺序
			continue;
		}

		if (nullptr != plist)
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				plist->push_back((float)atof(iteL->c_str()));
			}
		}
		//if (plist != nullptr)
			//reverse(plist->begin(), plist->end());//反转顺序
	}
}


void CNumberAll::PushBackIndex(const SigDayTechIndex& AllIndex)
{
	mAll._vMACDValue.push_back(AllIndex._MacdData.bar);//MACD
	mAll._vMa12.push_back(AllIndex._MacdData.m12);
	mAll._vMa26.push_back(AllIndex._MacdData.m26);
	mAll._vDiff.push_back(AllIndex._MacdData.diff);
	mAll._vDEA.push_back(AllIndex._MacdData.dea);
	mAll._vDMAValue.push_back(AllIndex._DMAData._DDD);//DMA
	mAll._vAMAValue.push_back(AllIndex._DMAData._AMA);
	mAll._vTRIX.push_back(AllIndex._TrixData._TRIX);//TRIX
	mAll._vTRMA.push_back(AllIndex._TrixData._TRMA);
	mAll._vK.push_back(AllIndex._Kdj.K_OF_KDJ);//KDJ
	mAll._vD.push_back(AllIndex._Kdj.D_OF_KDJ);
	mAll._vJ.push_back(AllIndex._Kdj.J_OF_KDJ);
	mAll._vAsi.push_back(AllIndex._Asi._asi);
	mAll._vAsit.push_back(AllIndex._Asi._asit);
}

bool CNumberAll::ChackDataSizeRaedFromFile()
{
	if (
		mAll._vCloseData.size() != mAll._vOpenData.size()
		|| mAll._vCloseData.size() != mAll._vHigh.size()
		|| mAll._vCloseData.size() != mAll._vLow.size()
		|| mAll._vCloseData.size() != mAll._vTimeDay.size()
		|| mAll._vCloseData.size() != mAll._vVolume.size()
		|| mAll._vCloseData.size() != mAll._vMa5.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}

const AllStockData& CNumberAll::GetAllValue()
{
	return mAll;
}


void AllStockData::clear()
{
	_vOpenData.clear();
	_vCloseData.clear();
	_vHigh.clear();
	_vLow.clear();
	_vVolume.clear();
	_vMa5.clear();
	_vMa10.clear();
	_vMa20.clear();
	_vTimeDay.clear();

	_vMACDValue.clear();
	_vMa12.clear();
	_vMa26.clear();
	_vDiff.clear();
	_vDEA.clear();
	_vDMAValue.clear();
	_vAMAValue.clear();
	_vTRIX.clear();
	_vTRMA.clear();
	_vtr.clear();
	_vtb.clear();
	_vta.clear();
	_vK.clear();
	_vD.clear();
	_vJ.clear();
	_vAsi.clear();
	_vAsit.clear();




}
