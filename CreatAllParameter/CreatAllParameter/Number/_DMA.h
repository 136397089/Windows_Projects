
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
	//更新目录下所有CSV文件里的DMA值(数据按列排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的DMA值(天数按行排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath);
	//////////////////////////////////////////////////////////////////////////
	//更新CSV文件文件里的DMA值(天数按行排)
	//strPath:文件路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
	//////////////////////////////////////////////////////////////////////////
	//根据当天的数据获得当天的DMA值
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