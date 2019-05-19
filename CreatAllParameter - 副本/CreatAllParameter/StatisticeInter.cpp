#include "stdafx.h"
#include <numeric>
#include <complex>//复数
#include <cmath>
// #include <afxmt.h>
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "DFT.h"//快速傅立叶
#include "glog/logging.h"//Log工具
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
#include "FreqStatistice.h"

CRITICAL_SECTION filterDataMeancs;

CStatisticeInter::CStatisticeInter():
moveVarDaySize(100)
{

}


CStatisticeInter::~CStatisticeInter()
{
	DeleteCriticalSection(&filterDataMeancs);
}
//
bool CStatisticeInter::Inition()
{
	InitializeCriticalSection(&filterDataMeancs);
	return true;
}
//
bool CStatisticeInter::Inter(
	const StockDataTable& daydata,
	const StockDataTable& weekdata,
	const StockDataTable& mounthdata,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate)
{
	Inition();

	vector<unsigned int> MALineResult1;
	vector<unsigned int> MALineResult2;
	MALineStatistice(daydata._vMa1, daydata._vMa4, MALineResult1, MALineResult2);
	return true;

	CcdpStatistics CDPStaTool;
	CDPStaTool.CountCDPData(weekdata);
	simulation(daydata,weekdata);

	MACD_EDA_Statistice(daydata, daystate.allIndexStates[_eMACD_BAR]);
	//GroupFreqStatistice(allnumber, stateinter);
	return true;
}




bool CStatisticeInter::MACD_EDA_Statistice(const StockDataTable& allnumber, const StateTable& _State)
{
	const StatePointsList& StaticList = _State._staticstate;
	//输入检查
	if (StaticList.size() < DATAMINSIZE)
	{
		_LastError = "StaticList  size error. In MACD_EDA_Statistice.";
		return false;
	}

	CFreqStatistice freqStatTool;
	CStatisticeTool statictool;
	FreqListType vfreqlist;

	StockDataTable newdata;
	VStockData PriceChangeResult;

	unsigned int backIndex = 0;
	unsigned int frontIndex = 0;
	StaticResults changerate;
	vector<StaticResults> Upresults;
	vector<StaticResults> Downresults;
	vector<StaticResults> Allresules;
	for (unsigned int i = 0; i < StaticList.size(); i++)
	{
		backIndex = frontIndex;
		frontIndex = StaticList[i]._TimeIndex;

		if (frontIndex <= backIndex)
			continue;
		newdata = allnumber.NewDataByIndex(frontIndex, frontIndex + 30);
		//统计区间上涨和下跌的次数比例
		freqStatTool.Inition();
		freqStatTool.GetGroupFrqu(newdata._vPriChaRate, vfreqlist);
		float rise = freqStatTool.GetFreqByValue(0.00001f, 1.0f, vfreqlist);
		float fall = freqStatTool.GetFreqByValue(-1.0f, -0.00001f, vfreqlist);
		changerate.RiseDayRate = rise / (float)newdata._vPriChaRate.size();
		changerate.FallDayRate = fall / (float)newdata._vPriChaRate.size();
		changerate.BeginDate.SetDay(newdata._vTimeDay[0]);
		changerate._IndexType = StaticList[i]._IndexType;
		//
		statictool.GetEveryDayChangeRate(newdata._vClose, PriceChangeResult);
		statictool.GetMaxChangeRates(PriceChangeResult, changerate);
		
		if (changerate._IndexType == _eStaticPositiveUpClose)
		{
			Upresults.push_back(changerate);
			Allresules.push_back(changerate);
		}
		else
		{
			Downresults.push_back(changerate);
			Allresules.push_back(changerate);
		}
	}
	Save_MACD_EDA_StatisticeResultTofile(Allresules);
	return true;
}


bool CStatisticeInter::Save_MACD_EDA_StatisticeResultTofile(vector<StaticResults>& result)
{
	fstream outfile("D:\\StockFile\\Log\\StatisticeResult.csv", ios::out);
	outfile
		<< "date" << ","
		<< "RiseRate" << ","
		<< "FallRate" << ","
		<< "RiseRate1" << ","
		<< "RiseRate2" << ","
		<< "RiseRate3" << ","
		<< "FallRate1" << ","
		<< "FallRate2" << ","
		<< "FallRate3" << ","
		<< "IndexType"
		<< endl;
	for (vector<StaticResults>::iterator ite = result.begin(); ite != result.end(); ite++)
	{
		outfile
		<< ite->BeginDate.GetDay()<<","
		<< ite->RiseDayRate << ","
		<< ite->FallDayRate << ","
		<< ite->RiseRate1 << ","
		<< ite->RiseRate2 << ","
		<< ite->RiseRate3 << ","
		<< ite->FallRate1 << ","
		<< ite->FallRate2 << ","
		<< ite->FallRate3 << ","
		<< ite->_IndexType << ",";
		outfile << endl;
	}

	outfile.close();
	return true;
}

