#include "stdafx.h"
#include <algorithm>
#include <numeric>
#include <time.h>  
#include "CMACDProg1.h"
#include "number/IndexNumber.h"
#include "StockAccountNum.h"
#include "CommonFunction.h"

static const string closeIndexName = "close";
static const string dayIndexName = "";
static const string DiffIndexName = "Diff";
static const string DEAIndexName = "DEA";
static const string BARIndexName = "BAR";
static const string ChangeRateIndexName = "p_change";

CMACDProg1::CMACDProg1():
_DealAccount("000000", 1000)
{
	_LastError = "";
	_RelativeLimitData;
	_AbsoluteLimitData;
	_ValueParameter;
	_DayVariableList.clear();
	_DataFromFile.clear();
	_ValueParameter.maxDataSize = 4;
}

CMACDProg1::~CMACDProg1()
{
}

 
/*
//
*/
bool CMACDProg1::ToPretreatment(const OneDayVariable& dayVar)
{
	//ShowDebugInformationInCon("GetValue run.");
	//保存数据
	if (_DayVariableList.size() > _ValueParameter.maxDataSize)
	{
		_DayVariableList.pop_front();
		_DayVariableList.push_back(dayVar);
	}
	else
	{
		ShowDebugInformationInCon("push back data in _DayVariableList.");
		_DayVariableList.push_back(dayVar);
	}
	
	PIDLimits limitData;
	limitData.closeMax = GetRelativeValuePID(G_IndexClose);
	limitData.MACD_EDAMax = GetRelativeValuePID(G_IndexMACD_EDA);
	limitData.MACD_DiffMax = GetRelativeValuePID(G_IndexMACD_Diff);
	limitData.MACD_BARMax = GetRelativeValuePID(G_IndexMACD_BAR);
	limitData.ChangeRateMax = GetRelativeValuePID(G_IndexChangeRate);
	
	//G_IndexChangeRate
	limitData.closeMin = GetAbsoluteValuePID(G_IndexClose);
	limitData.MACD_EDAMin = GetAbsoluteValuePID(G_IndexMACD_EDA);
	limitData.MACD_DiffMin = GetAbsoluteValuePID(G_IndexMACD_Diff);
	limitData.MACD_BARMin = GetAbsoluteValuePID(G_IndexMACD_BAR);
	limitData.ChangeRateMin = GetAbsoluteValuePID(G_IndexChangeRate);

	//将数据保存在到文件中，调试用
	//SavePIDDataToFile(ddd);

	//设置，如果是比之前的最大值还大，则被设置为当前最大值
	if (!SaveMLimitParameter(limitData))
	{
		ShowDebugInformationInCon("Failed: SetMaxParameter error.");
		return false;
	}

	return true;
}

/*
//
*/
bool CMACDProg1::RunInterface()
{
	_DealDate = "000001";
	//
	//预处理文件夹下所有的CSV文件数据
	//
	//
	if (!PretreatmentFileData("d:\\stockfile\\stockdata\\" + _DealDate + ".csv"))
	{
		return false;
	}
	//所有系数置为1
	SetCoefDataToOne();
	double ProceedsRate = 0;
	vector<double> ProceedsRateLise;
	vector<CStockAccountNum> StockAccountlist;
	while (true)
	{
		ProceedsRateLise.clear();
		vector<PIDCoefficients> relativeCofList = CoefEvolution(_RelativeCoefData);
		vector<PIDCoefficients> complementCofList = CoefEvolution(_ComplementRelCoefData);
		if (relativeCofList.size() != complementCofList.size())
		{
			return false;
		}
		//遍历所有系数的收益值，找到收益最大的系数
		for (size_t i = 0; i < relativeCofList.size(); i++)
		{
			_RelativeCoefData = relativeCofList[i];
			_ComplementRelCoefData = complementCofList[i];
			if (!StudyStock(ProceedsRate))
			{
				return false;
			}
			ProceedsRateLise.push_back(ProceedsRate);
			StockAccountlist.push_back(_DealAccount);
		}
		ShowDebugInformationInCon("**********************************************************");
		//根据收益率最大值，找到最大收益率对应的系数
		Return_IF_FALSE(GetBiggestCoefData(ProceedsRateLise, relativeCofList, complementCofList, StockAccountlist))
		//用后一半的数据验证
		Return_IF_FALSE(VerificationCoefData(ProceedsRate))
		Return_IF_FALSE(ShowBestBusinessResult(ProceedsRate))
		StockAccountlist.clear();
	}

	return true;
}

