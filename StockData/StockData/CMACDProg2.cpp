#include "stdafx.h"
#include <algorithm>
#include <numeric>
#include <time.h>  
#include "CMACDProg2.h"
#include "number/IndexNumber.h"
#include "StockAccountNum.h"
#include "CommonFunction.h"
#include "PosiCal.h"


static const string closeIndexName = "close";
static const string dayIndexName = "";
static const string DiffIndexName = "Diff";
static const string DEAIndexName = "DEA";
static const string BARIndexName = "BAR";
static const string ChangeRateIndexName = "p_change";
static const int MaxEvolutionSize = 10;//每次进化出来的个体数

CMACDProg2::CMACDProg2():
_BestDealAccount("000000", 1000)
{
	_LastError = "";
	_DayVariableList.clear();
	_DataFromFile.clear();
	_ValueParameter.maxDataSize = 10;
}

CMACDProg2::~CMACDProg2()
{
}


/*
//
*/
bool CMACDProg2::RunInterface()
{
	_InitCapital = 1000;

	//所有系数置为1
	SetCoefDataToOne();
	double ProceedsRate = 0;
	vector<double> ProceedsRateLise;
	vector<CStockAccountNum> StockAccountlist;
	while (true)
	{
		//
		//预处理文件夹下所有的CSV文件数据
		//
		if (!PretreatmentFileData("d:\\stockfile\\stockdata\\000001.csv"))
		{
			return false;
		}
		//系数进化
		vector<Coefficients> relativeCofList = CoefEvolution(_RelativeCoefData);
		ProceedsRateLise.clear();
		StockAccountlist.clear();
		//遍历所有系数的收益值，找到收益最大的系数
		for (size_t i = 0; i < relativeCofList.size(); i++)
		{
			_RelativeCoefData = relativeCofList[i];
			CStockAccountNum dealAccount(_DealDate, _InitCapital);
			if (!GetProceedsRate(ProceedsRate, dealAccount))
			{
				ShowDebugInformationInCon(_LastError);
				return false;
			}
			ProceedsRateLise.push_back(ProceedsRate);
			StockAccountlist.push_back(dealAccount);
		}
		ShowCutLine();
		//根据收益率最大值，找到最大收益率对应的系数
		Return_IF_FALSE(GetBiggestCoefData(ProceedsRateLise, relativeCofList, StockAccountlist))
		Return_IF_FALSE(ShowBestStudyBusinessResult())
		//用后一半的数据验证
		if (!PretreatmentFileData("d:\\stockfile\\stockdata\\601988.csv"))
		{
			return false;
		}
		Return_IF_FALSE(VerificationCoefData(ProceedsRate))
	}

	return true;
}

/*
//预处理文件夹下所有的CSV文件数据，将数据读入
*/
bool CMACDProg2::PretreatmentFileData(const string& filePath)
{
	CNumberManager indexTool;

	_DataFromFile = indexTool.ReadDataFromCSVFileAndCutRanks(filePath, ",");

	dayIterator = _DataFromFile.end();
	closeIterator = _DataFromFile.end();
	DiffIterator = _DataFromFile.end();
	DEAIterator = _DataFromFile.end();
	BARIterator = _DataFromFile.end();
	ChangeRateIterator = _DataFromFile.end();

	//
	for (vector<vector<string>>::iterator ite = _DataFromFile.begin(); ite != _DataFromFile.end(); ite++)
	{
		if (*ite->begin() == closeIndexName)		closeIterator = ite;
		if (*ite->begin() == dayIndexName)			dayIterator = ite;
		if (*ite->begin() == DiffIndexName)			DiffIterator = ite;
		if (*ite->begin() == DEAIndexName)			DEAIterator = ite;
		if (*ite->begin() == BARIndexName)			BARIterator = ite;
		if (*ite->begin() == ChangeRateIndexName)	ChangeRateIterator = ite;
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
	return true;
}

/*
//
*/
std::string CMACDProg2::GetLastError()
{
	return _LastError;
}


void CMACDProg2::SavePIDDataToFile(ParameterDatas todayDaya)
{
	fstream outfile("d:\\stockfile\\data.csv", ios::out | ios::app);
	for (int i = 0; i < todayDaya.size();i++)
	{
		outfile << "," << todayDaya[i];
	}
	outfile << endl;
	outfile.close();
}


// 获取当前被设定的系数对应的收益率
bool CMACDProg2::GetProceedsRate(double& proceedsRate, CStockAccountNum& account)
{
	//1.入口参数检查
	proceedsRate = 0.0;
	if (_DataFromFile.empty())
	{
		_LastError = "Error: Data from file is empty.";
		ShowDebugInformationInCon(_LastError);
		return false;
	}
	if (dayIterator == _DataFromFile.end()
		|| closeIterator == _DataFromFile.end()
		|| DiffIterator == _DataFromFile.end()
		|| DEAIterator == _DataFromFile.end()
		|| BARIterator == _DataFromFile.end()
		|| ChangeRateIterator == _DataFromFile.end())
	{
		_LastError = "Value is not enough.";
		ShowDebugInformationInCon(_LastError);
		return false;
	}

	vector<string>& dayData = *dayIterator;
	vector<string>& closeData = *closeIterator;
	vector<string>& DiffData = *DiffIterator;
	vector<string>& DEAData = *DEAIterator;
	vector<string>& BARData = *BARIterator;
	vector<string>& ChangeRateData = *ChangeRateIterator;
	//end:入口参数检查

	//2.
	unsigned int minsize = 50;
	if (dayData.size() < minsize)
	{
		_LastError = "dayData is too small.";
		return false;
	}
	_DayVariableList.clear();
	OneDayVariable dayValue;
	//前面的是最近的数据，所以从后面开始输入
	//第一个数据是标签名，所以不取 0 数据
	account.InitAccount(1000);

	//
	//只取前面一半的数据用来学习，后面一半的数据用检验
	for (size_t index = 100 ; index < dayData.size(); index++)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		dayValue._Date = dayData[index];
		if (!ToStudy(dayValue, account))//一个一个装载数据
		{
			ShowDebugInformationInCon(_LastError);
			_LastError = "ToStudy error.";
			return false;
		}
	}
	_StudyClose = dayValue.closePrice;
	proceedsRate = account.GetProceedsRate(dayValue.closePrice);
	return true;
}

