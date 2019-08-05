
#ifndef ALLSTOCKDATA_H
#define ALLSTOCKDATA_H

#include <map>
#include "number/NumberBase.h"
// #include "number/StateRecords.h"
// #include "StockAccountNum.h"

typedef map<StockNumberType,  VStockData const *> StockDataPointer;

//������Ʊ�����н������ݺ�ָ��
struct StockDataTable
{
	//
	void clear();
	//
	StockDataTable NewDataByIndex(unsigned int beginindex, unsigned int endindex) const;
	//
	void GetIndexMap(const vector<CDate>& InputDate,map<unsigned int,unsigned int>& indexMap);
	//
	bool ChackDataSize() const;
	//�����������ݵ�ָ�룬���ڱ������ݵ��ļ����У�
	StockDataPointer const GetAllPointerToSave();
	//��������ָ���ָ��
// 	StockDataPointer GetNumberPointer() const;
	//�ַ�����ʽ����תΪCDate��ʽ������
	void SetDate();
	//ͨ�����ڲ���ǰһʱ�̵�Index
	unsigned int GetCloselyFrontTimeIndexByDate(string strDate) const;
	//ͨ�����ڲ���ǰһʱ�̵�Index
	unsigned int GetCloselyFrontTimeIndexByDate(CDate date)const;
	//��ȡ�Ĺ�Ʊ�ɽ���Ϣ
	CycleType DataCycle;
	string _strStockCode;
	vector<string> _vTimeDay;
	vector<CDate> _vDate;
	vector<bool> DataEnable;
	VStockData _vOpen;
	VStockData _vClose;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	VStockData _vCumulativeChangerate;
	VStockData _vMapToMarket;
	//
	map<StockNumberType, VStockData> _vTableAllIndex;
	//ָ��������
};

#endif
