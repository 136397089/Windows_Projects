#pragma once
#ifndef _INDEXNUMBER_H
#define _INDEXNUMBER_H
//#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include "CommonFunction.h"
#include "../DateTool.h"


//����Ŀ���ļ�������������
#define MAX_FILE_LINE_READFROM_RANK 100
#define  MAX_FILE_LINE_READFROM_COLUMN 10000


enum IndexType
{
	//�����б�ǩ������
	_eFile_Close_INDEX,
	_eFile_Date_INDEX,
	_eFile_Open_INDEX,
	_eFile_High_INDEX,
	_eFile_Low_INDEX,
	_eFile_Volume_INDEX,
	//MACD
	_eMACD_MA12,
	_eMACD_MA26,
	_eMACD_DIFF,
	_eMACD_DEA,
	_eMACD_BAR,
	//KDJ
	_eKDJ_K,
	_eKDJ_D,
	_eKDJ_J,
	//DMA
	_eDMA_D,
	_eDMA_A,
	//TRIX
	_eTRIX_TRIX,
	_eTRIX_MA,
	_eTRIX_VTR,
	_eTRIX_VTB,
	_eTRIX_VTA,
	//
	_eMA_MA5,
	_eMA_MA10,
	_eMA_MA20,
	_eMA_MA40,
	_ePRICECHANGE,
	_eVOLHANGE,
	_eASI_I,
	_eASI_T
};
std::string GetIndexNameByIndexType(IndexType _indextype);
//�����б�ǩ������
#define File_Close_INDEX "close"
#define File_Date_INDEX "date"
#define File_Open_INDEX "open"
#define File_High_INDEX "high"
#define File_Low_INDEX "low"
#define File_Volume_INDEX "volume"

//MACD
#define MACD_MA12  "Ma12"
#define MACD_MA26  "Ma26"
#define MACD_DIFF "Diff"
#define MACD_DEA "DEA"
#define MACD_BAR "BAR"

//KDJ
#define KDJ_K "K_OF_KDJ"
#define KDJ_D "D_OF_KDJ"
#define KDJ_J "J_OF_KDJ"

//DMA
#define  DMA_D "DMA"
#define  DMA_A "AMA"
//TRIX
#define TRIX_TRIX "TRIX"
#define TRIX_MA "TRMA"
#define TRIX_VTR "tr"
#define TRIX_VTB "tb"
#define TRIX_VTA "ta"
//
#define MA_MA5 "M5"
#define MA_MA10 "M10"
#define MA_MA20 "M20"
#define MA_MA40 "M40"

#define PRICECHANGE "pChange"
#define VOLHANGE "volChange"

#define ASI_I "ASI"
#define ASI_T "ASIT"
//�̺߳���
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam);

class CNumberManager;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
struct DatePriceData
{
public:
	int DateIndex;
	DatePriceData():_Open(0.0f),_Close(0.0f),_High(0.0f),_Low(0.0f),DateIndex(0){}
	CDate mDate;
	tySData _Open;
	tySData _Close;
	tySData _High;
	tySData _Low;
	tySData _Volume;
	tySData _Ma5;
	tySData _Ma10;
	tySData _Ma20;
	tySData operator [] (string index);
};
struct IndexBaseType
{
	tySData _Index1;
	tySData _Index2;
	tySData _Index3;
	tySData _Index4;
	tySData _Index5;
	IndexBaseType& operator = (const IndexBaseType& rhs)
	{
		_Index1 = rhs._Index1;
		_Index2 = rhs._Index2;
		_Index3 = rhs._Index3;
		_Index4 = rhs._Index4;
		_Index5 = rhs._Index5;
		return *this;
	}
};
//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
struct NumberTreadPara
{
	CNumberManager* pmacd_;
	string filename_;
};
//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CNumberManager
{
public:
	CNumberManager(void);
	~CNumberManager(void);

	//////////////////////////////////////////////////////////////////////////
	//��CSV�ļ��ж�Ӧ�е�����
	//filepath:�ļ�·��
	//strTittle����Ҫ����������
	//////////////////////////////////////////////////////////////////////////
	VStockData  ReadColumnStringFormFile(string filepath, string strTittle);
	//////////////////////////////////////////////////////////////////////////
	//���Ӷ�Ӧ���е�CSV�ļ���
	//FilePath���ļ�·��
	//vMACDValue:Ҫ�����������
	//tittle:����
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileColumn(string FilePath, VStockData vMACDValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//��CSV�ļ��ж�Ӧ�е�����
	//filepath:�ļ�·��
	//strTittle����Ҫ����������
	//////////////////////////////////////////////////////////////////////////
	VStockData  ReadRanksStringFormFile(string filepath, string strTittle);
	//////////////////////////////////////////////////////////////////////////
	//���Ӷ�Ӧ���б��浽CSV�ļ���
	//FilePath���ļ�·��
	//vMACDValue:Ҫ�����������
	//tittle:����
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath,const  VStockData vNewValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//���Ӷ�Ӧ���б��浽CSV�ļ���
	//FilePath���ļ�·��
	//vMACDValue:Ҫ�����������
	//tittle:����
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath, const vector<string> vNewValue, string tittle);


	//////////////////////////////////////////////////////////////////////////
	//���ж��ļ��е����ݲ�����
	//
	//////////////////////////////////////////////////////////////////////////
	StringList ReadDataFromCSVFileRanks(const string& fullFilePath);
	//////////////////////////////////////////////////////////////////////////
	//��CSV�ļ���ȡ����,���и�ɶ�ά������ʽ
	//////////////////////////////////////////////////////////////////////////
	StringBlock ReadDataFromCSVFileAndCutRanks(const string& fullFilePath, string strCutIndex = ",");
	//////////////////////////////////////////////////////////////////////////
	//�����ά�����ַ�����CSV�ļ�
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToCSVFile(const string& fullFilePath, const StringBlock& savedata);


	//////////////////////////////////////////////////////////////////////////
	//�Ӽ��а��ȡ����
	//////////////////////////////////////////////////////////////////////////
	StringList ReadDataFromClickBoardRanks();
	//////////////////////////////////////////////////////////////////////////
	//�Ӽ��а��ȡ����,���и�ɶ�ά������ʽ
	//////////////////////////////////////////////////////////////////////////
	StringBlock ReadDataFromClickBoardAndCutRanks();
	//////////////////////////////////////////////////////////////////////////
	//�����ά���鵽���а�
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToClickBoard(StringBlock& savedata);
	//////////////////////////////////////////////////////////////////////////
	//�½��߳�
	//filename���ļ�·��
	//////////////////////////////////////////////////////////////////////////
	void RunTread(string filePath);
	HANDLE _StockCSVFileMutex;
public:
	int _TreadNum;
};



#endif
