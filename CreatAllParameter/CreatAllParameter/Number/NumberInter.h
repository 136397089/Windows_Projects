#pragma once
#ifndef _NUMBERALL_H
#define _NUMBERALL_H
#include <fstream>
#include <map>
#include "NumberBase.h"
#include "../StockAccountNum.h"
#include "../log.h"
#include "../StockDataTable.h"
#include "StateRecords.h"
#include "../NumbersToSql.h"



typedef map<StockNumberType, StateTable> IndexStateMap;
typedef map<StockNumberType, StateTable*> IndexStatePointer;
typedef map<StockNumberType, const StateTable*> IndexStateConstPointer;


//�������
#define MACD_INDEX1 36.0f
#define MACD_INDEX2 78.0f
#define KDJ_INDEX1 9.0f
#define KDJ_INDEX2 3.0f
#define KDJ_INDEX3 3.0f

class SinDayData: public SinCyclePriceData
{
public:
	StockDataType changepercent;//�ǵ���
	StockDataType turnoverratio;//������
	StockDataType per; //��ӯ��
	StockDataType pb;//�о���
	StockDataType mktcap;//����ֵ
	StockDataType nmc;//��ͨ��ֵ
// 	StockDataType _Open;
// 	StockDataType _Close;
// 	StockDataType _High;
// 	StockDataType _Low;
// 	StockDataType _Volume;
	SinDayData()
		:changepercent(0), turnoverratio(0), per(0), pb(0), mktcap(0), nmc(0)
		/*,_Open(0), _Close(0), _High(0), _Low(0), _Volume(0)*/{}
};


class StateRecords;
class CIndicatorsInterface //:protected CNumberManager
{
public:
	CIndicatorsInterface();
	~CIndicatorsInterface();
	//////////////////////////////////////////////////////////////////////////
	//���ļ��ж����ݣ�������ָ��
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndIndicators_History(const string& filename, const string& FilePath);

	//
	bool MappingToSH(const StockDataTable& shdata);

	bool GetDataAndIndicators_SH(const string& filename, const string& FilePath);

	//////////////////////////////////////////////////////////////////////////
	//���ļ��ж����ݣ�������ָ��
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndIndicatorMintue_History(const string& filename, const string& FilePath);
	//
	StockDataTable& GetResourceValue();
	//
	StockDataTable& GetWeekValue();
	//
	StockDataTable& GetMonthValue();
	//
	StockDataTable& GetRealtimeValue();
	//��MYSQL�ж������й�Ʊ��ʵʱ����
	void RefreshAllStockDate_RealTime();
	//
	bool GetIndicators_Realtime(const string& _stockName, CycleType dataCycle);
	//
	CDate GetLastDate();
private:
	//����ʵʱ�ļ۸�������Ŀǰ��Ʊ������
	bool GetStockPriceData_RealTime(const string& stockData, CycleType dataCycle,map<RealDataIndex, SinCyclePriceData>& returnData);
	//����ָ��
	bool Cal30MinuteIndicators();
	//����ָ��
	bool CalResourceIndicators();
	//����ָ��
	bool CalWeekIndicators();
	//����ָ��
	bool CalMonthIndicators();
	//////////////////////////////////////////////////////////////////////////
	//���ļ��ж������е����ݣ�ȫ������Ϊ�ַ����� AllString��
	//////////////////////////////////////////////////////////////////////////
	void RaedDateFromFile(const string& strFilePath, StringBlock& AllString);
	//////////////////////////////////////////////////////////////////////////
	//���ַ������ݷ���ɿ����ڼ��������
	//////////////////////////////////////////////////////////////////////////
	void ProcessingTransverseData(const StringBlock& AllString, StockDataTable& mTargetValue);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool ResizeData(CDate beginData);
	//������ڶ�Ӧ��ʱ��
	CDate GetCycleTime(CycleType dataCycle);
	StockDataTable mResourceValue;
	StockDataTable mWeekValue;
	StockDataTable mMonthValue;
	bool IsLongitudinal;//��־�����Ƿ��������
	//���ڴ��ʵʱ����
	StockDataTable mRealTimeValue;
	map<RealDataIndex, SinCyclePriceData> CurrentData;
	vector<RealDataIndex> NoUpdatesTableType;
	map<unsigned int, unsigned int> IndexMap;
};



#endif