/*
//预处理文件夹下所有的CSV文件数据,计算PID各个数据的值范围
*/
bool CMACDProg1::PretreatmentFileData(const string& filePath)
{
	CNumberManager indexTool;

	
	_DataFromFile = indexTool.ReadDataFromCSVFileAndCutRanks(filePath, ",");

	dayIterator = _DataFromFile.end();
	closeIterator = _DataFromFile.end();
	DiffIterator = _DataFromFile.end();
	DEAIterator = _DataFromFile.end();
	BARIterator = _DataFromFile.end();
	ChangeRateIterator = _DataFromFile.end();

	for (vector<vector<string>>::iterator ite = _DataFromFile.begin(); ite != _DataFromFile.end(); ite++)
	{
		if (*ite->begin() == closeIndexName)		closeIterator = ite;
		if (*ite->begin() == dayIndexName)			dayIterator = ite;
		if (*ite->begin() == DiffIndexName)			DiffIterator = ite;
		if (*ite->begin() == DEAIndexName)			DEAIterator = ite;
		if (*ite->begin() == BARIndexName)			BARIterator = ite;
		if (*ite->begin() == ChangeRateIndexName)		ChangeRateIterator = ite;
	}
	if (dayIterator == _DataFromFile.end()
		|| closeIterator == _DataFromFile.end()
		|| DiffIterator == _DataFromFile.end()
		|| DEAIterator == _DataFromFile.end()
		|| BARIterator == _DataFromFile.end()
		|| ChangeRateIterator == _DataFromFile.end())
	{
		ShowDebugInformationInCon("Value is not enough.");
		return false;
	}
	if (dayIterator->size() != closeIterator->size()
		|| dayIterator->size() != DiffIterator->size()
		|| dayIterator->size() != DEAIterator->size()
		|| dayIterator->size() != BARIterator->size())
	{
		ShowDebugInformationInCon("Value size is difference.");
		return false;
	}
	vector<string>& dayData = *closeIterator;
	vector<string>& closeData = *closeIterator;
	vector<string>& DiffData = *DiffIterator;
	vector<string>& DEAData = *DEAIterator;
	vector<string>& BARData = *BARIterator;
	vector<string>& ChangeRateData = *ChangeRateIterator;

	unsigned int minsize = 50;
	if (dayData.size() < minsize)
	{
		return false;
	}
	OneDayVariable dayValue;
	//vector<float> valuelist;
	//vector<>
	//前面的是最近的数据，所以从后面开始输入
	//第一个数据是标签名，所以不取 0 数据
	//只取后面一半的数据用来学习，前面一半的数据用检验
	for (size_t index = dayData.size() - 1; index > dayData.size()/2; index--)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		if (!ToPretreatment(dayValue))//一个一个装载数据
		{
			return false;
		}
	}

	ShowPID(_RelativeLimitData.closeMax, "closeRelMax");
	ShowPID(_RelativeLimitData.closeMin, "closeRelMin");
	ShowPID(_RelativeLimitData.MACD_BARMax,"MACD_BARRelMax" );
	ShowPID(_RelativeLimitData.MACD_BARMin, "MACD_BARRelMin");
	ShowPID(_RelativeLimitData.MACD_DiffMax, "MACD_DiffRelMax");
	ShowPID(_RelativeLimitData.MACD_DiffMin, "MACD_DiffRelMin");
	ShowPID(_RelativeLimitData.MACD_EDAMax, "MACD_EDARelMax");
	ShowPID(_RelativeLimitData.MACD_EDAMin, "MACD_EDARelMin");
	ShowPID(_RelativeLimitData.ChangeRateMax, "ChangeRateRelMax");
	ShowPID(_RelativeLimitData.ChangeRateMin, "ChangeRateRelMin");

	cout << endl << endl;

	ShowPID(_AbsoluteLimitData.closeMax, "closeAbsMax");
	ShowPID(_AbsoluteLimitData.closeMin, "closeAbsMin");
	ShowPID(_AbsoluteLimitData.MACD_BARMax, "MACD_BARAbsMax");
	ShowPID(_AbsoluteLimitData.MACD_BARMin, "MACD_BARAbsMin");
	ShowPID(_AbsoluteLimitData.MACD_DiffMax, "MACD_DiffAbsMax");
	ShowPID(_AbsoluteLimitData.MACD_DiffMin, "MACD_DiffAbsMin");
	ShowPID(_AbsoluteLimitData.MACD_EDAMax, "MACD_EDAAbsMax");
	ShowPID(_AbsoluteLimitData.MACD_EDAMin, "MACD_EDAAbsMin");
	ShowPID(_AbsoluteLimitData.ChangeRateMax, "ChangeRateAbsMax");
	ShowPID(_AbsoluteLimitData.ChangeRateMin, "ChangeRateAbsMin");

	return true;
}

/*
//
*/
bool CMACDProg1::SetParameter(ValueParameter mPatameter)
{
	_ValueParameter = mPatameter;
	//_LastError = "UnSupport.";
	return true;
}

/*
//
*/
std::string CMACDProg1::GetLastError()
{
	//string tempstr = PrintToString(1);
	//ShowDebugInformationInCon(tempstr);
	return _LastError;
}

/*
//
*/
PID CMACDProg1::GetRelativeValuePID(string index)
{
	PID reValue;
	//参数检查
	if (_DayVariableList.size() == 0)
	{
		return reValue;
	}
	//
	vector<float>  relativeData;
	list<OneDayVariable>::iterator ite = _DayVariableList.begin();
	OneDayVariable& beginValue = *ite;
	ite++;
	if (G_IndexClose == index)
	{
		while (ite != _DayVariableList.end())
		{
			relativeData.push_back(ite->closePrice - beginValue.closePrice);
			ite++;
		}
	}
	else if (G_IndexMACD_EDA == index)
	{
		while (ite != _DayVariableList.end())
		{
			relativeData.push_back(ite->fMACD_EDA - beginValue.fMACD_EDA);
			ite++;
		}
	}
	else if (G_IndexMACD_Diff == index)
	{
		while (ite != _DayVariableList.end())
		{
			relativeData.push_back(ite->fMACD_Diff - beginValue.fMACD_Diff);
			ite++;
		}
	}
	else if (G_IndexMACD_BAR == index)
	{
		while (ite != _DayVariableList.end())
		{
			relativeData.push_back(ite->fMACD_BAR - beginValue.fMACD_BAR);
			ite++;
		}
	}
	else if (G_IndexChangeRate == index)
	{
		while (ite != _DayVariableList.end())
		{
			relativeData.push_back(ite->changeRate - beginValue.changeRate);
			ite++;
		}
	}
	//relativeData保存的是与初值的
	if (relativeData.size() <= 1)
	{
		return reValue;
	}
	reValue.result_P = relativeData.back();
	for (vector<float>::iterator ite = relativeData.begin(); ite != relativeData.end(); ite++)
	{
		reValue.result_I += *ite;
	}
	reValue.result_D = relativeData[relativeData.size() - 1] - relativeData[relativeData.size() - 2];
	reValue.dataDescribe = index;

	return reValue;
}

