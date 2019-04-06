#include "stdafx.h"
#include <numeric>
#include <complex>//复数
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "DFT.h"//快速傅立叶
#include "glog/logging.h"//Log工具
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
CStatisticeInter::CStatisticeInter()
{
}


CStatisticeInter::~CStatisticeInter()
{
}
//
bool CStatisticeInter::Inition()
{
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


//
bool CStatisticeInter::GroupFreqStatistice(const StockDataTable& stockdata,	CStateInter& stateinter)
{
	CFreqStatistice tempStat;
	changeRateStatis vfreqlist;
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
			tempStat.GetGroupFrqu(tempdata._vPriChaRate, 0.004f, vfreqlist);
		}
	}
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
	changeRateStatis vfreqlist;

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
		freqStatTool.GetGroupFrqu(newdata._vPriChaRate, 0.01f, vfreqlist);
		float rise = freqStatTool.GetFreqByValue(0.00001f, 1.0f);
		float fall = freqStatTool.GetFreqByValue(-1.0f, -0.00001f);
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

tyStockData CStatisticeInter::Autocorrelation(const VStockData& _inputdata, unsigned int interval)
{
	if (interval >= _inputdata.size())
	{
		return 0;
	}
	tyStockData Average = accumulate(_inputdata.begin(), _inputdata.end(), 0) / _inputdata.size();

	tyStockData Var = 0;
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





