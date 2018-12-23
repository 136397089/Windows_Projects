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
	/*��ں���
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
	//����PID��������֮��ı���ֵ
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
	//ѧϰ������Ʊ
	*/
	bool ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount);
	/*
	//�������PIDֵ�������޵��е��еı���λ��
	*/
	bool GetProportionPosition(PIDParameters& realTimePID);
	/*
	//�����ó���Ʊ��Ŀ���λ
	*/
	double AnalysisPosition(const PIDParameters& realTimePID);
	/*
	//���PIDֵ�������޵��е��еı���λ��
	*/
	bool GetPIDValueProportionPosition(const PID& upLimit, const PID& downLimit, PID& index);
	/*
	//����PID�Ĳ���
	*/
	PID GetComplement(const PID& index);
	/*
	//������PID��ϵ������Ϊ1
	*/
	void SetCoefDataToOne();
	/*
	//ϵ��������ͨ��coefficientParameterΪԭ�ͣ���ʱ������һ���µ�PIDCoefficient
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
	//�ϸ�����ʹ�õ�����
	PIDLimits _RelativeLimitData;
	PIDLimits _AbsoluteLimitData;
	string _LastError;
	string _DealDate;
	PIDCoefficients _RelativeCoefData;//PIDϵ��
	PIDCoefficients _ComplementRelCoefData;//PID���������ϵͳ
	//
	ValueParameter _ValueParameter;
	
	//�����˺�
	CStockAccountNum _DealAccount;
	//�ڴ�������ʱ����ʱ���ڴ�������������
	list<OneDayVariable> _DayVariableList;
	//һ��CSV�ļ��е���������
	DimensionString::iterator dayIterator;
	DimensionString::iterator closeIterator;
	DimensionString::iterator DiffIterator;
	DimensionString::iterator DEAIterator;
	DimensionString::iterator BARIterator;
	DimensionString::iterator ChangeRateIterator;
	DimensionString _DataFromFile;
};




#endif