/*
//
*/
//获取绝对值的PID_没有乘上系数
PID CMACDProg1::GetAbsoluteValuePID(string index)
{
	PID reValue;
	//参数检查
	if (_DayVariableList.size() == 0)
	{
		return reValue;
	}

	vector<float>  absoluteData;
	list<OneDayVariable>::iterator ite = _DayVariableList.begin();
	if (G_IndexClose == index)
	{
		while (ite != _DayVariableList.end())
		{
			absoluteData.push_back(ite->closePrice);
			ite++;
		}
	}
	else if (G_IndexMACD_EDA == index)
	{
		while (ite != _DayVariableList.end())
		{
			absoluteData.push_back(ite->fMACD_EDA);
			ite++;
		}
	}
	else if (G_IndexMACD_Diff == index)
	{
		while (ite != _DayVariableList.end())
		{
			absoluteData.push_back(ite->fMACD_Diff);
			ite++;
		}
	}
	else if (G_IndexMACD_BAR == index)
	{
		while (ite != _DayVariableList.end())
		{
			absoluteData.push_back(ite->fMACD_BAR);
			ite++;
		}
	}
	else if (G_IndexChangeRate == index)
	{
		while (ite != _DayVariableList.end())
		{
			absoluteData.push_back(ite->changeRate);
			ite++;
		}
	}
	//relativeData保存的是与初值的
	if (absoluteData.size() <= 1)
	{
		return reValue;
	}
	reValue.result_P = absoluteData.back();
	for (vector<float>::iterator ite = absoluteData.begin(); ite != absoluteData.end(); ite++)
	{
		reValue.result_I += *ite;
	}
	reValue.result_D = absoluteData[absoluteData.size() - 1] - absoluteData[absoluteData.size() - 2];

	return reValue;
}

bool CMACDProg1::SaveMLimitParameter(PIDLimits todayDaya)
{

	_RelativeLimitData.closeMax.SetMaxData(todayDaya.closeMax);
	_RelativeLimitData.closeMin.SetMinData(todayDaya.closeMax);
	_RelativeLimitData.ChangeRateMax.SetMaxData(todayDaya.ChangeRateMax);
	_RelativeLimitData.ChangeRateMin.SetMinData(todayDaya.ChangeRateMax);
	_RelativeLimitData.MACD_BARMax.SetMaxData(todayDaya.MACD_BARMax);
	_RelativeLimitData.MACD_BARMin.SetMinData(todayDaya.MACD_BARMax);
	_RelativeLimitData.MACD_DiffMax.SetMaxData(todayDaya.MACD_DiffMax);
	_RelativeLimitData.MACD_DiffMin.SetMinData(todayDaya.MACD_DiffMax);
	_RelativeLimitData.MACD_EDAMax.SetMaxData(todayDaya.MACD_EDAMax);
	_RelativeLimitData.MACD_EDAMin.SetMinData(todayDaya.MACD_EDAMax);

	_AbsoluteLimitData.closeMax.SetMaxData(todayDaya.closeMin);
	_AbsoluteLimitData.closeMin.SetMinData(todayDaya.closeMin);
	_AbsoluteLimitData.ChangeRateMax.SetMaxData(todayDaya.ChangeRateMin);
	_AbsoluteLimitData.ChangeRateMin.SetMinData(todayDaya.ChangeRateMin);
	_AbsoluteLimitData.MACD_BARMax.SetMaxData(todayDaya.MACD_BARMin);
	_AbsoluteLimitData.MACD_BARMin.SetMinData(todayDaya.MACD_BARMin);
	_AbsoluteLimitData.MACD_DiffMax.SetMaxData(todayDaya.MACD_DiffMin);
	_AbsoluteLimitData.MACD_DiffMin.SetMinData(todayDaya.MACD_DiffMin);
	_AbsoluteLimitData.MACD_EDAMax.SetMaxData(todayDaya.MACD_EDAMin);
	_AbsoluteLimitData.MACD_EDAMin.SetMinData(todayDaya.MACD_EDAMin);
	return true;
}

void CMACDProg1::ShowPID(const PID& showdata, string tiitleName)
{
	cout << tiitleName<<"\t";
	cout << "P:" << showdata.result_P << "\tI:" << showdata.result_I << "\tD:" << showdata.result_D << endl;

}

