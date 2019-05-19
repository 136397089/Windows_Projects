#pragma once
#ifndef _NUMBERBASE_H
#define _NUMBERBASE_H
//#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <map>
#include "CommonFunction.h"
#include "../DateTool.h"


//读出目标文件允许的最大行数
#define MAX_FILE_LINE_READFROM_RANK 100
#define  MAX_FILE_LINE_READFROM_COLUMN 10000


enum IndexType
{
	//定义列标签的名称
	_eFile_Close_INDEX,
	_eFile_Date_INDEX,
	_eFile_Open_INDEX,
	_eFile_High_INDEX,
	_eFile_Low_INDEX,
	_eFile_HighLow_INDEX,
	_eFile_Volume_INDEX,
	_Debug_Index,
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
	//MA
	_eMA_MA5,
	_eMA_MA10,
	_eMA_MA20,
	_eMA_MA40,
	//
	_ePRICECHANGE,
	_eVOLHANGE,
	//
	_eASI_I,
	_eASI_T,
	//
	_eCDP_CDP,
	_eCDP_AH,
	_eCDP_NH,
	_eCDP_NL,
	_eCDP_AL,
	//
	_eAR,
	_eBR,
	_eCR,
	_eCRMA1,
	_eCRMA2,
	_eCRMA3,
	_eCRMA4,
	_eVR,
	_ePSY,
	//
	_eEMV,
	_eEMVMA
};
//定义列标签的名称
#define File_Close_INDEX "close"
#define File_Date_INDEX "date"
#define File_Open_INDEX "open"
#define File_High_INDEX "high"
#define File_Low_INDEX "low"
#define File_Volume_INDEX "volume"
//MACD
#define MACD_MA12  "MACD_Ma12"
#define MACD_MA26  "MACD_Ma26"
#define MACD_DIFF "MACD_Diff"
#define MACD_DEA "MACD_DEA"
#define MACD_BAR "MACD_BAR"
//KDJ
#define KDJ_K "KDJ_K"
#define KDJ_D "KDJ_D"
#define KDJ_J "KDJ_J"
//DMA
#define  DMA_D "DMA_D"
#define  DMA_A "DMA_A"
//TRIX
#define TRIX_TRIX "TRIX_IX"
#define TRIX_MA "TRIX_MA"
#define TRIX_VTR "TRIX_tr"
#define TRIX_VTB "TRIX_tb"
#define TRIX_VTA "TRIX_ta"
//MA
#define MA_MA5 "MA_M5"
#define MA_MA10 "MA_M10"
#define MA_MA20 "MA_M20"
#define MA_MA40 "MA_M40"
//ChangeRate
#define PRICECHANGE "pChange"
#define VOLHANGE "volChange"
//ASI
#define ASI_I "ASI_I"
#define ASI_T "ASI_T"
//CDP
#define CDP_CDP "CDP_CDP"
#define CDP_AH "CDP_AH"
#define CDP_NH "CDP_NH"
#define CDP_NL "CDP_NL"
#define CDP_AL "CDP_AL"
//AR_BR_VR_PSY
#define ABVP_AR "ABVP_AR"
#define ABVP_BR "ABVP_BR"
#define ABVP_CR "ABVP_CR"
#define ABVP_CRMA1 "ABVP_CRMA1"
#define ABVP_CRMA2 "ABVP_CRMA2"
#define ABVP_CRMA3 "ABVP_CRMA3"
#define ABVP_CRMA4 "ABVP_CRMA4"
#define ABVP_VR "ABVP_VR"
#define ABVP_PSY "ABVP_PSY"
#define EMV_EMV "EMV_EMV"
#define EMV_EMVMA "EMV_EMVMA"

std::string GetIndexNameByIndexType(IndexType _indextype);

//线程函数
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam);

