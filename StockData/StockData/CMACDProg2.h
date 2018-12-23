#pragma once
#include <string>
#include <list>
#include <vector>
#include <map>
#include "CommonStruct.h"
#include "StockAccountNum.h"
#include "PosiCal.h"
#ifndef _COMPUTEVALUEBTMACE2_H
#define _COMPUTEVALUEBTMACE2_H

using namespace std;


typedef vector <vector<string>> DimensionString;



#define MAX_SIZE 10


class CMACDProg2
{
public:

	CMACDProg2();
	~CMACDProg2();
	/*
	//入口函数
	*/
	virtual bool RunInterface();
private:
	/*
	//预处理文件夹下所有的CSV文件数据
	*/
	bool PretreatmentFileData(const string& filePath);
	/*
	//
	*/
	void SavePIDDataToFile(ParameterDatas todayDaya);
	/*
	//
	*/
	bool GetProceedsRate(double& proceedsRate, CStockAccountNum& account);
	/*
	//
	*/
	bool VerificationCoefData(double& ProceedsRate);
	/*
	//学习分析股票
	*/
	bool ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount);
	/*
	//分析得出股票的目标仓位
	*/
	double AnalysisPosition(const list<OneDayVariable>& _DayVariableList);
	/*
	//将所有的系数都设为1
	*/
	void SetCoefDataToOne();
	/*
	//系数进化，通过coefficientParameter为原型，随时进化出一批新的PIDCoefficient
	*/
	vector<Coefficients> CoefEvolution(const Coefficients& coefficientParameter);
	/*
	//在所有的参数中找出收益最好的那个参数
	*/
	bool GetBiggestCoefData(vector<double> ProceedsRateLise,
							vector<Coefficients> relativeCofList,
							vector<CStockAccountNum> StockAccountlist);
	/*
	//
	*/
	bool ShowBestStudyBusinessResult();
	/*
	//将系数保存
	*/
	bool SavePIDCoefficientToFile(const Coefficients& RelativeCoef, const Coefficients& ComplementCoef);

	string GetLastError();
private:

	string _LastError;
	string _DealDate;
	double _InitCapital;

	Coefficients _RelativeCoefData;//每次要用于计算系数

	Coefficients _BestRelativeCoefData;//最好的系数
	Coefficients _BestComplementRelCoefData;//最好的补数系统

	//
	ValueParameter _ValueParameter;
	double _StudyClose;
	
	//交易账号
	CStockAccountNum _BestDealAccount;//收益最好的账号
	//在处理数据时，临时用于存放最近几天数据
	list<OneDayVariable> _DayVariableList;
	//一个CSV文件中的所有数据
	DimensionString::iterator dayIterator;
	DimensionString::iterator closeIterator;
	DimensionString::iterator DiffIterator;
	DimensionString::iterator DEAIterator;
	DimensionString::iterator BARIterator;
	DimensionString::iterator ChangeRateIterator;
	DimensionString _DataFromFile;
};




#endif