void CMACDProg1::SavePIDDataToFile(PIDParameters todayDaya)
{
	fstream outfile("d:\\stockfile\\data.csv", ios::out | ios::app);
	outfile << "," << todayDaya.closeRel.result_P << "," << todayDaya.closeRel.result_I << "," << todayDaya.closeRel.result_D;
	outfile << "," << todayDaya.closeAbs.result_P << "," << todayDaya.closeAbs.result_I << "," << todayDaya.closeAbs.result_D;

	outfile << "," << todayDaya.ChangeRateRel.result_P << "," << todayDaya.ChangeRateRel.result_I << "," << todayDaya.ChangeRateRel.result_D;
	outfile << "," << todayDaya.ChangeRateAbs.result_P << "," << todayDaya.ChangeRateAbs.result_I << "," << todayDaya.ChangeRateAbs.result_D;

	outfile << "," << todayDaya.MACD_BARRel.result_P << "," << todayDaya.MACD_BARRel.result_I << "," << todayDaya.MACD_BARRel.result_D;
	outfile << "," << todayDaya.MACD_BARAbs.result_P << "," << todayDaya.MACD_BARAbs.result_I << "," << todayDaya.MACD_BARAbs.result_D;

	outfile << "," << todayDaya.MACD_DiffRel.result_P << "," << todayDaya.MACD_DiffRel.result_I << "," << todayDaya.MACD_DiffRel.result_D;
	outfile << "," << todayDaya.MACD_DiffAbs.result_P << "," << todayDaya.MACD_DiffAbs.result_I << "," << todayDaya.MACD_DiffAbs.result_D;

	outfile << "," << todayDaya.MACD_EDARel.result_P << "," << todayDaya.MACD_EDARel.result_I << "," << todayDaya.MACD_EDARel.result_D;
	outfile << "," << todayDaya.MACD_EDAAbs.result_P << "," << todayDaya.MACD_EDAAbs.result_I << "," << todayDaya.MACD_EDAAbs.result_D;

	outfile << "," << todayDaya.ChangeRateRel.result_P << "," << todayDaya.ChangeRateRel.result_I << "," << todayDaya.ChangeRateRel.result_D;
	outfile << "," << todayDaya.ChangeRateAbs.result_P << "," << todayDaya.ChangeRateAbs.result_I << "," << todayDaya.ChangeRateAbs.result_D;
	outfile << endl;
	outfile.close();
}

DimensionString::iterator CMACDProg1::GetDataIter(DimensionString data)
{
	return data.end();
}


// 学习股票的数据
bool CMACDProg1::StudyStock(double& ProceedsRate)
{
	ProceedsRate = 0.0;
	if (_DataFromFile.empty())
	{
		ShowDebugInformationInCon("Error: Data from file is empty.");
		return false;
	}

	if (dayIterator == _DataFromFile.end()
		|| closeIterator == _DataFromFile.end()
		|| DiffIterator == _DataFromFile.end()
		|| DEAIterator == _DataFromFile.end()
		|| BARIterator == _DataFromFile.end()
		|| ChangeRateIterator == _DataFromFile.end())
	{
		ShowDebugInformationInCon("Value is not enough.");
		return false;
	}

	vector<string>& dayData = *dayIterator;
	vector<string>& closeData = *closeIterator;
	vector<string>& DiffData = *DiffIterator;
	vector<string>& DEAData = *DEAIterator;
	vector<string>& BARData = *BARIterator;
	vector<string>& ChangeRateData = *ChangeRateIterator;

	unsigned int minsize = 50;
	if (dayData.size() < minsize)
	{
		return false;
	}
	_DayVariableList.clear();
	OneDayVariable dayValue;
	//前面的是最近的数据，所以从后面开始输入
	//第一个数据是标签名，所以不取 0 数据
	//只取后面一半的数据用来学习，前面一半的数据用检验
	_DealAccount.InitAccount(1000);

	for (size_t index = 0 ; index < dayData.size() / 2; index++)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		dayValue._Date = dayData[index];
		if (!ToStudy(dayValue, _DealAccount))//一个一个装载数据
		{
			return false;
		}
	}
	ProceedsRate = _DealAccount.GetProceedsRate(dayValue.closePrice);
	return true;
}
bool CMACDProg1::VerificationCoefData(double& ProceedsRate)
{
	//参数检查
	ProceedsRate = 0.0;
	if (_DataFromFile.empty())
	{
		ShowDebugInformationInCon("Error: Data from file is empty.");
		return false;
	}

	if (dayIterator == _DataFromFile.end()
		|| closeIterator == _DataFromFile.end()
		|| DiffIterator == _DataFromFile.end()
		|| DEAIterator == _DataFromFile.end()
		|| BARIterator == _DataFromFile.end()
		|| ChangeRateIterator == _DataFromFile.end())
	{
		ShowDebugInformationInCon("Value is not enough.");
		return false;
	}

	vector<string>& dayData = *dayIterator;
	vector<string>& closeData = *closeIterator;
	vector<string>& DiffData = *DiffIterator;
	vector<string>& DEAData = *DEAIterator;
	vector<string>& BARData = *BARIterator;
	vector<string>& ChangeRateData = *ChangeRateIterator;

	unsigned int minsize = 50;
	if (dayData.size() < minsize)
	{
		return false;
	}
	_DayVariableList.clear();
	OneDayVariable dayValue;
	//前面的是最近的数据，所以从后面开始输入
	//第一个数据是标签名，所以不取 0 数据
	//只取后面一半的数据用来学习，前面一半的数据用检验
	_DealAccount.InitAccount(1000);
	//CStockAccountNum stockAccount(_DealDate, 1000);
	for (size_t index = closeData.size() / 2; index < closeData.size(); index++)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		dayValue._Date = dayData[index];
		if (!ToStudy(dayValue, _DealAccount))//一个一个装载数据
		{
			return false;
		}
	}

	ProceedsRate = _DealAccount.GetProceedsRate(dayValue.closePrice);
	string tempstring;
	TO_STRING(_DealAccount.GetCurrentAssets(dayValue.closePrice), tempstring);
	cout << " Current Assets is :" << tempstring;
	ShowDebugInformationInCon(" Current Assets is :" + tempstring);
	return true;
}


