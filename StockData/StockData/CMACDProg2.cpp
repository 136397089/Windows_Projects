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
static const int MaxEvolutionSize = 10;//ÿ�ν��������ĸ�����

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

	//����ϵ����Ϊ1
	SetCoefDataToOne();
	double ProceedsRate = 0;
	vector<double> ProceedsRateLise;
	vector<CStockAccountNum> StockAccountlist;
	while (true)
	{
		//
		//Ԥ�����ļ��������е�CSV�ļ�����
		//
		if (!PretreatmentFileData("d:\\stockfile\\stockdata\\000001.csv"))
		{
			return false;
		}
		//ϵ������
		vector<Coefficients> relativeCofList = CoefEvolution(_RelativeCoefData);
		ProceedsRateLise.clear();
		StockAccountlist.clear();
		//��������ϵ��������ֵ���ҵ���������ϵ��
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
		//�������������ֵ���ҵ���������ʶ�Ӧ��ϵ��
		Return_IF_FALSE(GetBiggestCoefData(ProceedsRateLise, relativeCofList, StockAccountlist))
		Return_IF_FALSE(ShowBestStudyBusinessResult())
		//�ú�һ���������֤
		if (!PretreatmentFileData("d:\\stockfile\\stockdata\\601988.csv"))
		{
			return false;
		}
		Return_IF_FALSE(VerificationCoefData(ProceedsRate))
	}

	return true;
}

/*
//Ԥ�����ļ��������е�CSV�ļ����ݣ������ݶ���
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


// ��ȡ��ǰ���趨��ϵ����Ӧ��������
bool CMACDProg2::GetProceedsRate(double& proceedsRate, CStockAccountNum& account)
{
	//1.��ڲ������
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
	//end:��ڲ������

	//2.
	unsigned int minsize = 50;
	if (dayData.size() < minsize)
	{
		_LastError = "dayData is too small.";
		return false;
	}
	_DayVariableList.clear();
	OneDayVariable dayValue;
	//ǰ�������������ݣ����ԴӺ��濪ʼ����
	//��һ�������Ǳ�ǩ�������Բ�ȡ 0 ����
	account.InitAccount(1000);

	//
	//ֻȡǰ��һ�����������ѧϰ������һ��������ü���
	for (size_t index = 100 ; index < dayData.size(); index++)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		dayValue._Date = dayData[index];
		if (!ToStudy(dayValue, account))//һ��һ��װ������
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
	//1.�������
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
	//ǰ�������������ݣ����ԴӺ��濪ʼ����
	//��һ�������Ǳ�ǩ�������Բ�ȡ 0 ����
	//ֻȡ����һ�����������ѧϰ��ǰ��һ��������ü���

	CStockAccountNum _DealAccount(_DealDate, _InitCapital);
	_RelativeCoefData = _BestRelativeCoefData;
	string tempstring;

	for (size_t i = 0; i < _RelativeCoefData.size(); i++)
	{
		TO_STRING(_RelativeCoefData[i], tempstring);
		ShowDebugInformationInCon("��֤����:" + tempstring);
	}
	ShowDebugInformationInCon("��ʼ��֤���ݡ�");
	for (size_t index = closeData.size()-100; index < closeData.size(); index++)
	{
		dayValue.closePrice = atof(closeData[index].c_str());
		dayValue.fMACD_Diff = atof(DiffData[index].c_str());
		dayValue.fMACD_EDA = atof(DEAData[index].c_str());
		dayValue.fMACD_BAR = atof(BARData[index].c_str());
		dayValue.changeRate = atof(ChangeRateData[index].c_str());
		dayValue._Date = dayData[index];
		if (!ToStudy(dayValue, _DealAccount))//һ��һ��װ������
		{
			return false;
		}
	}
	ProceedsRate = _DealAccount.GetProceedsRate(dayValue.closePrice);

	TO_STRING(_DealAccount.GetCurrentAssets(dayValue.closePrice), tempstring);
	ShowDebugInformationInCon("��֤�������ʲ�:" + tempstring);
	TO_STRING(ProceedsRate, tempstring);
	ShowDebugInformationInCon("��֤��������Ч��:" + tempstring);
	TO_STRING(_DealAccount.GetAllBusiness().size(), tempstring);
	ShowDebugInformationInCon("��֤�����״���:" + tempstring);
// 	ShowDebugInformationInCon("��֤�����״���:" + _DealAccount.GetAllBusiness().begin()->_Date);
// 	TO_STRING(_DealAccount.GetAllBusiness().size, tempstring);
// 	ShowDebugInformationInCon("��֤�����״���:" + tempstring);

	return true;
}
//�������ѧϰ��Ʊ
bool CMACDProg2::ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount)
{
	//1.��������,ֻ�������maxDataSize�����ݣ�֮ǰ������ȥ��
	if (_DayVariableList.size() > _ValueParameter.maxDataSize)
	{
		_DayVariableList.pop_front();
		_DayVariableList.push_back(dayValue);
	}
	else
	{
		_DayVariableList.push_back(dayValue);
	}

	//2.�����λ
	CPosiAnalyzerHandle tempMethod(1);
	double PositionResult = tempMethod.AnalysisPosition(_DayVariableList, _RelativeCoefData);
 
	Business tempbusiness;
	tempbusiness._Price = dayValue.closePrice;
	tempbusiness._Date = dayValue._Date;
	//3.�޸��˺ŵĲ�λ
	if (!stockAccount.ChangeStockPosition(tempbusiness, PositionResult))
	{
		ShowDebugInformationInCon(stockAccount.GetLastError());
		return false;
	}
	return true;
}



//
//�����ó���Ʊ��Ŀ���λ
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
//�����е�ϵ������Ϊ1
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
//ϵ��������ͨ��coefficientParameterΪԭ�ͣ���ʱ������һ���µ�Coefficient
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
//��ϵ������
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
//�����еĲ������ҳ�������õ��Ǹ�����
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
// 	ShowDebugInformationInCon("ѧϰ���ݵ����������Ϊ��"+strBiggestRate);
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
		ShowDebugInformationInCon("��Ѳ���Ϊ��" + tempstring);
	}
	TO_STRING(_BestDealAccount.GetProceedsRate(lastPrice), tempstring);
	ShowDebugInformationInCon("ѧϰ�����������Ϊ:" + tempstring);
	TO_STRING(_BestDealAccount.GetAllBusiness().size(), tempstring);
	ShowDebugInformationInCon("ѧϰ�����״���:" + tempstring);
	if (_BestDealAccount.GetAllBusiness().size() > 1)
	{
		ShowDebugInformationInCon("ѧϰ����һ�ν�������:" + _BestDealAccount.GetAllBusiness().begin()->_Date);
		ShowDebugInformationInCon("ѧϰ�����һ����������:" + _BestDealAccount.GetAllBusiness().rbegin()->_Date);
		_BestDealAccount.PrintAllBusiness("d:\\stockfile\\allbusiness.csv");
	}
	return true;
}