bool CMACDProg2::VerificationCoefData(double& ProceedsRate)
{
	//1.参数检查
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

	//2.
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

	CStockAccountNum _DealAccount(_DealDate, _InitCapital);
	_RelativeCoefData = _BestRelativeCoefData;
	string tempstring;

	for (size_t i = 0; i < _RelativeCoefData.size(); i++)
	{
		TO_STRING(_RelativeCoefData[i], tempstring);
		ShowDebugInformationInCon("验证参数:" + tempstring);
	}
	ShowDebugInformationInCon("开始验证数据。");
	for (size_t index = closeData.size()-100; index < closeData.size(); index++)
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

	TO_STRING(_DealAccount.GetCurrentAssets(dayValue.closePrice), tempstring);
	ShowDebugInformationInCon("验证集最终资产:" + tempstring);
	TO_STRING(ProceedsRate, tempstring);
	ShowDebugInformationInCon("验证集最终收效率:" + tempstring);
	TO_STRING(_DealAccount.GetAllBusiness().size(), tempstring);
	ShowDebugInformationInCon("验证集交易次数:" + tempstring);
// 	ShowDebugInformationInCon("验证集交易次数:" + _DealAccount.GetAllBusiness().begin()->_Date);
// 	TO_STRING(_DealAccount.GetAllBusiness().size, tempstring);
// 	ShowDebugInformationInCon("验证集交易次数:" + tempstring);

	return true;
}
//进入分析学习股票
bool CMACDProg2::ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount)
{
	//1.保存数据,只处理最近maxDataSize个数据，之前的数据去除
	if (_DayVariableList.size() > _ValueParameter.maxDataSize)
	{
		_DayVariableList.pop_front();
		_DayVariableList.push_back(dayValue);
	}
	else
	{
		_DayVariableList.push_back(dayValue);
	}

	//2.计算仓位
	CPosiAnalyzerHandle tempMethod(1);
	double PositionResult = tempMethod.AnalysisPosition(_DayVariableList, _RelativeCoefData);
 
	Business tempbusiness;
	tempbusiness._Price = dayValue.closePrice;
	tempbusiness._Date = dayValue._Date;
	//3.修改账号的仓位
	if (!stockAccount.ChangeStockPosition(tempbusiness, PositionResult))
	{
		ShowDebugInformationInCon(stockAccount.GetLastError());
		return false;
	}
	return true;
}



//
//分析得出股票的目标仓位
//
double CMACDProg2::AnalysisPosition(const list<OneDayVariable>& DayVariableList)
{
	string strdata;
	TO_STRING(GetSum(_RelativeCoefData), strdata);
	ShowDebugInformationInCon("RelativeCoefData_Sum=" + strdata);
// 	TO_STRING(_ComplementRelCoefData.GetSum(), strdata);
// 	ShowDebugInformationInCon("ComplementRelCoefData_Sum=" + strdata);
	double position = GetSum(_RelativeCoefData);

	return position;
}


