
#pragma once
#include "indexnumber.h"
#include <list>
#ifndef _DMA_H
#define _DMA_H


struct DMA_DATA:public IndexBaseType
{
	DMA_DATA():
	_DDD(_Index1),
	_AMA(_Index2)
	{}
	tySData _AMA;
	tySData _DDD;
};
class CRight// : public CNumberManager
{
public:
	CRight();
	~CRight();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����DMAֵ(���ݰ�����)
	//strPath:Ŀ¼·��
// 	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdateFileColumn(string strFolderPath);
// 	//////////////////////////////////////////////////////////////////////////
// 	//����Ŀ¼������CSV�ļ����DMAֵ(����������)
// 	//strPath:Ŀ¼·��
// 	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdatePathFilesRanks(string strPath);
// 	//////////////////////////////////////////////////////////////////////////
// 	//����CSV�ļ��ļ����DMAֵ(����������)
// 	//strPath:�ļ�·��
// 	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdateFileRanks(string strfilename);
private:
	void GetNextDMA(float _ClostData, DMA_DATA _Dma);
	list<float> closelyData50;
};



#endif