//进入分析学习股票
bool CMACDProg1::ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount)
{
	//ShowDebugInformationInCon("ToOperator run.");
	//保存数据
	if (_DayVariableList.size() > _ValueParameter.maxDataSize)
	{
		_DayVariableList.pop_front();
		_DayVariableList.push_back(dayValue);
	}
	else
	{
		ShowDebugInformationInCon("push back data in _DayVariableList.");
		_DayVariableList.push_back(dayValue);
	}

	//计算实时的PID值P比例；I积分；D微分
	PIDParameters realTimePIDPID;
	realTimePIDPID.closeRel = GetRelativeValuePID(G_IndexClose);
	realTimePIDPID.MACD_EDARel = GetRelativeValuePID(G_IndexMACD_EDA);
	realTimePIDPID.MACD_DiffRel = GetRelativeValuePID(G_IndexMACD_Diff);
	realTimePIDPID.MACD_BARRel = GetRelativeValuePID(G_IndexMACD_BAR);
	realTimePIDPID.ChangeRateRel = GetRelativeValuePID(G_IndexChangeRate);
	realTimePIDPID.closeAbs = GetAbsoluteValuePID(G_IndexClose);
	realTimePIDPID.MACD_EDAAbs = GetAbsoluteValuePID(G_IndexMACD_EDA);
	realTimePIDPID.MACD_DiffAbs = GetAbsoluteValuePID(G_IndexMACD_Diff);
	realTimePIDPID.MACD_BARAbs = GetAbsoluteValuePID(G_IndexMACD_BAR);
	realTimePIDPID.ChangeRateAbs = GetAbsoluteValuePID(G_IndexChangeRate);

	if (!GetProportionPosition(realTimePIDPID))
	{
		return false;
	}
	double PositionResult = AnalysisPosition(realTimePIDPID);

	//cout << "result = " << result << endl; //result是计算出来的仓位值
	


	string positionstr;
	TO_STRING(PositionResult, positionstr);
	ShowDebugInformationInCon("positionstr = " + positionstr);
	Business tempbusiness;
	tempbusiness._Price = dayValue.closePrice;
	tempbusiness._Date = dayValue._Date;
	if (!stockAccount.ChangeStockPosition(tempbusiness, PositionResult))
	{
		ShowDebugInformationInCon(stockAccount.GetLastError());
	}
	return true;
}

bool CMACDProg1::GetProportionPosition(PIDParameters& realTimePID)
{
	//
	GetPIDValueProportionPosition(_RelativeLimitData.closeMax, _RelativeLimitData.closeMin, realTimePID.closeRel);
	GetPIDValueProportionPosition(_RelativeLimitData.ChangeRateMax, _RelativeLimitData.ChangeRateMin, realTimePID.ChangeRateRel);
	GetPIDValueProportionPosition(_RelativeLimitData.MACD_EDAMax, _RelativeLimitData.MACD_EDAMin, realTimePID.MACD_EDARel);
	GetPIDValueProportionPosition(_RelativeLimitData.MACD_BARMax, _RelativeLimitData.MACD_BARMin, realTimePID.MACD_BARRel);
	GetPIDValueProportionPosition(_RelativeLimitData.MACD_DiffMax, _RelativeLimitData.MACD_DiffMin, realTimePID.MACD_DiffRel);
	//
	GetPIDValueProportionPosition(_AbsoluteLimitData.closeMax, _AbsoluteLimitData.closeMin, realTimePID.closeAbs);
	GetPIDValueProportionPosition(_AbsoluteLimitData.ChangeRateMax, _AbsoluteLimitData.ChangeRateMin, realTimePID.ChangeRateAbs);
	GetPIDValueProportionPosition(_AbsoluteLimitData.MACD_EDAMax, _AbsoluteLimitData.MACD_EDAMin, realTimePID.MACD_EDAAbs);
	GetPIDValueProportionPosition(_AbsoluteLimitData.MACD_BARMax, _AbsoluteLimitData.MACD_BARMin, realTimePID.MACD_BARAbs);
	GetPIDValueProportionPosition(_AbsoluteLimitData.MACD_DiffMax, _AbsoluteLimitData.MACD_DiffMin, realTimePID.MACD_DiffAbs);
	
	return true;
}