/*
//将所有的系数都设为1
*/
void CMACDProg2::SetCoefDataToOne()
{
	_RelativeCoefData.clear();
	CPosiAnalyzerHandle temphandle(1);
	for (int i = 0; i < temphandle.GetCofListSize(); i++)
	{
		srand((unsigned)time(NULL));
		float frand = (float)(rand() % 50);
		_RelativeCoefData.push_back((frand) / 50.0);
		Sleep(500);
	}

	return;
}


/*
//系数进化，通过coefficientParameter为原型，随时进化出一批新的Coefficient
*/
vector<Coefficients> CMACDProg2::CoefEvolution(const Coefficients& coefficientParameter)
{
	vector<Coefficients> PIDCoefficientList;
	Coefficients tempPIDCoefficient;

	PIDCoefficientList.push_back(coefficientParameter);
	for (int i = 0; i < MaxEvolutionSize; i++)
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < coefficientParameter.size();i++)
		{
			float frand = (float)(rand() % 100);
			float tempdata = coefficientParameter[i] + (frand - 50.0) / 500.0;
			if (tempdata > 1.0)
			{
				tempdata = 1.0;
			}
			if (tempdata < 0)
			{
				tempdata = 0.0;
			}
			tempPIDCoefficient.push_back(tempdata);
		}
		Sleep(500);
		PIDCoefficientList.push_back(tempPIDCoefficient);
		tempPIDCoefficient.clear();
	}
	return PIDCoefficientList;
}


/*
//将系数保存
*/
bool CMACDProg2::SavePIDCoefficientToFile(const Coefficients& RelativeCoef, const Coefficients& ComplementCoef)
{
	fstream outfile("d:\\stockfile\\PIDCoefficient.txt", ios::out);

	for (Coefficients::const_iterator ite = RelativeCoef.begin(); ite != RelativeCoef.end();ite++)
	{
		outfile << *ite << endl;
	}
	for (Coefficients::const_iterator ite = ComplementCoef.begin(); ite != ComplementCoef.end(); ite++)
	{
		outfile << *ite << endl;
	}

	outfile.close();
	return true;
}


/*
//在所有的参数中找出收益最好的那个参数
*/

bool CMACDProg2::GetBiggestCoefData(
	vector<double> ProceedsRateLise,
	vector<Coefficients> relativeCofList,
	vector<CStockAccountNum> StockAccountlist)
{
	int biggestposition = 0;
	vector<double>::iterator biggest = max_element(begin(ProceedsRateLise), end(ProceedsRateLise));

	biggestposition = distance(begin(ProceedsRateLise), biggest);
	_BestRelativeCoefData = relativeCofList[biggestposition];
	_BestDealAccount = StockAccountlist[biggestposition];
	//SavePIDCoefficientToFile(_BestRelativeCoefData, _BestComplementRelCoefData);
// 	string strBiggestRate = "" ;
// 	TO_STRING(*biggest, strBiggestRate);
// 	ShowDebugInformationInCon("学习数据的最大收益率为："+strBiggestRate);
	return true;
}

bool CMACDProg2::ShowBestStudyBusinessResult()
{
	float lastPrice = 0.0;
	if (closeIterator != _DataFromFile.end())
	{
		lastPrice = _StudyClose;
	}
	else
	{
		return false;
	}
	string tempstring = "";
	for (int i = 0; i < _BestRelativeCoefData.size();i++)
	{
		TO_STRING(_BestRelativeCoefData[i], tempstring);
		ShowDebugInformationInCon("最佳参数为：" + tempstring);
	}
	TO_STRING(_BestDealAccount.GetProceedsRate(lastPrice), tempstring);
	ShowDebugInformationInCon("学习集最大收益率为:" + tempstring);
	TO_STRING(_BestDealAccount.GetAllBusiness().size(), tempstring);
	ShowDebugInformationInCon("学习集交易次数:" + tempstring);
	if (_BestDealAccount.GetAllBusiness().size() > 1)
	{
		ShowDebugInformationInCon("学习集第一次交易日期:" + _BestDealAccount.GetAllBusiness().begin()->_Date);
		ShowDebugInformationInCon("学习集最后一个交易日期:" + _BestDealAccount.GetAllBusiness().rbegin()->_Date);
		_BestDealAccount.PrintAllBusiness("d:\\stockfile\\allbusiness.csv");
	}
	return true;
}





