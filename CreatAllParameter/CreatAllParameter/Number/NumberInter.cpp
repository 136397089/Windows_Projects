#include "stdafx.h"
#include "NumberInter.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "..\glog\logging.h"
#include "../PriChag.h"
CNumberInterface::CNumberInterface()
{
	//FileNumber = 0;
}


CNumberInterface::~CNumberInterface()
{
}





////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void CNumberInterface::RaedDateFromFile(const string& strFilePath)
{
	bool IsLongitudinal  = false;//标志数据是否横向排列
	CNumberManager mCSVFileTool;

	mAll._vCloseData.clear();
	mAll._vHigh.clear();
	mAll._vLow.clear();
	mAll._vOpenData.clear();
	mAll._vTimeDay.clear();
	mAll._vVolume.clear();

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


bool CNumberInterface::AnalyseTheFile(const string& filename, const string& FilePath)
{
	//清除上次分析的文件所有指标数据
	mAll.clear();
	//从文件中读出需要的数据
	RaedDateFromFile(FilePath + "\\" + filename);
	if (!mAll.ChackDataSize())
	{
		LOG(ERROR) << "NumberAll error: size of data read from file is not equal.";
		return false;
	}
	LOG(INFO)
		<< "************************************"
		<< filename
		<< "************************************" << endl;
	//定义参数
#define MACD_INDEX1 12.0f
#define MACD_INDEX2 26.0f
#define KDJ_INDEX1 9.0f
#define KDJ_INDEX2 3.0f
#define KDJ_INDEX3 3.0f
	//计算类
	CChagRate hPriChaRate;
	CChagRate hVolChaRate;
	CMacdManager hMacd(MACD_INDEX1, MACD_INDEX2,9);
	CDMA hDMA;
	CTRIX hTRIX;

	CKDJ hKDJ(KDJ_INDEX1, KDJ_INDEX2, KDJ_INDEX3);
	CAsi hAsi;
	CCDP hCdp;
	CDMI hDmi;
	CMa hma(5, 10, 20, 60);
	//结果输出
	SigDayTechIndex AllIndex;
	//状态记录
// 	StatusRecords hStateRecode;
// 	hStateRecode.Inition();
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
	VStockData::const_iterator volite = mAll._vVolume.begin();
	//1.1计算指标
	while (closeite != mAll._vCloseData.end() && lowite != mAll._vLow.end() && highite != mAll._vHigh.end())
	{
		OneDayPrice._Close = *closeite;
		OneDayPrice._High = *highite;
		OneDayPrice._Low = *lowite;
		OneDayPrice._Open = *openite;
		OneDayPrice._Volume = *volite;

		OneDayPrice.mDate.SetDay(*dayIte);
 		hMacd.GetNextMacd(OneDayPrice, AllIndex._MacdData);
		hDMA.GetNextDMA(OneDayPrice, AllIndex._DMAData);
		hTRIX.GetNextTRIX(OneDayPrice, AllIndex._TrixData);
		hKDJ.GetNextKDJ(OneDayPrice, AllIndex._Kdj);
		hAsi.GetNextASI(OneDayPrice, AllIndex._Asi);
		hCdp.GetNextCDP(OneDayPrice, AllIndex._Cdp);
		hDmi.GetNextDMI(OneDayPrice, AllIndex._Dmi);
		hma.GetNextMa(OneDayPrice, AllIndex._Ma);
		hPriChaRate.GetNextChangeRate(OneDayPrice._Close, AllIndex._Pchangerate);
		hVolChaRate.GetNextChangeRate(OneDayPrice._Volume, AllIndex._Volchagrate);
		//保存计算完的各类指标数据到对应的Vector当中
		PushBackIndex(AllIndex);
		//迭代器后移
		++index;
		++dayIte;
		++highite;
		++lowite;
		++closeite;
		++openite;
	}

	return true;
}



// StockDataPointer CNumberInterface::GetDataPointers()
// {
// 	StockDataPointer allDataPointer;
// 	allDataPointer._pDay = &mAll._vTimeDay;
// 	allDataPointer._pCloseData = &mAll._vCloseData;
// 	allDataPointer._pOpenData = &mAll._vOpenData;
// 	allDataPointer._pHigh = &mAll._vHigh;
// 	allDataPointer._pLow = &mAll._vLow;
// 	allDataPointer._pAMAValue = &mAll._vAMAValue;
// 	allDataPointer._pMa12 = &mAll._vMACDMa12;
// 	allDataPointer._pMa26 = &mAll._vMACDMa26;
// 	allDataPointer._pDiff = &mAll._vDiff;
// 	allDataPointer._pDEA = &mAll._vDEA;
// 	allDataPointer._pDMAValue = &mAll._vDMAValue;
// 	allDataPointer._pAMAValue = &mAll._vAMAValue;
// 	allDataPointer._pTRIX = &mAll._vTRIX;
// 	allDataPointer._pTRMA = &mAll._vTRMA;
// 	allDataPointer._pK = &mAll._vK;
// 	allDataPointer._pD = &mAll._vD;
// 	allDataPointer._pJ = &mAll._vJ;
// 	allDataPointer._pAsi = &mAll._vAsi;
// 	allDataPointer._pAsit = &mAll._vAsit;
// 	return allDataPointer;
// }



void CNumberInterface::CheckAndPrintKDJMin(const string& filename)
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




void CNumberInterface::ProcessingTransverseData(const StringBlock& AllString)
{
	VStockData* plist = nullptr;
	for (StringBlock::const_iterator iteB = AllString.begin(); iteB != AllString.end(); iteB++)
	{
		if (iteB->size() < 10)
			continue;
		plist = nullptr;
		if (*(iteB->begin()) == File_Open_INDEX)
			plist = &mAll._vOpenData;
		if (*(iteB->begin()) == File_Close_INDEX)
			plist = &mAll._vCloseData;
		if (*(iteB->begin()) == File_High_INDEX)
			plist = &mAll._vHigh;
		if (*(iteB->begin()) == File_Low_INDEX)
			plist = &mAll._vLow;
		if (*(iteB->begin()) == File_Volume_INDEX)
			plist = &mAll._vVolume;
// 		if (*(iteB->begin()) == "v_ma5")
// 			plist = &mAll._vMa5;

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


void CNumberInterface::PushBackIndex(const SigDayTechIndex& AllIndex)
{
	mAll._vMACDValue.push_back(AllIndex._MacdData.bar);//MACD
	mAll._vMACDMa12.push_back(AllIndex._MacdData.m12);
	mAll._vMACDMa26.push_back(AllIndex._MacdData.m26);
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
	mAll._vMa1.push_back(AllIndex._Ma.Ma1);
	mAll._vMa2.push_back(AllIndex._Ma.Ma2);
	mAll._vMa3.push_back(AllIndex._Ma.Ma3);
	mAll._vMa4.push_back(AllIndex._Ma.Ma4);
	mAll._vPriChaRate.push_back(AllIndex._Pchangerate);
	mAll._vVolChaRate.push_back(AllIndex._Volchagrate);
}


const AllStockData& CNumberInterface::GetAllValue()
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
	_vTimeDay.clear();
	_vMa1.clear();
	_vMa2.clear();
	_vMa3.clear();
	_vMa4.clear();

	_vMACDValue.clear();
	_vMACDMa12.clear();
	_vMACDMa26.clear();
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
	_vPriChaRate.clear();




}


#define COPYVECTOR(VECTORNAME,BEGININDEX,ENDINDEX)\
		if (	ENDINDEX < BEGININDEX ||\
			BEGININDEX >= VECTORNAME.size())\
			temp.VECTORNAME = VECTORNAME;\
		else if(ENDINDEX >= VECTORNAME.size())\
			temp.VECTORNAME.insert(temp.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.end());\
		else\
			temp.VECTORNAME.insert(temp.VECTORNAME.end(),\
								VECTORNAME.begin() + BEGININDEX,\
								VECTORNAME.begin() + ENDINDEX)
AllStockData AllStockData::NewDataByIndex(unsigned int beginindex, unsigned int endindex) const
{
	AllStockData temp;

	//用宏截取新的vector
	COPYVECTOR(_vTimeDay, beginindex, endindex);
	COPYVECTOR(_vCloseData, beginindex, endindex);
	COPYVECTOR(_vOpenData, beginindex, endindex);
	COPYVECTOR(_vHigh, beginindex, endindex);
	COPYVECTOR(_vLow, beginindex, endindex);
	COPYVECTOR(_vVolume, beginindex, endindex);
	COPYVECTOR(_vPriChaRate, beginindex, endindex);

	return temp;
}

bool AllStockData::ChackDataSize()
{
	if (
		_vCloseData.size() != _vOpenData.size()
		|| _vCloseData.size() != _vHigh.size()
		|| _vCloseData.size() != _vLow.size()
		|| _vCloseData.size() != _vTimeDay.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}


//
StockDataPointer AllStockData::GetAllPointer() const
{
	StockDataPointer indexPointerMap;
	indexPointerMap[_eFile_Open_INDEX] = &_vOpenData;
	indexPointerMap[_eFile_Close_INDEX] = &_vCloseData;
	indexPointerMap[_eFile_High_INDEX] = &_vHigh;
	indexPointerMap[_eFile_Low_INDEX] = &_vLow;
	indexPointerMap[_eFile_Volume_INDEX] = &_vVolume;
	indexPointerMap[_ePRICECHANGE] = &_vPriChaRate;
	indexPointerMap[_eVOLHANGE] = &_vVolChaRate;
	indexPointerMap[_eMA_MA5] = &_vMa1;
	indexPointerMap[_eMA_MA10] = &_vMa2;
	indexPointerMap[_eMA_MA20] = &_vMa3;
	indexPointerMap[_eMA_MA40] = &_vMa4;
	indexPointerMap[_eMACD_BAR] = &_vMACDValue;
	indexPointerMap[_eMACD_MA12] = &_vMACDMa12;
	indexPointerMap[_eMACD_MA26] = &_vMACDMa26;
	indexPointerMap[_eMACD_DIFF] = &_vDiff;
	indexPointerMap[_eMACD_DEA] = &_vDEA;
	indexPointerMap[_eDMA_D] = &_vDMAValue;
	indexPointerMap[_eDMA_A] = &_vAMAValue;
	indexPointerMap[_eTRIX_TRIX] = &_vTRIX;
	indexPointerMap[_eTRIX_MA] = &_vTRMA;
	indexPointerMap[_eTRIX_VTR] = &_vtr;
	indexPointerMap[_eTRIX_VTB] = &_vtb;
	indexPointerMap[_eTRIX_VTA] = &_vta;
	indexPointerMap[_eKDJ_K] = &_vK;
	indexPointerMap[_eKDJ_D] = &_vD;
	indexPointerMap[_eKDJ_J] = &_vJ;
	indexPointerMap[_eASI_I] = &_vAsi;
	indexPointerMap[_eASI_T] = &_vAsit;

	return indexPointerMap;
}
//
StockDataPointer AllStockData::GetIndexPointer() const
{
	StockDataPointer indexPointerMap;
	indexPointerMap[_eMA_MA5] = &_vMa1;
	indexPointerMap[_eMA_MA10] = &_vMa2;
	indexPointerMap[_eMA_MA20] = &_vMa3;
	indexPointerMap[_eMA_MA40] = &_vMa4;
	indexPointerMap[_eMACD_BAR] = &_vMACDValue;
	indexPointerMap[_eMACD_MA12] = &_vMACDMa12;
	indexPointerMap[_eMACD_MA26] = &_vMACDMa26;
	indexPointerMap[_eMACD_DIFF] = &_vDiff;
	indexPointerMap[_eMACD_DEA] = &_vDEA;
	indexPointerMap[_eDMA_D] = &_vDMAValue;
	indexPointerMap[_eDMA_A] = &_vAMAValue;
	indexPointerMap[_eTRIX_TRIX] = &_vTRIX;
	indexPointerMap[_eTRIX_MA] = &_vTRMA;
	indexPointerMap[_eTRIX_VTR] = &_vtr;
	indexPointerMap[_eTRIX_VTB] = &_vtb;
	indexPointerMap[_eTRIX_VTA] = &_vta;
	indexPointerMap[_eKDJ_K] = &_vK;
	indexPointerMap[_eKDJ_D] = &_vD;
	indexPointerMap[_eKDJ_J] = &_vJ;
	indexPointerMap[_eASI_I] = &_vAsi;
	indexPointerMap[_eASI_T] = &_vAsit;

	return indexPointerMap;
}




