#include "stdafx.h"
#include "NumberAll.h"
#include "../TrendAnal.h"
#include <time.h>  
#include "..\glog\logging.h"

CNumberAll::CNumberAll()
{
	FileNumber = 0;
}


CNumberAll::~CNumberAll()
{
}
bool CNumberAll::RunProg(string strFolderPath)
{
	WIN32_FIND_DATA p;
	string mFilePath = strFolderPath + "\\*.csv";
	//LPCWSTR findPath = mFilePath.c_str();
	HANDLE h = FindFirstFile(mFilePath.c_str(), &p);
	//统计文件夹下的文件数
	FileNumber = 0;
	FileIndex = 0;
	while (FindNextFile(h, &p))
	{
		FileNumber++;
	}
	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return false;
	}

	//resultFile.open("D:\\StockFile\\resultfile.txt", fstream::out);
	resultFileCsv.open("D:\\StockFile\\resultfile.csv", fstream::out);
	if (! resultFileCsv)
		return false;
	beginIndex = 100;
	vector<string> checkStockFileNames;
	checkStockFileNames.clear();
	//遍历所有文件
	do
	{ 
// 		srand((unsigned)time(NULL));
// 		int a = rand() % 2;
// 		cout << a << endl;
// 		if (a < 5)
			AnalyseTheFile(p.cFileName, strFolderPath);
// 		else
// 			Sleep(100);
// 			checkStockFileNames.push_back(p.cFileName);
	} while (FindNextFile(h, &p));
// 	for (vector<string>::iterator ite = checkStockFileNames.begin(); ite != checkStockFileNames.end(); ++ite)
// 		AnalyseTheFile(*ite, strFolderPath);

	resultFileCsv.close();
	return true;
}

void CNumberAll::UpdateFileColumn(string strFolderPath)
{
	RunProg(strFolderPath);
}

void CNumberAll::UpdatePathFilesRanks(string strPath)
{
	RunProg(strPath);
}

void CNumberAll::UpdateFileRanks(string strfilename)
{
}

void CNumberAll::RaedDateFromFile(const string& strFilePath)
{
	bool IsLongitudinal  = false;//标志数据是否横向排列

	_vCloseData.clear();
	_vHigh.clear();
	_vLow.clear();
	_vOpenData.clear();
	_vTimeDay.clear();
	_vVolume.clear();
	_vMa5.clear();
	_vMa10.clear();
	_vMa20.clear();
	StringList lineString = ReadDataFromCSVFileRanks(strFilePath);
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
	//清除上次分析的文件所有指标数据
	ClearAllIndex();
	//一天的价格
	DatePriceData OneDayPrice;
	//账号
	CStockAccount hAccount(filename,10000);
	index = 0;
	//用于标记是否可以卖出，刚开始没有买入任何股票，所以不可以卖出
	_IsInSale = false;
	//要遍历的所有数据
	vector<string>::const_iterator dayIte = _vTimeDay.begin();
	VStockData::const_iterator closeite = _vCloseData.begin();
	VStockData::const_iterator highite = _vHigh.begin();
	VStockData::const_iterator lowite = _vLow.begin();
	VStockData::const_iterator openite = _vOpenData.begin();
	//1.1计算指标
	while (closeite != _vCloseData.end() && lowite != _vLow.end() && highite != _vHigh.end())
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
		//保存数据
		PushBackIndex(AllIndex);
		//对每日的状态进行判断
		hStateRecode.DailyStateRecord(AllIndex, OneDayPrice);
		//
		if (index > 200)
			ToSimulationSL1(OneDayPrice, hStateRecode, hAccount);
		//迭代器后移
		++index;
		++dayIte;
		++highite;
		++lowite;
		++closeite;
		++openite;
	}

	StockDataPointer dataPointer = GetDataPointers();
	hStateRecode.ResultPrint();
	hStateRecode.GetFinalTrend(dataPointer);

	CheckAndPrintKDJMin(filename);
	//
	resultFileCsv << filename <<","<< hAccount.GetProceedsRate(*_vCloseData.rbegin()) << endl;
	FileIndex++;
	cout << FilePath << ":" << FileIndex * 100 / FileNumber << "%" << endl;
	hAccount.PrintAllBusiness("D:\\StockFile\\Log\\Allbusiness.txt");
	//保存到文件
	SaveDataToFile(FilePath + "\\" + filename);
	return true;
}

bool CNumberAll::SaveDataToFile(const string& strFilePath)
{
// 	ReSavefileRanks(strFilePath, _vAsi, "ASI");
// 	ReSavefileRanks(strFilePath, _vAsit, "ASIT");
// 	ReSavefileRanks(strFilePath, _vDEA, MACD_DEA);
// 	ReSavefileRanks(strFilePath, _vDiff, MACD_DIFF);
	ReSavefileRanks(strFilePath, _vMACDValue, MACD_BAR);
	ReSavefileRanks(strFilePath, _vDMAValue, DMA_D);
	ReSavefileRanks(strFilePath, _vAMAValue, DMA_A);
// 	ReSavefileRanks(strFilePath, _vTRIX, TRIX_TRIX);
// 	ReSavefileRanks(strFilePath, _vTRMA, TRIX_MA);
	ReSavefileRanks(strFilePath, _vK, KDJ_K);
	ReSavefileRanks(strFilePath, _vD, KDJ_D);
	ReSavefileRanks(strFilePath, _vJ, KDJ_J);
// 	ReSavefileRanks(strFilePath, vtr, TRIX_VTR);
// 	ReSavefileRanks(strFilePath, vtb, TRIX_VTB);
// 	ReSavefileRanks(strFilePath, vta, TRIX_VTA);
	return true;
}

