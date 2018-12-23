#pragma once
#include <string>
#include <list>
#include <vector>
#include <map>
#include "CommonStruct.h"
#include "StockAccountNum.h"
#ifndef _COMPUTEVALUEBTMACE_H
#define _COMPUTEVALUEBTMACE_H

using namespace std;


typedef vector <vector<string>> DimensionString;

struct PIDParameters
{
	PID closeRel;
	PID closeAbs;
	PID ChangeRateRel;
	PID ChangeRateAbs;
	PID MACD_EDARel;
	PID MACD_EDAAbs;
	PID MACD_DiffRel;
	PID MACD_DiffAbs;
	PID MACD_BARRel;
	PID MACD_BARAbs;
	inline double GetSum();
 	//LimitParameter operator / (LimitParameter inpudata);

}; 
struct PIDCoefficients
{
	PID closeRel;
	PID closeAbs;
	PID ChangeRateRel;
	PID ChangeRateAbs;
	PID MACD_EDARel;
	PID MACD_EDAAbs;
	PID MACD_DiffRel;
	PID MACD_DiffAbs;
	PID MACD_BARRel;
	PID MACD_BARAbs;
	inline double GetSum();
};




#define MAX_SIZE 10


class CMACDProg1
{
public:

	CMACDProg1();
	~CMACDProg1();
	/*入口函数
	//
	*/
	virtual bool RunInterface();
	/*
	//
	*/
	virtual bool SetParameter(ValueParameter mPatameter);
	/*
	//
	*/
	virtual string GetLastError();
	/*
	//计算PID在上下限之间的比例值
	*/
	PID GetRelativeValuePID(string index);
	/*
	//
	*/
	bool SaveMLimitParameter(PIDLimits todayDaya);
	/*
	//
	*/
	PID GetAbsoluteValuePID(string index);
private:
	/*
	//
	*/
	bool ToPretreatment(const OneDayVariable& dayVar);
	/*
	//
	*/
	bool PretreatmentFileData(const string& filePath);
	/*
	//
	*/
	void ShowPID(const PID& showdata,string tiitleName);
	/*
	//
	*/
	void SavePIDDataToFile(PIDParameters todayDaya);
	/*
	//
	*/
	DimensionString::iterator GetDataIter(DimensionString);
	/*
	//
	*/
	bool StudyStock(double& ProceedsRate);
	/*
	//
	*/
	bool VerificationCoefData(double& ProceedsRate);
	/*
	//学习分析股票
	*/
	bool ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount);
	/*
	//获得所有PID值在上下限当中当中的比例位置
	*/
	bool GetProportionPosition(PIDParameters& realTimePID);
	/*
	//分析得出股票的目标仓位
	*/
	double AnalysisPosition(const PIDParameters& realTimePID);
	/*
	//获得PID值在上下限当中当中的比例位置
	*/
	bool GetPIDValueProportionPosition(const PID& upLimit, const PID& downLimit, PID& index);
	/*
	//计算PID的补数
	*/
	PID GetComplement(const PID& index);
	/*
	//将所有PID的系数都设为1
	*/
	void SetCoefDataToOne();
	/*
	//系数进化，通过coefficientParameter为原型，随时进化出一批新的PIDCoefficient
	*/
	vector<PIDCoefficients> CoefEvolution(const PIDCoefficients& coefficientParameter);
	/*
	//
	*/
	bool GetBiggestCoefData(vector<double> ProceedsRateLise,
							vector<PIDCoefficients> relativeCofList,
							vector<PIDCoefficients> complementCofList,
							vector<CStockAccountNum> StockAccountlist);
	/*
	//
	*/
	bool ShowBestBusinessResult(double ProceedsRate);
	/*
	//
	*/
	bool SavePIDCoefficientToFile(const PIDCoefficients& RelativeCoef, const PIDCoefficients& ComplementCoef);
private:
	//上个方案使用的数据
	PIDLimits _RelativeLimitData;
	PIDLimits _AbsoluteLimitData;
	string _LastError;
	string _DealDate;
	PIDCoefficients _RelativeCoefData;//PID系数
	PIDCoefficients _ComplementRelCoefData;//PID补数的相关系统
	//
	ValueParameter _ValueParameter;
	
	//交易账号
	CStockAccountNum _DealAccount;
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
