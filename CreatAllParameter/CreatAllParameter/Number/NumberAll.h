#pragma once
#ifndef _NUMBERALL_H
#define _NUMBERALL_H
#include <fstream>
#include "IndexNumber.h"
#include "../StockAccountNum.h"
#include "_DMA.h"
#include "Macd.h"
#include "trix.h"
#include "KDJ.h"
#include "ASI.h"
#include "../CDP.h"
#include "../DMI.h"
#include "StateRecords.h"
#include "../log.h"


struct AllStockData
{
	void clear();
	//��ȡ�Ĺ�Ʊ�ɽ���Ϣ
	VStockData _vOpenData;
	VStockData _vCloseData;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	VStockData _vMa5;
	VStockData _vMa10;
	VStockData _vMa20;
	vector<string> _vTimeDay;
	//ָ��������
	VStockData _vMACDValue, _vMa12, _vMa26, _vDiff, _vDEA;
	VStockData _vDMAValue, _vAMAValue;
	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
	VStockData _vK, _vD, _vJ;
	VStockData _vAsi, _vAsit;
};


class StateRecords;
class CNumberAll //:protected CNumberManager
{
public:
	CNumberAll();
	~CNumberAll();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ���ļ��е�����
	//////////////////////////////////////////////////////////////////////////
	bool AnalyseTheFile(const string& filename ,const string& FilePath);


	const AllStockData& GetAllValue();

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
	//������������ݱ��浽�ļ�����
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToFile(const string& strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//���̷���һ
	//////////////////////////////////////////////////////////////////////////
	void ToSimulationSL1(const DatePriceData& date, StateRecords& hStateRecode, CStockAccount& _Account);
	//////////////////////////////////////////////////////////////////////////
	//���̷�����
	//////////////////////////////////////////////////////////////////////////
	void ToSimulationSL2(const float close, const string& date, StateRecords& hStateRecode, CStockAccount& _Account);
	//////////////////////////////////////////////////////////////////////////
	//����������ݵ�ָ��
	//////////////////////////////////////////////////////////////////////////
	StockDataPointer GetDataPointers();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	inline void PushBackIndex(const SigDayTechIndex& AllIndex);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	inline bool ChackDataSizeRaedFromFile();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void CheckAndPrintKDJMin(const string& filename);
	bool _IsInSale;
	int index;
	CLog resultFile;
	//fstream resultFileCsv;
	double startTime;
	AllStockData mAll;
	//��ȡ�Ĺ�Ʊ�ɽ���Ϣ
// 	VStockData _vOpenData;
// 	VStockData _vCloseData;
// 	VStockData _vHigh;
// 	VStockData _vLow;
// 	VStockData _vVolume;
// 	VStockData _vMa5;
// 	VStockData _vMa10;
// 	VStockData _vMa20;
// 	vector<string> _vTimeDay;
// 	//ָ��������
// 	VStockData _vMACDValue, _vMa12, _vMa26, _vDiff, _vDEA;
// 	VStockData _vDMAValue, _vAMAValue;
// 	VStockData _vTRIX, _vTRMA,_vtr,_vtb,_vta;
// 	VStockData _vK, _vD, _vJ;
// 	VStockData _vAsi, _vAsit;
	/*VStockData vMa5, vMa10, vMa20, vMa40;*/
};



#endif
