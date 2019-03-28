
#ifndef ALLSTOCKDATA_H
#define ALLSTOCKDATA_H

#include <map>
#include "number/NumberBase.h"
// #include "number/StateRecords.h"
// #include "StockAccountNum.h"

typedef map<IndexType, const VStockData*> StockDataPointer;

//������Ʊ�����н������ݺ�ָ��
struct StockDataTable
{
	void clear();
	//
	StockDataTable NewDataByIndex(unsigned int beginindex, unsigned int endindex) const;
	//
	bool ChackDataSize() const;
	//
	StockDataPointer GetAllPointerToSave() const;
	//
	StockDataPointer GetNumberPointer() const;
	//�ַ�����ʽ����תΪCDate��ʽ������
	void SetDate();
	//ͨ�����ڲ���ǰһʱ�̵�Index
	unsigned int GetLastTimeIndexByDate(string strDate) const;
	//ͨ�����ڲ���ǰһʱ�̵�Index
	unsigned int GetLastTimeIndexByDate(CDate date)const;
	//��ȡ�Ĺ�Ʊ�ɽ���Ϣ
	vector<string> _vTimeDay;
	vector<CDate> _vDate;
	VStockData _vOpen;
	VStockData _vClose;
	VStockData _vHigh;
	VStockData _vLow;
	VStockData _vVolume;
	//
	VStockData _vPriChaRate;
	VStockData _vVolChaRate;
	VStockData _vMa1, _vMa2, _vMa3, _vMa4;
	//ָ��������
	VStockData _vMACDValue, _vMACDMa12, _vMACDMa26, _vDiff, _vDEA;
	VStockData _vDMAValue, _vAMAValue;
	VStockData _vTRIX, _vTRMA, _vtr, _vtb, _vta;
	VStockData _vK, _vD, _vJ;
	VStockData _vAsi_i, _vAsit;
	VStockData _vCDP, _vAH_High, _vNH_NormalHigh, _vAL_Low, _vNL_NormalLow;
	VStockData _vAR, _vBR, _vVR, _vPSY, _vCR, _vCRMA1, _vCRMA2, _vCRMA3, _vCRMA4;
	VStockData _vEMV,_vEMVMA;
};

#endif
