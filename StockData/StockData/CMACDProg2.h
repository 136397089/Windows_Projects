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
	//��ں���
	*/
	virtual bool RunInterface();
private:
	/*
	//Ԥ�����ļ��������е�CSV�ļ�����
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
	//ѧϰ������Ʊ
	*/
	bool ToStudy(const OneDayVariable& dayValue, CStockAccountNum& stockAccount);
	/*
	//�����ó���Ʊ��Ŀ���λ
	*/
	double AnalysisPosition(const list<OneDayVariable>& _DayVariableList);
	/*
	//�����е�ϵ������Ϊ1
	*/
	void SetCoefDataToOne();
	/*
	//ϵ��������ͨ��coefficientParameterΪԭ�ͣ���ʱ������һ���µ�PIDCoefficient
	*/
	vector<Coefficients> CoefEvolution(const Coefficients& coefficientParameter);
	/*
	//�����еĲ������ҳ�������õ��Ǹ�����
	*/
	bool GetBiggestCoefData(vector<double> ProceedsRateLise,
							vector<Coefficients> relativeCofList,
							vector<CStockAccountNum> StockAccountlist);
	/*
	//
	*/
	bool ShowBestStudyBusinessResult();
	/*
	//��ϵ������
	*/
	bool SavePIDCoefficientToFile(const Coefficients& RelativeCoef, const Coefficients& ComplementCoef);

	string GetLastError();
private:

	string _LastError;
	string _DealDate;
	double _InitCapital;

	Coefficients _RelativeCoefData;//ÿ��Ҫ���ڼ���ϵ��

	Coefficients _BestRelativeCoefData;//��õ�ϵ��
	Coefficients _BestComplementRelCoefData;//��õĲ���ϵͳ

	//
	ValueParameter _ValueParameter;
	double _StudyClose;
	
	//�����˺�
	CStockAccountNum _BestDealAccount;//������õ��˺�
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