void CNumberAll::ToSimulationSL1(const DatePriceData& data, StateRecords& hStateRecode, CStockAccount& _Account)
{
	VStockData::reverse_iterator iteKDJ1 = _vJ.rbegin();
	VStockData::reverse_iterator iteKDJ2 = iteKDJ1 + 1;
	VStockData::reverse_iterator iteMACD1 = _vMACDValue.rbegin();
	VStockData::reverse_iterator iteMACD2 = iteMACD1 + 1;
	VStockData::reverse_iterator iteASI1 = _vAsi.rbegin();
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
// 	if (*iteMACD1 < *iteMACD2 && *iteASI1 < *iteASI2)
// 	{
// 		_Account.SellOutAll(data._Close, data.mDate.dateTime);
// 		return;
// 	}
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
	allDataPointer._pDay = &_vTimeDay;
	allDataPointer._pCloseData = &_vCloseData;
	allDataPointer._pOpenData = &_vOpenData;
	allDataPointer._pHigh = &_vHigh;
	allDataPointer._pLow = &_vLow;
	allDataPointer._pAMAValue = &_vAMAValue;
	allDataPointer._pMa12 = &_vMa12;
	allDataPointer._pMa26 = &_vMa26;
	allDataPointer._pDiff = &_vDiff;
	allDataPointer._pDEA = &_vDEA;
	allDataPointer._pDMAValue = &_vDMAValue;
	allDataPointer._pAMAValue = &_vAMAValue;
	allDataPointer._pTRIX = &_vTRIX;
	allDataPointer._pTRMA = &_vTRMA;
	allDataPointer._pK = &_vK;
	allDataPointer._pD = &_vD;
	allDataPointer._pJ = &_vJ;
	allDataPointer._pAsi = &_vAsi;
	allDataPointer._pAsit = &_vAsit;
	return allDataPointer;
}

void CNumberAll::ToSimulationSL2(const float close, const string& date, StateRecords& hStateRecode, CStockAccount& _Account)
{

}

void CNumberAll::CheckAndPrintKDJMin(const string& filename)
{
	VStockData::reverse_iterator iteKDJ1 = _vJ.rbegin();
	VStockData::reverse_iterator iteKDJ2 = iteKDJ1 + 1;
	VStockData::reverse_iterator iteMACD1 = _vMACDValue.rbegin();
	VStockData::reverse_iterator iteMACD2 = iteMACD1 + 1;
	VStockData::reverse_iterator iteASI1 = _vAsi.rbegin();
	VStockData::reverse_iterator iteASI2 = iteASI1 + 1;
	if (*iteKDJ1 <= 0 && *iteKDJ2 <= 0 && *iteMACD1 > *iteMACD2 && *iteASI1 > *iteASI2)
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
			plist = &_vOpenData;
		if (*(iteB->begin()) == "close")
			plist = &_vCloseData;
		if (*(iteB->begin()) == "high")
			plist = &_vHigh;
		if (*(iteB->begin()) == "low")
			plist = &_vLow;
		if (*(iteB->begin()) == "volume")
			plist = &_vVolume;
		if (*(iteB->begin()) == "v_ma5")
			plist = &_vMa5;

		if (*(iteB->begin()) == "")//开头是空字符串的为日期行
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				_vTimeDay.push_back(*iteL);
			}
			reverse(_vTimeDay.begin(), _vTimeDay.end());
			continue;
		}

		if (nullptr != plist)
		{
			for (StringList::const_iterator iteL = (++iteB->begin()); iteL != iteB->end(); iteL++)
			{
				plist->push_back((float)atof(iteL->c_str()));
			}
		}
		if (plist != nullptr)
			reverse(plist->begin(), plist->end());
	}
}

void CNumberAll::ClearAllIndex()
{
	//清理上次的数据
	_vMACDValue.clear();//MACD
	_vMa12.clear();
	_vMa26.clear();
	_vDiff.clear();
	_vDEA.clear();
	_vDMAValue.clear();//DMA
	_vAMAValue.clear();
	_vTRIX.clear();//TRIX
	_vTRMA.clear();
	_vK.clear();//KDJ
	_vD.clear();
	_vJ.clear();
	_vAsi.clear();
	_vAsit.clear();
}

void CNumberAll::PushBackIndex(const SigDayTechIndex& AllIndex)
{
	_vMACDValue.push_back(AllIndex._MacdData.bar);//MACD
	_vMa12.push_back(AllIndex._MacdData.m12);
	_vMa26.push_back(AllIndex._MacdData.m26);
	_vDiff.push_back(AllIndex._MacdData.diff);
	_vDEA.push_back(AllIndex._MacdData.dea);
	_vDMAValue.push_back(AllIndex._DMAData._DDD);//DMA
	_vAMAValue.push_back(AllIndex._DMAData._AMA);
	_vTRIX.push_back(AllIndex._TrixData._TRIX);//TRIX
	_vTRMA.push_back(AllIndex._TrixData._TRMA);
	_vK.push_back(AllIndex._Kdj.K_OF_KDJ);//KDJ
	_vD.push_back(AllIndex._Kdj.D_OF_KDJ);
	_vJ.push_back(AllIndex._Kdj.J_OF_KDJ);
	_vAsi.push_back(AllIndex._Asi._asi);
	_vAsit.push_back(AllIndex._Asi._asit);
}

bool CNumberAll::ChackDataSizeRaedFromFile()
{
	if (
		_vCloseData.size() != _vOpenData.size()
		|| _vCloseData.size() != _vHigh.size()
		|| _vCloseData.size() != _vLow.size()
		|| _vCloseData.size() != _vTimeDay.size()
		|| _vCloseData.size() != _vVolume.size()
		|| _vCloseData.size() != _vMa5.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}