//根据模拟CDP的高低点来买卖股票
void CStatisticeInter::simulation(const StockDataTable& daynumber, const StockDataTable& weeknumber)
{
	const VStockData &dayclose = daynumber._vClose;
	const VStockData &daylow = daynumber._vLow;
	const VStockData &dayhigh = daynumber._vHigh;
	const vector<string> &day = daynumber._vTimeDay;

	const VStockData &weekNL = weeknumber._vNL_NormalLow;
	const VStockData &weekKDJ_J = weeknumber._vJ;
// 	const VStockData &daylow = daynumber._vLow;
// 	const VStockData &dayhigh = daynumber._vHigh;
	CStockAccount account("000001",1000000);

	float StopLossLine1 = 0.95f;
	float StopLossLine2 = 0.90f;
	float StopLossLine3 = 0.98f;

	//CDFT DFTTool;
	//DFTDataList result = DFTTool.DFT(daynumber._vPSY);
	for (unsigned int i = DATAMINSIZE; i < day.size(); i++)
	{
		unsigned int iLastWeek = weeknumber.GetLastTimeIndexByDate(daynumber._vDate[i]);
		if (iLastWeek == 0)
			continue;
		if (daylow[i] < weekNL[iLastWeek] &&
			account.GetStockQuantityOwned() <= 0.01 &&
			daynumber._vPSY[i]   < 30)
		{
			account.AllIn(weekNL[iLastWeek], day[i]);
			continue;
		}
		if (dayhigh[i] / account.GetLastBuyPrice() >= 1.05f &&
			account.GetStockQuantityOwned() >= 1.0f)
		{
			account.SellOutAll(account.GetLastBuyPrice()*1.05f, day[i]);
			LOG(INFO) << day[i] << " Profit :" << account.GetCurrentAssets(0.0f)
				<< " Buy: " << account.GetLastBuyPrice() << "--"
				<< "Sell: " << account.GetLastSellPrice() << endl;
			continue;
		}
		if (dayhigh[i] >= weeknumber._vNH_NormalHigh[iLastWeek]&&
			account.GetStockQuantityOwned() >= 1.0f)
		{
			account.SellOutAll(weeknumber._vNH_NormalHigh[iLastWeek], day[i]);
			LOG(INFO) << day[i] << " Loss :" << account.GetCurrentAssets(0.0f)
				<< " Buy: " << account.GetLastBuyPrice() << "--"
				<< "Sell: " << account.GetLastSellPrice() << endl;
		}
		if (daylow[i] / account.GetLastBuyPrice() <= StopLossLine1 &&
			account.GetStockQuantityOwned() >= 1.0f
			&& daynumber._vAsit[i] - daynumber._vAsit[i - 1] < 0)
		{
			account.SellOutAll(account.GetLastBuyPrice() * StopLossLine1, day[i]);
			LOG(INFO) << day[i] << " Loss :" << account.GetCurrentAssets(0.0f)
				<< " Buy: " << account.GetLastBuyPrice() << "--"
				<< "Sell: " << account.GetLastSellPrice() << endl;
		}
		if (daylow[i] / account.GetLastBuyPrice() <= StopLossLine2 &&
			account.GetStockQuantityOwned() >= 1.0f)
		{
			account.SellOutAll(account.GetLastBuyPrice() * StopLossLine2, day[i]);
			LOG(INFO) << day[i] << " Loss :" << account.GetCurrentAssets(0.0f)
				<<" Buy: "<< account.GetLastBuyPrice() << "--"
				<<"Sell: "<< account.GetLastSellPrice()<< endl;
		}
	}
	return;
}

