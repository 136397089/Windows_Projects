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




typedef map<IndexType, StateTable> IndexStateMap;
typedef map<IndexType, StateTable*> IndexStatePointer;
typedef map<IndexType, const StateTable*> IndexStateConstPointer;


//�������
#define MACD_INDEX1 36.0f
#define MACD_INDEX2 78.0f
#define KDJ_INDEX1 9.0f
#define KDJ_INDEX2 3.0f
#define KDJ_INDEX3 3.0f

class SinDayData: public SinDayPriceData
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
class CNumberInterface //:protected CNumberManager
{
public:
	CNumberInterface();
	~CNumberInterface();
	//////////////////////////////////////////////////////////////////////////
	//���ļ��ж����ݣ�������ָ��
	//////////////////////////////////////////////////////////////////////////
	bool GetDataAndNumbers(const string& filename ,const string& FilePath);

	//
	const StockDataTable& GetDayValue() const;
	//
	const StockDataTable& GetWeekValue() const;
	//
	const StockDataTable& GetMonthValue() const;
	//���ļ����ж������յ�����
	bool GetTodayData(const string& fileFullPath, map<string, SinDayData>& TodayData);
private:
	//////////////////////////////////////////////////////////////////////////
	//���ļ��ж������е�����
	//////////////////////////////////////////////////////////////////////////
	void RaedDateFromFile(const string& strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//���������е�����
	//////////////////////////////////////////////////////////////////////////
	void ProcessingTransverseData(const StringBlock& AllString);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	bool ResizeData(CDate beginData);
	bool _IsInSale;
	int index;
	//CLog resultFile;
	double startTime;
	StockDataTable mDayValue;
	StockDataTable mWeekValue;
	StockDataTable mMonthValue;
};



#endif