bool CMACDProg1::GetPIDValueProportionPosition(const PID& upLimit, const PID& downLimit, PID& index)
{
	if (upLimit.result_P - downLimit.result_P == 0
		|| upLimit.result_I - downLimit.result_I == 0
		|| upLimit.result_D - downLimit.result_D == 0)
	{
		_LastError = "Have a zero Denominator.";
		ShowDebugInformationInCon(_LastError);
		return false;
	}
	index.result_P = (index.result_P - downLimit.result_P) / (upLimit.result_P - downLimit.result_P);
	index.result_I = (index.result_I - downLimit.result_I) / (upLimit.result_I - downLimit.result_I);
	index.result_D = (index.result_D - downLimit.result_D) / (upLimit.result_D - downLimit.result_D);
	return true;
}
//分析得出股票的目标仓位
double CMACDProg1::AnalysisPosition(const PIDParameters& realTimePID)
{
	PIDParameters tempParameter;
	//原比例值乘上对应的相关系数
	tempParameter.ChangeRateAbs = realTimePID.ChangeRateAbs * _RelativeCoefData.ChangeRateAbs;
	tempParameter.closeAbs = realTimePID.closeAbs * _RelativeCoefData.closeAbs;
	tempParameter.MACD_BARAbs = realTimePID.MACD_BARAbs * _RelativeCoefData.MACD_BARAbs;
	tempParameter.MACD_DiffAbs = realTimePID.MACD_DiffAbs * _RelativeCoefData.MACD_DiffAbs;
	tempParameter.MACD_EDAAbs = realTimePID.MACD_EDAAbs * _RelativeCoefData.MACD_EDAAbs;

	tempParameter.ChangeRateRel = realTimePID.ChangeRateRel * _RelativeCoefData.ChangeRateRel;
	tempParameter.closeRel = realTimePID.closeRel * _RelativeCoefData.closeRel;
	tempParameter.MACD_BARRel = realTimePID.MACD_BARRel * _RelativeCoefData.MACD_BARRel;
	tempParameter.MACD_DiffRel = realTimePID.MACD_DiffRel * _RelativeCoefData.MACD_DiffRel;
	tempParameter.MACD_EDARel = realTimePID.MACD_EDARel * _RelativeCoefData.MACD_EDARel;


	double sunOfPID = tempParameter.GetSum();
	//比例的补数乘上相关系数
	tempParameter.ChangeRateAbs = GetComplement(realTimePID.ChangeRateAbs) * _ComplementRelCoefData.ChangeRateAbs;
	tempParameter.closeAbs = GetComplement(realTimePID.closeAbs) * _ComplementRelCoefData.closeAbs;
	tempParameter.MACD_BARAbs = GetComplement(realTimePID.MACD_BARAbs) * _ComplementRelCoefData.MACD_BARAbs;
	tempParameter.MACD_DiffAbs = GetComplement(realTimePID.MACD_DiffAbs) * _ComplementRelCoefData.MACD_DiffAbs;
	tempParameter.MACD_EDAAbs = GetComplement(realTimePID.MACD_EDAAbs) * _ComplementRelCoefData.MACD_EDAAbs;
	
	tempParameter.ChangeRateRel = GetComplement(realTimePID.ChangeRateRel) * _ComplementRelCoefData.ChangeRateRel;
	tempParameter.closeRel = GetComplement(realTimePID.closeRel) * _ComplementRelCoefData.closeRel;
	tempParameter.MACD_BARRel = GetComplement(realTimePID.MACD_BARRel) * _ComplementRelCoefData.MACD_BARRel;
	tempParameter.MACD_DiffRel = GetComplement(realTimePID.MACD_DiffRel) * _ComplementRelCoefData.MACD_DiffRel;
	tempParameter.MACD_EDARel = GetComplement(realTimePID.MACD_EDARel) * _ComplementRelCoefData.MACD_EDARel;

	sunOfPID += tempParameter.GetSum();
	if (_RelativeCoefData.GetSum() + _ComplementRelCoefData.GetSum() == 0)
	{
		cout << "Have a zero sum." << endl;
		ShowDebugInformationInCon("Have a zero sum.");
		return 0;
	}
	string strdata;
	TO_STRING(_RelativeCoefData.GetSum(), strdata);
	ShowDebugInformationInCon("RelativeCoefData_Sum=" + strdata);
// 	TO_STRING(_ComplementRelCoefData.GetSum(), strdata);
// 	ShowDebugInformationInCon("ComplementRelCoefData_Sum=" + strdata);
	double sumData = _RelativeCoefData.GetSum();// +_ComplementRelCoefData.GetSum();
	
	return sunOfPID / sumData;
}

void CMACDProg1::SetCoefDataToOne()
{
	PID tempPID;
	tempPID.result_D = 1;
	tempPID.result_I = 1;
	tempPID.result_P = 1;

	_RelativeCoefData.ChangeRateRel = tempPID;
	_RelativeCoefData.ChangeRateAbs = tempPID;
	_RelativeCoefData.closeRel = tempPID;
	_RelativeCoefData.closeAbs = tempPID;
	_RelativeCoefData.MACD_BARAbs = tempPID;
	_RelativeCoefData.MACD_BARRel = tempPID;
	_RelativeCoefData.MACD_DiffAbs = tempPID;
	_RelativeCoefData.MACD_DiffRel = tempPID;
	_RelativeCoefData.MACD_EDAAbs = tempPID;
	_RelativeCoefData.MACD_EDARel = tempPID;

	_ComplementRelCoefData.ChangeRateRel = tempPID;
	_ComplementRelCoefData.ChangeRateAbs = tempPID;
	_ComplementRelCoefData.closeRel = tempPID;
	_ComplementRelCoefData.closeAbs = tempPID;
	_ComplementRelCoefData.MACD_BARAbs = tempPID;
	_ComplementRelCoefData.MACD_BARRel = tempPID;
	_ComplementRelCoefData.MACD_DiffAbs = tempPID;
	_ComplementRelCoefData.MACD_DiffRel = tempPID;
	_ComplementRelCoefData.MACD_EDAAbs = tempPID;
	_ComplementRelCoefData.MACD_EDARel = tempPID;

	return;
}