StockDataType CStatisticeInter::Autocorrelation(const VStockData& _inputdata, unsigned int interval)
{
	if (interval >= _inputdata.size())
	{
		return 0;
	}
	StockDataType Average = accumulate(_inputdata.begin(), _inputdata.end(), 0) / _inputdata.size();

	StockDataType Var = 0;
	for (unsigned int i = 0; i < _inputdata.size();i++)
	{
		Var = Var + (_inputdata[i] - Average)*(_inputdata[i] - Average);
	}

	for (unsigned int i = interval; i < _inputdata.size(); i++)
	{

	}
	return 0;
}
//对均线_madata1在均线_madata2之上和之下的时长进行统计
//返回的结果放在_UpCloseresult，_DownCloseresult当中
void CStatisticeInter::MALineStatistice(
	const VStockData& _madata1,
	const VStockData& _madata2,
	vector<unsigned int>& _UpCloseresult,
	vector<unsigned int>& _DownCloseresult)
{
	//
	_UpCloseresult.clear();
	_DownCloseresult.clear();
	if (_madata1.size() != _madata2.size() && _madata1.size() < DATAMINSIZE)
	{
		return;
	}
	unsigned int biggercount = 0;
	unsigned int lesscount = 0;
	VStockData::const_iterator iteMA1 = _madata1.begin();
	VStockData::const_iterator iteMA2 = _madata2.begin();
	while (iteMA1 != _madata1.end()){
		if (*iteMA1 >= *iteMA2){
			biggercount++;
			if (lesscount > 0)
				_UpCloseresult.push_back(lesscount);
			lesscount = 0;
		}
		else{
			lesscount++;
			if (biggercount > 0)
				_DownCloseresult.push_back(biggercount);
			biggercount = 0;
		}
		iteMA1++;
		iteMA2++;
	}
	//对不同的时长进行分组统计
	unsigned int groupsize = 2;
	map<unsigned int, unsigned int> _Upresult2;
	map<unsigned int, unsigned int> _Downresult2;
	_Upresult2.clear();
	_Downresult2.clear();
	for (size_t i = 0; i < 240; i++)
	{
		_Upresult2[i*groupsize] = 0;
		_Downresult2[i*groupsize] = 0;
	}
	GroupStatistice(_UpCloseresult, groupsize, _Upresult2);
	GroupStatistice(_DownCloseresult, groupsize, _Downresult2);
	return;
}


