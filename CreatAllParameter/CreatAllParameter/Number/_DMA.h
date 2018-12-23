
#pragma once
#include "indexnumber.h"
#include <list>
#ifndef _DMA_H
#define _DMA_H


struct DMA_DATA/*:public IndexBaseType*/
{
	DMA_DATA() :_DDD(0), _AMA(0){}
	tySData _AMA;
	tySData _DDD;
// 	DMA_DATA& operator = (const DMA_DATA& rhs){
// 		_Index1 = rhs._Index1;
// 		_Index2 = rhs._Index2;
// 		_Index3 = rhs._Index3;
// 		_Index4 = rhs._Index4;
// 		_Index5 = rhs._Index5;
// 		return *this;
// 	}
};
class CDMA : public CNumberManager
{
public:
	CDMA();
	~CDMA();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����DMAֵ(���ݰ�����)
	//strPath:Ŀ¼·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����DMAֵ(����������)
	//strPath:Ŀ¼·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath);
	//////////////////////////////////////////////////////////////////////////
	//����CSV�ļ��ļ����DMAֵ(����������)
	//strPath:�ļ�·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
	//////////////////////////////////////////////////////////////////////////
	//���ݵ�������ݻ�õ����DMAֵ
	//
	//////////////////////////////////////////////////////////////////////////
	void GetNextDMA(const DatePriceData& OneDayData, DMA_DATA& _Dma);
	void Inition();
private:
	list<tySData> closelyData50;
	list<tySData> frontDMAData;
	unsigned int Paremeter50;
	unsigned int Paremeter10;
};



#endif