PID CMACDProg1::GetComplement(const PID& index)
{
	PID tempPID;
	tempPID.SetTo(1.0);
	tempPID = tempPID - index;
	return tempPID;
}
/*
//系数进化，通过coefficientParameter为原型，随时进化出一批新的PIDCoefficient
*/
vector<PIDCoefficients> CMACDProg1::CoefEvolution(const PIDCoefficients& coefficientParameter)
{
	vector<PIDCoefficients> PIDCoefficientList;
	PIDCoefficients tempPIDCoefficient;
	//int data = rand();
	PIDCoefficientList.push_back(coefficientParameter);
	for (int i = 0; i < 10;i++)
	{
		srand((unsigned)time(NULL));

		tempPIDCoefficient.ChangeRateAbs.result_P = coefficientParameter.ChangeRateAbs.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.ChangeRateAbs.result_I = coefficientParameter.ChangeRateAbs.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.ChangeRateAbs.result_D = coefficientParameter.ChangeRateAbs.result_D + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.ChangeRateRel.result_P = coefficientParameter.ChangeRateRel.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.ChangeRateRel.result_I = coefficientParameter.ChangeRateRel.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.ChangeRateRel.result_D = coefficientParameter.ChangeRateRel.result_D + ((rand() % 100)-50)/30.0;

		tempPIDCoefficient.MACD_BARAbs.result_P = coefficientParameter.MACD_BARAbs.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_BARAbs.result_I = coefficientParameter.MACD_BARAbs.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_BARAbs.result_D = coefficientParameter.MACD_BARAbs.result_D + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_BARRel.result_P = coefficientParameter.MACD_BARRel.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_BARRel.result_I = coefficientParameter.MACD_BARRel.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_BARRel.result_D = coefficientParameter.MACD_BARRel.result_D + ((rand() % 100)-50)/30.0;

		tempPIDCoefficient.MACD_DiffAbs.result_P = coefficientParameter.MACD_DiffAbs.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_DiffAbs.result_I = coefficientParameter.MACD_DiffAbs.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_DiffAbs.result_D = coefficientParameter.MACD_DiffAbs.result_D + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_DiffRel.result_P = coefficientParameter.MACD_DiffRel.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_DiffRel.result_I = coefficientParameter.MACD_DiffRel.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_DiffRel.result_D = coefficientParameter.MACD_DiffRel.result_D + ((rand() % 100)-50)/30.0;

		tempPIDCoefficient.MACD_EDAAbs.result_P = coefficientParameter.MACD_EDAAbs.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_EDAAbs.result_I = coefficientParameter.MACD_EDAAbs.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_EDAAbs.result_D = coefficientParameter.MACD_EDAAbs.result_D + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_EDARel.result_P = coefficientParameter.MACD_EDARel.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_EDARel.result_I = coefficientParameter.MACD_EDARel.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.MACD_EDARel.result_D = coefficientParameter.MACD_EDARel.result_D + ((rand() % 100)-50)/30.0;

		tempPIDCoefficient.closeAbs.result_P = coefficientParameter.closeAbs.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.closeAbs.result_I = coefficientParameter.closeAbs.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.closeAbs.result_D = coefficientParameter.closeAbs.result_D + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.closeRel.result_P = coefficientParameter.closeRel.result_P + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.closeRel.result_I = coefficientParameter.closeRel.result_I + ((rand() % 100)-50)/30.0;
		tempPIDCoefficient.closeRel.result_D = coefficientParameter.closeRel.result_D + ((rand() % 100)-50)/30.0;

		Sleep(500);
		PIDCoefficientList.push_back(tempPIDCoefficient);
	}
	return PIDCoefficientList;
}