bool CStatisticeInter::StatisticeHistoryData(
	const StockDataTable& daynumber,
	const StockDataTable& weeknumber,
	const StockDataTable& mounthnumber,
	const StockDataTable& shnumber,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate,
	string Stockcode)
{
	EnterCriticalSection(&filterDataMeancs);
	if (!weeknumber.ChackDataSize()
		/*|| !weeknumber.ChackDataSize()*/
		/*|| !mounthnumber.ChackDataSize()*/)
	{
		LeaveCriticalSection(&filterDataMeancs);
		return false;
	}
	CFreqStatistice freqTool;
	_FreqList.clear();

	const StockDataTable& AnaNumber = daynumber;
	MeanVarPoint _tempMeanVarData;//用于暂存移动方差和移动均值
	CMeanVariance moveMeanVarTool;//用于全部计算方差和均值的工具
	MeanVar _lastMeanVar;//moveMeanVarTool迭代总移动方差和均值的返回变量
	MeanVar _allDataMeanVar;//moveMeanVarTool迭代总移动方差和均值的返回变量
	MeanVar _moveMeanVar;//moveMeanVarTool迭代计算移动方差和均值的返回变量
	daystate.moveMeanList.clear();//
	DayPrice oneDayPrice;//用于暂存日价格数据
	list <DayPrice> movePriceList;//平移方差计算时保存之前价格的价格
	_tempMeanVarData._TimeIndex = 0;
	_tempMeanVarData._Date = AnaNumber._vDate[0];
	daystate.moveMeanList.push_back(_tempMeanVarData);
	CStockAccount account("000001",100000000);
	//遍历所有的数据，计算方差和均值
	for (unsigned int indexCurrent = 6; indexCurrent < AnaNumber._vTimeDay.size(); indexCurrent++)
	{
		oneDayPrice._openData = AnaNumber._vOpen[indexCurrent];
		oneDayPrice._closeData = AnaNumber._vClose[indexCurrent];
		oneDayPrice._highData = AnaNumber._vHigh[indexCurrent];
		oneDayPrice._lowData = AnaNumber._vLow[indexCurrent];
		oneDayPrice._frontopen = AnaNumber._vOpen[indexCurrent - 1];
		oneDayPrice._frontclose = AnaNumber._vClose[indexCurrent - 1];
		oneDayPrice._fronthigh = AnaNumber._vHigh[indexCurrent - 1];
		oneDayPrice._frontlow = AnaNumber._vLow[indexCurrent - 1];
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _allDataMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _allDataMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _allDataMeanVar);
		unsigned int monthIndex = mounthnumber.GetLastTimeIndexByDate(AnaNumber._vDate[indexCurrent]);
		unsigned int shIndex = shnumber.GetLastTimeIndexByDate(AnaNumber._vDate[indexCurrent]);
	
		CheckCurrentStockDataAndGetName(
			AnaNumber,mounthnumber,_allDataMeanVar,
			indexCurrent,monthIndex,Stockcode);
		if (monthIndex == 0 || shIndex == 0)
			continue;
		if ( false
			|| (!(AnaNumber._vAsi_i[indexCurrent - 1] - AnaNumber._vAsi_i[indexCurrent - 2]> 0.0))
// 			|| (!(AnaNumber._vK[indexCurrent - 1] - AnaNumber._vK[indexCurrent - 2] > 0.00))
// 			|| (!(AnaNumber._vDiff[indexCurrent - 1] - AnaNumber._vDiff[indexCurrent - 2] > 0))
// 			|| (!(2 * AnaNumber._vDiff[indexCurrent - 2] - AnaNumber._vDiff[indexCurrent - 1] - AnaNumber._vDiff[indexCurrent - 3] < 0))
// 			|| (!(2 * AnaNumber._vK[indexCurrent - 2] - AnaNumber._vK[indexCurrent - 1] - AnaNumber._vK[indexCurrent - 3] < 0.00))
// 			|| (!(mounthnumber._vDiff[monthIndex] - mounthnumber._vDiff[monthIndex - 1] > 0))
// 			|| (!(_allDataMeanVar.mean > 0.00))
// 			|| (!(AnaNumber._vK[indexCurrent - 1] < 50))
// 			|| (AnaNumber._vCRMA1[indexCurrent - 1] - AnaNumber._vCRMA1[indexCurrent - 2] < 0)
// 			|| (!(shnumber._vK[shIndex] - shnumber._vK[shIndex - 1] > 0.0f))
// 			|| (!(shnumber._vDiff[shIndex] - shnumber._vDiff[shIndex - 1] > 0.0f))
//  			|| (!(2 * shnumber._vDiff[shIndex - 1] - shnumber._vDiff[shIndex] - shnumber._vDiff[shIndex - 2] < 0.0f))
// 			|| ((2 * shnumber._vK[shIndex - 1] - shnumber._vK[shIndex] - shnumber._vK[shIndex - 2] < 0.0f))
			)
		{
			//account.SellOutAll((oneDayPrice._highData + oneDayPrice._lowData + oneDayPrice._closeData) / 3, "1991-1-1");
			continue;
		}
		//account.AllIn((oneDayPrice._highData + oneDayPrice._lowData + oneDayPrice._closeData) / 3, "1991-1-1");
		//迭代计算全部数据的方差和均值
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _lastMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _lastMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _lastMeanVar);
		freqTool.PushFreqData(GetReturnRate_H(oneDayPrice), _FreqList);
		freqTool.PushFreqData(GetReturnRate_C(oneDayPrice), _FreqList);
		freqTool.PushFreqData(GetReturnRate_L(oneDayPrice), _FreqList);
		//计算移动方差和均值
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _moveMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _moveMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _moveMeanVar);
		movePriceList.push_back(oneDayPrice);
		if (indexCurrent >= moveVarDaySize)
		{
		//计算移动方差和均值，去除最开始的数据
			oneDayPrice = movePriceList.front();
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_H(oneDayPrice), _moveMeanVar);
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_C(oneDayPrice), _moveMeanVar);
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_L(oneDayPrice), _moveMeanVar);
			movePriceList.pop_front();
		}
		//将移动方差和均值保存到moveMeanVarList当中
		_tempMeanVarData._TimeIndex = indexCurrent;
		_tempMeanVarData._Date = AnaNumber._vDate[indexCurrent];
		_tempMeanVarData._Mean = _moveMeanVar.mean;
		_tempMeanVarData._Var = _moveMeanVar.var;
		_tempMeanVarData._MeanToVarRatio = _moveMeanVar.mean / _moveMeanVar.var;
		daystate.moveMeanList.push_back(_tempMeanVarData);
	}
	SaveMeanVarList_CS(Stockcode, _lastMeanVar, _allDataMeanVar);
	LeaveCriticalSection(&filterDataMeancs);
	return true;
}

bool CStatisticeInter::SaveMeanVarList_CS(
	const string& Stockcode,
	const MeanVar& _lastMeanVar,
	const MeanVar& _allDataMeanVar)
{
	EnterCriticalSection(&filterDataMeancs);
	CFreqStatistice freqTool;
	filterDataVarList[Stockcode] = _lastMeanVar.var;
	filterDataMeanList[Stockcode] = _lastMeanVar.mean;
	allDataVarList[Stockcode] = _allDataMeanVar.var;
	allDataMeanList[Stockcode] = _allDataMeanVar.mean;
	freqTool.SetEmptydate(_FreqList);
	LeaveCriticalSection(&filterDataMeancs);
	return true;
}