class CNumberBase;

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
	StockDataType _Open;
	StockDataType _Close;
	StockDataType _High;
	StockDataType _Low;
	StockDataType _Volume;
	StockDataType _Ma5;
	StockDataType _Ma10;
	StockDataType _Ma20;
	StockDataType operator [] (string index);
};
struct IndexBaseType
{
	StockDataType _Index1;
	StockDataType _Index2;
	StockDataType _Index3;
	StockDataType _Index4;
	StockDataType _Index5;
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

struct MeanVarPoint
{
	StockDataType _MeanToVarRatio;
	StockDataType _Var;
	StockDataType _Mean;
	unsigned int _TimeIndex;
	CDate _Date;
	MeanVarPoint()
	{
		_TimeIndex = 0;
		_Var = 0;
		_Mean = 0;
	}
};

typedef map<float, int> FreqListType;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
struct NumberTreadPara
{
	CNumberBase* pmacd_;
	string filename_;
};
//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CNumberBase
{
public:
	CNumberBase(void);
	~CNumberBase(void);

	//////////////////////////////////////////////////////////////////////////
	//读CSV文件中对应列的数据
	//filepath:文件路径
	//strTittle：需要读出的列名
	//////////////////////////////////////////////////////////////////////////
	VStockData  ReadColumnStringFormFile(string filepath, string strTittle);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的列到CSV文件中
	//FilePath：文件路径
	//vMACDValue:要加入的列数据
	//tittle:列名
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileColumn(string FilePath, VStockData vMACDValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//读CSV文件中对应行的数据
	//filepath:文件路径
	//strTittle：需要读出的行名
	//////////////////////////////////////////////////////////////////////////
	VStockData  ReadRanksStringFormFile(string filepath, string strTittle);

	//////////////////////////////////////////////////////////////////////////
	//调用ReSavefileRanks之前运行
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanksBegin(string FilePath);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//
	//tittle:行名
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath, const  VStockData& vNewValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//
	//tittle:行名
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath, const vector<string>& vNewValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath, const  map<string, StockDataType>& vNewValue);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanks(string FilePath, const  FreqListType& vNewValue);
	//////////////////////////////////////////////////////////////////////////
	//调用ReSavefileRanks之后运行
	//////////////////////////////////////////////////////////////////////////
	void ReSavefileRanksEnd(string FilePath);


	//////////////////////////////////////////////////////////////////////////
	//按行读文件中的数据并返回
	//
	//////////////////////////////////////////////////////////////////////////
	StringList ReadDataFromCSVFileRanks(const string& fullFilePath);
	//////////////////////////////////////////////////////////////////////////
	//从CSV文件读取数据,并切割成二维数组形式
	//////////////////////////////////////////////////////////////////////////
	StringBlock ReadDataFromCSVFileAndCutRanks(const string& fullFilePath, string strCutIndex = ",");
	//////////////////////////////////////////////////////////////////////////
	//保存二维数组字符串到CSV文件
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToCSVFile(const string& fullFilePath, const StringBlock& savedata);


	//////////////////////////////////////////////////////////////////////////
	//从剪切板读取数据
	//////////////////////////////////////////////////////////////////////////
	StringList ReadDataFromClickBoardRanks();
	//////////////////////////////////////////////////////////////////////////
	//从剪切板读取数据,并切割成二维数组形式
	//////////////////////////////////////////////////////////////////////////
	StringBlock ReadDataFromClickBoardAndCutRanks();
	//////////////////////////////////////////////////////////////////////////
	//保存二维数组到剪切板
	//////////////////////////////////////////////////////////////////////////
	bool SaveDataToClickBoard(StringBlock& savedata);
	//////////////////////////////////////////////////////////////////////////
	//新建线程
	//filename：文件路径
	//////////////////////////////////////////////////////////////////////////
	void RunTread(string filePath);
	HANDLE _StockCSVFileMutex;

	//ReSavefileRanks使用参数
	vector<string> buffer;
	vector<string> newBuffer;

public:
	int _TreadNum;
};



#endif