bool CMACDProg1::SavePIDCoefficientToFile(const PIDCoefficients& RelativeCoef, const PIDCoefficients& ComplementCoef)
{
	fstream outfile("d:\\stockfile\\PIDCoefficient.txt", ios::out);

	outfile << RelativeCoef.ChangeRateAbs.result_P << endl;
	outfile << RelativeCoef.ChangeRateAbs.result_I << endl;
	outfile << RelativeCoef.ChangeRateAbs.result_D << endl;
	outfile << RelativeCoef.ChangeRateRel.result_P << endl;
	outfile << RelativeCoef.ChangeRateRel.result_I << endl;
	outfile << RelativeCoef.ChangeRateRel.result_D << endl;

	outfile << RelativeCoef.MACD_BARAbs.result_P << endl;
	outfile << RelativeCoef.MACD_BARAbs.result_I << endl;
	outfile << RelativeCoef.MACD_BARAbs.result_D << endl;
	outfile << RelativeCoef.MACD_BARRel.result_P << endl;
	outfile << RelativeCoef.MACD_BARRel.result_I << endl;
	outfile << RelativeCoef.MACD_BARRel.result_D << endl;

	outfile << RelativeCoef.MACD_DiffAbs.result_P << endl;
	outfile << RelativeCoef.MACD_DiffAbs.result_I << endl;
	outfile << RelativeCoef.MACD_DiffAbs.result_D << endl;
	outfile << RelativeCoef.MACD_DiffRel.result_P << endl;
	outfile << RelativeCoef.MACD_DiffRel.result_I << endl;
	outfile << RelativeCoef.MACD_DiffRel.result_D << endl;

	outfile << RelativeCoef.MACD_EDAAbs.result_P << endl;
	outfile << RelativeCoef.MACD_EDAAbs.result_I << endl;
	outfile << RelativeCoef.MACD_EDAAbs.result_D << endl;
	outfile << RelativeCoef.MACD_EDARel.result_P << endl;
	outfile << RelativeCoef.MACD_EDARel.result_I << endl;
	outfile << RelativeCoef.MACD_EDARel.result_D << endl;

	outfile << RelativeCoef.closeAbs.result_P << endl;
	outfile << RelativeCoef.closeAbs.result_I << endl;
	outfile << RelativeCoef.closeAbs.result_D << endl;
	outfile << RelativeCoef.closeRel.result_P << endl;
	outfile << RelativeCoef.closeRel.result_I << endl;
	outfile << RelativeCoef.closeRel.result_D << endl;

	/******************************************************************/
	outfile << ComplementCoef.ChangeRateAbs.result_P << endl;
	outfile << ComplementCoef.ChangeRateAbs.result_I << endl;
	outfile << ComplementCoef.ChangeRateAbs.result_D << endl;
	outfile << ComplementCoef.ChangeRateRel.result_P << endl;
	outfile << ComplementCoef.ChangeRateRel.result_I << endl;
	outfile << ComplementCoef.ChangeRateRel.result_D << endl;

	outfile << ComplementCoef.MACD_BARAbs.result_P << endl;
	outfile << ComplementCoef.MACD_BARAbs.result_I << endl;
	outfile << ComplementCoef.MACD_BARAbs.result_D << endl;
	outfile << ComplementCoef.MACD_BARRel.result_P << endl;
	outfile << ComplementCoef.MACD_BARRel.result_I << endl;
	outfile << ComplementCoef.MACD_BARRel.result_D << endl;

	outfile << ComplementCoef.MACD_DiffAbs.result_P << endl;
	outfile << ComplementCoef.MACD_DiffAbs.result_I << endl;
	outfile << ComplementCoef.MACD_DiffAbs.result_D << endl;
	outfile << ComplementCoef.MACD_DiffRel.result_P << endl;
	outfile << ComplementCoef.MACD_DiffRel.result_I << endl;
	outfile << ComplementCoef.MACD_DiffRel.result_D << endl;

	outfile << ComplementCoef.MACD_EDAAbs.result_P << endl;
	outfile << ComplementCoef.MACD_EDAAbs.result_I << endl;
	outfile << ComplementCoef.MACD_EDAAbs.result_D << endl;
	outfile << ComplementCoef.MACD_EDARel.result_P << endl;
	outfile << ComplementCoef.MACD_EDARel.result_I << endl;
	outfile << ComplementCoef.MACD_EDARel.result_D << endl;

	outfile << ComplementCoef.closeAbs.result_P << endl;
	outfile << ComplementCoef.closeAbs.result_I << endl;
	outfile << ComplementCoef.closeAbs.result_D << endl;
	outfile << ComplementCoef.closeRel.result_P << endl;
	outfile << ComplementCoef.closeRel.result_I << endl;
	outfile << ComplementCoef.closeRel.result_D << endl;

	outfile.close();
	return true;
}

bool CMACDProg1::GetBiggestCoefData(
	vector<double> ProceedsRateLise,
	vector<PIDCoefficients> relativeCofList,
	vector<PIDCoefficients> complementCofList,
	vector<CStockAccountNum> StockAccountlist)
{
	int biggestposition = 0;
	vector<double>::iterator biggest = max_element(begin(ProceedsRateLise), end(ProceedsRateLise));
	biggestposition = distance(begin(ProceedsRateLise), biggest);
	_RelativeCoefData = relativeCofList[biggestposition];
	_ComplementRelCoefData = complementCofList[biggestposition];
	_DealAccount = StockAccountlist[biggestposition];
	SavePIDCoefficientToFile(_RelativeCoefData, _ComplementRelCoefData);
	return true;
}

bool CMACDProg1::ShowBestBusinessResult(double ProceedsRate)
{
	cout << "Proceeds Rate VerificationCoef:" << ProceedsRate << endl;
	cout << "Business Number: " << _DealAccount.GetAllBusiness().size() << endl;
	if (_DealAccount.GetAllBusiness().size() > 1)
	{
		cout << "Business Begin Day: " << _DealAccount.GetAllBusiness().begin()->_Date << endl;
		cout << "Business End Day: " << _DealAccount.GetAllBusiness().rbegin()->_Date << endl;
		_DealAccount.PrintAllBusiness("d:\\stockfile\\allbusiness.csv");
	}
	cout << "**************************************" << endl;
	return true;
}




double PIDParameters::GetSum()
{
	return closeRel.GetSum() +
		closeAbs.GetSum() +
		ChangeRateRel.GetSum() +
		ChangeRateAbs.GetSum() +
		MACD_EDARel.GetSum() +
		MACD_EDAAbs.GetSum() +
		MACD_DiffRel.GetSum() +
		MACD_DiffAbs.GetSum() +
		MACD_BARRel.GetSum() +
		MACD_BARAbs.GetSum();
}

double PIDCoefficients::GetSum()
{

	return closeRel.GetSum() +
		closeAbs.GetSum() +
		ChangeRateRel.GetSum() +
		ChangeRateAbs.GetSum() +
		MACD_EDARel.GetSum() +
		MACD_EDAAbs.GetSum() +
		MACD_DiffRel.GetSum() +
		MACD_DiffAbs.GetSum() +
		MACD_BARRel.GetSum() +
		MACD_BARAbs.GetSum();
}