void CheckCurrentStockDataAndGetName(
	const StockDataTable& AnaNumber,
	const StockDataTable& mounthnumber,
	const MeanVar& allDataMeanVar,
	unsigned int Currentindex,
	unsigned int monthIndex,
	const string& StockCode)
{
	if (monthIndex != 0 && Currentindex >= AnaNumber._vTimeDay.size() - 1)
	{
		if ((AnaNumber._vDiff[Currentindex] - AnaNumber._vDiff[Currentindex - 1] < 0.00f))
			LOG(INFO) << StockCode << " Day diff failure to meet conditions.";
		else if ((2 * AnaNumber._vDiff[Currentindex - 1] - AnaNumber._vDiff[Currentindex] - AnaNumber._vDiff[Currentindex - 2] > 0))
			LOG(INFO) << StockCode << " Day diff failure to meet conditions.";
		else if ((AnaNumber._vK[Currentindex] - AnaNumber._vK[Currentindex - 1] < 0.00))
			LOG(INFO) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((2 * AnaNumber._vK[Currentindex - 1] - AnaNumber._vK[Currentindex] - AnaNumber._vK[Currentindex - 2] > 0.00f))
			LOG(INFO) << StockCode << " KDJ_K failure to meet conditions.";
		else if ((mounthnumber._vDiff[monthIndex] - mounthnumber._vDiff[monthIndex - 1] < 0))
			LOG(INFO) << StockCode << " Month diff failure to meet conditions.";
		else if ((allDataMeanVar.mean < 0.00f))
			LOG(INFO) << StockCode << " Day mean failure to meet conditions.";
		else
		{
			LOG(INFO) << " Get stock name:    " << StockCode;
			LOG(INFO) << " Data: " << AnaNumber._vTimeDay[Currentindex] << " inedx: " << Currentindex;
			LOG(INFO) << " MACD DIF: " << AnaNumber._vDiff[Currentindex] << " ," << AnaNumber._vDiff[Currentindex - 1];
		}
	}
}




CRealTimeAna::CRealTimeAna()
{
}

CRealTimeAna::~CRealTimeAna()
{
}


bool CRealTimeAna::AnalysisRealTimeData(const StockDataTable& number, string stockCode)
{
	unsigned lastIndex = number._vTimeDay.size() - 1;
	if (lastIndex < 10)
		return false;
	if (number._vK[lastIndex] > number._vK[lastIndex - 1]
		&& number._vK[lastIndex] + number._vK[lastIndex - 2] > 2 * number._vK[lastIndex-1]
		&& number._vJ[lastIndex - 1] < number._vK[lastIndex - 1]
		&& number._vJ[lastIndex] > number._vK[lastIndex]
		)
	{
		LOG(INFO) << "Realtime code KDJ up close:     " << stockCode;
	}
	if (number._vK[lastIndex] > number._vK[lastIndex - 1]
		&& 2 * number._vK[lastIndex - 1] - number._vK[lastIndex] - number._vK[lastIndex - 2] < 0
		&& number._vDiff[lastIndex] > number._vDiff[lastIndex - 1]
		&& 2 * number._vDiff[lastIndex - 1] - number._vDiff[lastIndex] - number._vDiff[lastIndex - 2] < 0
		)
	{
		LOG(INFO) << "Get Realtime stock code:     " << stockCode;
	}

	return true;
}
//
bool GroupFreqStatistice(const StockDataTable& stockdata, CStateInter& stateinter)
{
	CFreqStatistice tempStat;
	FreqListType vfreqlist;
	StatePointsList& thePoints = stateinter.allIndexStates[_eMACD_DEA]._trendstate;
	for (unsigned int i = 0; i < thePoints.size(); i++)
	{
		unsigned int timeindex = thePoints[i]._TimeIndex;
		if (thePoints[i]._IndexType == _eTrendBottomRise&&
			stockdata._vDEA[timeindex] > 0)
		{
			unsigned int beginindex = thePoints[i]._TimeIndex;
			StockDataTable tempdata = stockdata.NewDataByIndex(beginindex, beginindex + 30);
			tempStat.Inition();
			tempStat.GetGroupFrqu(tempdata._vPriChaRate, vfreqlist);
		}
	}
	return true;
}