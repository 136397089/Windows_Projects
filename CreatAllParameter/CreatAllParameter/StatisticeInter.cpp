#include "stdafx.h"
#include <numeric>
#include <complex>//����
#include <cmath>
#include "Number/NumberInter.h"
#include "StatisticeInter.h"
#include "DFT.h"//���ٸ���Ҷ
#include "glog/logging.h"//Log����
#include "CcdpStatistics.h"
#include "StockAccountNum.h"
CStatisticeInter::CStatisticeInter():
moveVarDaySize(100)
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
	//������
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
		//ͳ���������Ǻ��µ��Ĵ�������
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

//����ģ��CDP�ĸߵ͵���������Ʊ
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
//�Ծ���_madata1�ھ���_madata2֮�Ϻ�֮�µ�ʱ������ͳ��
//���صĽ������_UpCloseresult��_DownCloseresult����
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
	//�Բ�ͬ��ʱ�����з���ͳ��
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


bool CStatisticeInter::StatisticeDebugFunstion(
	const StockDataTable& daynumber,
	const StockDataTable& weeknumber,
	const StockDataTable& mounthnumber,
	CStateInter& daystate,
	CStateInter& weekstate,
	CStateInter& monthstate,
	string Stockcode)
{
	if (!daynumber.ChackDataSize()
		/*|| !weeknumber.ChackDataSize()*/
		/*|| !mounthnumber.ChackDataSize()*/)
	{
		return false;
	}
	MeanVarPoint _tempMeanVarData;//�����ݴ��ƶ�������ƶ���ֵ
	CMeanVariance moveMeanVarTool;//����ȫ�����㷽��;�ֵ�Ĺ���
	MeanVar _moveMeanVar;//moveMeanVarTool���������ƶ�����;�ֵ�ķ��ر���
	MeanVar _lastMeanVar;//moveMeanVarTool�������ƶ�����;�ֵ�ķ��ر���
	MeanVar _allDataMeanVar;//moveMeanVarTool�������ƶ�����;�ֵ�ķ��ر���
	daystate.moveMeanList.clear();//
	DayPrice oneDayPrice;//�����ݴ��ռ۸�����
	list <DayPrice> movePriceList;//ƽ�Ʒ������ʱ����֮ǰ�۸�ļ۸�
	_tempMeanVarData._TimeIndex = 0;
	_tempMeanVarData._Date = daynumber._vDate[0];
	daystate.moveMeanList.push_back(_tempMeanVarData);
	//�������е����ݣ����㷽��;�ֵ
	for (unsigned int i = 50; i < daynumber._vTimeDay.size();i++)
	{
		oneDayPrice._openData = daynumber._vOpen[i];
		oneDayPrice._closeData = daynumber._vClose[i];
		oneDayPrice._highData = daynumber._vHigh[i];
		oneDayPrice._lowData = daynumber._vLow[i];
		oneDayPrice._frontopen = daynumber._vOpen[i-1];
		oneDayPrice._frontclose = daynumber._vClose[i-1];
		oneDayPrice._fronthigh = daynumber._vHigh[i-1];
		oneDayPrice._frontlow = daynumber._vLow[i-1];
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _allDataMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _allDataMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _allDataMeanVar);
		unsigned int monthIndex = mounthnumber.GetLastTimeIndexByDate(daynumber._vDate[i]);
		if (  mounthnumber._vDiff[monthIndex] - mounthnumber._vDiff[monthIndex - 1] < 0
			||daynumber._vDiff[i-1] - daynumber._vDiff[i - 2] < 0
			|| daynumber._vD[i - 1] - daynumber._vD[i - 2] < 0
			|| daynumber._vD[i - 1]>80
			)
		{
			continue;
		}
		movePriceList.push_back(oneDayPrice);
		//��������ȫ�����ݵķ���;�ֵ
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _lastMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _lastMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _lastMeanVar);
		//�����ƶ�����;�ֵ
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_H(oneDayPrice), _moveMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_C(oneDayPrice), _moveMeanVar);
		moveMeanVarTool.GetNextMeanVar(GetReturnRate_L(oneDayPrice), _moveMeanVar);
		if (i >= moveVarDaySize)
		{
		//�����ƶ�����;�ֵ��ȥ���ʼ������
			oneDayPrice = movePriceList.front();
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_H(oneDayPrice), _moveMeanVar);
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_C(oneDayPrice), _moveMeanVar);
			moveMeanVarTool.GetMeanVarRemoveData(GetReturnRate_L(oneDayPrice), _moveMeanVar);
			movePriceList.pop_front();
		}
		//���ƶ�����;�ֵ���浽moveMeanVarList����
		_tempMeanVarData._TimeIndex = i;
		_tempMeanVarData._Date = daynumber._vDate[i];
		_tempMeanVarData._Mean = _moveMeanVar.mean;
		_tempMeanVarData._Var = _moveMeanVar.var;
		_tempMeanVarData._MeanToVarRatio = _moveMeanVar.mean / _moveMeanVar.var;
		daystate.moveMeanList.push_back(_tempMeanVarData);
	}
	allDataVarList[Stockcode] = _allDataMeanVar.var;
	allDataMeanList[Stockcode] = _allDataMeanVar.mean;
	filterDataVarList[Stockcode] = _lastMeanVar.var;
	filterDataMeanList[Stockcode] = _lastMeanVar.mean;
	return true;
}





