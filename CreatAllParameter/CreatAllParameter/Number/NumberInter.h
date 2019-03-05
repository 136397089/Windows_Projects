#pragma once
#ifndef _NUMBERALL_H
#define _NUMBERALL_H
#include <fstream>
#include <map>
#include "IndexNumber.h"
#include "../StockAccountNum.h"
#include "_DMA.h"
#include "Macd.h"
#include "trix.h"
#include "KDJ.h"
#include "ASI.h"
#include "../CDP.h"
#include "../DMI.h"
#include "StatusRecords.h"
#include "../log.h"
#include "../Ma.h"

typedef map<IndexType, AllStatus> IndexStatusMap;
typedef map<IndexType, AllStatus*> IndexStatusPointer;
typedef map<IndexType, const AllStatus*> IndexStatusConstPointer;
typedef map<IndexType, const VStockData*> StockDataPointer;

//������Ʊ�����н������ݺ�ָ��
struct AllStockData
{
	void clear();
	//
	AllStockData NewDataByIndex(unsigned int beginindex,unsigned int endindex) const;
	//
	inline bool ChackDataSize();
	//
	StockDataPointer GetAllPointer() const;
	//
	StockDataPointer GetIndexPointer() const;
	//��ȡ�Ĺ�Ʊ�ɽ���Ϣ
	VStockData _vOpenData;
	VStockData _vCloseData;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	VStockData _vPriChaRate;
	VStockData _vVolChaRate;
	VStockData _vMa1, _vMa2, _vMa3, _vMa4;
	vector<string> _vTimeDay;
	//ָ��������
	VStockData _vMACDValue, _vMACDMa12, _vMACDMa26, _vDiff, _vDEA;
	VStockData _vDMAValue, _vAMAValue;
	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
	VStockData _vK, _vD, _vJ;
	VStockData _vAsi, _vAsit;
};





class StatusRecords;
class CNumberInterface //:protected CNumberManager
{
public:
	CNumberInterface();
	~CNumberInterface();
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
	//����������ݵ�ָ��
	//////////////////////////////////////////////////////////////////////////
// 	StockDataPointer GetDataPointers();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	inline void PushBackIndex(const SigDayTechIndex& AllIndex);
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	//inline bool ChackDataSizeRaedFromFile();
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	void CheckAndPrintKDJMin(const string& filename);
	bool _IsInSale;
	int index;
	CLog resultFile;
	double startTime;
	AllStockData mAll;
};



#endif
