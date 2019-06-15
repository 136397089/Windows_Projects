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




enum StockNumberType
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
	//
	_eMACD,
	_eTRIX,
	_eASI,
	_eDMA,
	_eKDJ,
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
	_eMA_MA1,
	_eMA_MA2,
	_eMA_MA3,
	_eMA_MA4,
	_eMA_UPRATE,
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
	_eEMVMA,
	//
	_eBOOLMid,
	_eBOOLUp,
	_eBOOLSTD,
	_eBOOLDown,
	//
	_eDMI_DIP,
	_eDMI_DIN,
	_eDMI_ADX,
	_eDMI_ADXR,
	//
	_eRSI1,
	_eRSI2,
	_eRSI3
};
//定义列标签的名称
#define File_Close_INDEX "close"
#define File_Date_INDEX "date"
#define File_Open_INDEX "open"
#define File_High_INDEX "high"
#define File_Low_INDEX "low"
#define File_Volume_INDEX "volume"
//MACD
// #define MACD_MA12  "MACD_Ma12"
// #define MACD_MA26  "MACD_Ma26"
// #define MACD_DIFF "MACD_Diff"
// #define MACD_DEA "MACD_DEA"
// #define MACD_BAR "MACD_BAR"
// //KDJ
// #define KDJ_K "KDJ_K"
// #define KDJ_D "KDJ_D"
// #define KDJ_J "KDJ_J"
// //DMA
// #define  DMA_D "DMA_D"
// #define  DMA_A "DMA_A"
// //TRIX
// #define TRIX_TRIX "TRIX_IX"
// #define TRIX_MA "TRIX_MA"
// #define TRIX_VTR "TRIX_tr"
// #define TRIX_VTB "TRIX_tb"
// #define TRIX_VTA "TRIX_ta"
// //MA
// #define MA_MA5 "MA_M5"
// #define MA_MA10 "MA_M10"
// #define MA_MA20 "MA_M20"
// #define MA_MA40 "MA_M40"
// #define MA_UPRATE "MA_UPRATE"
// //ChangeRate
// #define PRICECHANGE "pChange"
// #define VOLHANGE "volChange"
// //ASI
// #define ASI_I "ASI_I"
// #define ASI_T "ASI_T"
// //CDP
// #define CDP_CDP "CDP_CDP"
// #define CDP_AH "CDP_AH"
// #define CDP_NH "CDP_NH"
// #define CDP_NL "CDP_NL"
// #define CDP_AL "CDP_AL"
// //AR_BR_VR_PSY
// #define ABVP_AR "ABVP_AR"
// #define ABVP_BR "ABVP_BR"
// #define ABVP_CR "ABVP_CR"
// #define ABVP_CRMA1 "ABVP_CRMA1"
// #define ABVP_CRMA2 "ABVP_CRMA2"
// #define ABVP_CRMA3 "ABVP_CRMA3"
// #define ABVP_CRMA4 "ABVP_CRMA4"
// #define ABVP_VR "ABVP_VR"
// #define ABVP_PSY "ABVP_PSY"
// #define EMV_EMV "EMV_EMV"
// #define EMV_EMVMA "EMV_EMVMA"


std::string GetNumberNameByIndexType(StockNumberType _indextype);
map<StockNumberType, string> GetAllNumberype();
//线程函数
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam);

class CNumberBase;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
struct SinCyclePriceData
{
public:
	SinCyclePriceData():_Open(0.0f),_Close(0.0f),_High(0.0f),_Low(0.0f),_Volume(0){}
	void Inition();
	void Inition(StockDataType _dOpen,StockDataType _dClose,StockDataType _dHigh,StockDataType _dLow,StockDataType _dVol,string _strdate);
	bool operator ==(const SinCyclePriceData& d)const ;
	CDate mDate;
	StockDataType _Open;
	StockDataType _Close;
	StockDataType _High;
	StockDataType _Low;
	StockDataType _Volume;
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
	void AddColumnToCsvFile(string FilePath, VStockData vMACDValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//读CSV文件中对应行的数据
	//filepath:文件路径
	//strTittle：需要读出的行名
	//////////////////////////////////////////////////////////////////////////
	VStockData  ReadRanksStringFormFile(string filepath, string strTittle);

	//////////////////////////////////////////////////////////////////////////
	//调用ReSavefileRanks之前运行
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFileBegin(string FilePath);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//
	//tittle:行名
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFile(string FilePath, const  VStockData& vNewValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//
	//tittle:行名
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFile(string FilePath, const vector<string>& vNewValue, string tittle);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFile(string FilePath, const  map<string, StockDataType>& vNewValue);
	//////////////////////////////////////////////////////////////////////////
	//增加对应的行保存到CSV文件中
	//FilePath：文件路径
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFile(string FilePath, const  FreqListType& vNewValue);
	//////////////////////////////////////////////////////////////////////////
	//调用ReSavefileRanks之后运行
	//////////////////////////////////////////////////////////////////////////
	void AddRanksToCsvFileEnd(string FilePath);


	//////////////////////////////////////////////////////////////////////////
	//按行读文件中的数据并返回
	//
	//////////////////////////////////////////////////////////////////////////
	bool ReadDataFromCSVFileRanks(const string& fullFilePath, StringList& strAllData) const;
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


////////////////////////////////////////////////////////////////////////////
//以下是定义指标数据类型
//
//////////////////////////////////////////////////////////////////////////

struct Macd
{
public:
	Macd() :m12(0), m26(0), dif(0), dea(0), bar(0){}
	StockDataType m12;
	StockDataType m26;
	StockDataType dif;
	StockDataType dea;
	StockDataType bar;
};

struct KDJ
{
public:
	KDJ() :K_OF_KDJ(0), D_OF_KDJ(0), J_OF_KDJ(0){}
	StockDataType K_OF_KDJ;
	StockDataType D_OF_KDJ;
	StockDataType J_OF_KDJ;
};

struct DayHLCOV
{
	DayHLCOV(){};
	DayHLCOV(
		StockDataType _hlcopen,
		StockDataType _hlchigh,
		StockDataType  _hlclow,
		StockDataType _hlcclose,
		StockDataType  _hlcvol)
		:_HLCOpen(_hlcopen),
		_HLCHigh(_hlchigh),
		_HLCLow(_hlclow),
		_HLCClose(_hlcclose),
		_HLCVol(_hlcvol){}
	StockDataType _HLCOpen;
	StockDataType _HLCHigh;
	StockDataType _HLCLow;
	StockDataType _HLCClose;
	StockDataType _HLCVol;
};

struct ARBRCRVRPSY
{
	ARBRCRVRPSY() :AR(0), BR(0), VR(0), CR(0), CRMa1(0), CRMa2(0), CRMa3(0), CRMa4(0), PSY(0){}
	StockDataType AR;
	StockDataType BR;
	StockDataType VR;
	StockDataType CR;
	StockDataType CRMa1;
	StockDataType CRMa2;
	StockDataType CRMa3;
	StockDataType CRMa4;
	StockDataType PSY;
};

struct DMA_DATA
{
	DMA_DATA() :_DDD(0), _AMA(0){}
	StockDataType _AMA;
	StockDataType _DDD;
};

struct ASI
{
	ASI() :_asi(0), _asit(0){}
	StockDataType _asi;//
	StockDataType _asit;//均值
};

struct CDP
{
	CDP() :_CDP(0), _AH_High(0), _NH_NormalHigh(0), _AL_Low(0), _NL_NormalLow(0){}
	StockDataType _CDP;
	StockDataType _AH_High;
	StockDataType _NH_NormalHigh;
	StockDataType _AL_Low;
	StockDataType _NL_NormalLow;

};

struct  Dmi
{
	StockDataType _DIP;
	StockDataType _DIN;
	StockDataType _ADX;
	StockDataType _ADXR;
	Dmi() :_DIP(0), _DIN(0), _ADX(0), _ADXR(0){}
};
struct DMTR
{
	DMTR() :DMP(0), DMM(0), TR(0){}
	StockDataType DMP;
	StockDataType DMM;
	StockDataType TR;
// 	StockDataType ADX;

};

struct EMV
{
	StockDataType emv;
	StockDataType emvma;
	StockDataType TemporaryVariable;
	StockDataType highToLowMa;
	EMV() :emv(0), emvma(0), TemporaryVariable(1), highToLowMa(1){}
};

struct MA
{
	MA() :Ma1(0), Ma2(0), Ma3(0), Ma4(0) {}
	StockDataType Ma1;
	StockDataType Ma2;
	StockDataType Ma3;
	StockDataType Ma4;
	StockDataType upRate;
};
struct TRIX 
{
	TRIX() :_TRIX(0), _TRMA(0), _AX(0), _BX(0), _TR(0){}
	StockDataType _TRIX;
	StockDataType _TRMA;
	StockDataType _AX;
	StockDataType _BX;
	StockDataType _TR;

};
struct BOOLIndex
{
	BOOLIndex() :MidPrice(0), Uper(0), Downer(0){}
	StockDataType MidPrice;
	StockDataType PriceSTD;
	StockDataType Uper;
	StockDataType Downer;
};
struct RSI
{
	RSI() :RSI1(0), RSI2(0), RSI3(0){}
	StockDataType RSI1;
	StockDataType RSI2;
	StockDataType RSI3;
};
struct MACDCombin
{
	Macd TodayMacd;
	Macd YesterdayMacd;
	Macd BefoYesMacd;
	Macd ForecastMacd;

	CDate Today;
	CDate Yesterday;
	CDate BefoYes;
	CDate CurrentDate;
};

struct KDJCombin
{
	KDJ		TodayKDJ;
	KDJ		YesterdayKDJ;
	KDJ		BefYesKDJ;
	KDJ		ForecastKDJ;

	CDate	Today;
	CDate	Yesterday;
	CDate	BefYes;
	CDate	CurrentDate;
};




struct realTimeDataToAna
{
	KDJCombin CloselyKDJs;
	MACDCombin CloselyMACDs;
};



struct DayPrice
{
	StockDataType _closeData;
	StockDataType _highData;
	StockDataType _lowData;
	StockDataType _openData;
	CDate _date;

	StockDataType _frontClose;
	StockDataType _frontHigh;
	StockDataType _frontLow;
	StockDataType _frontOpen;
	CDate _frontdate;

};




enum BasisType
{
	_eDPUnknow = 0,

	_eBasisDayDIFF = (1 << 0),
	_eBasisDeDayDIFF = (1 << 1),
	_eBasisDayCR = (1 << 2),//
	_eBasisDayKDJ_D = (1 << 3),
	_eBasisDeDayKDJ_K = (1 << 4),
	_eBasisDeDayDEA = (1 << 5),

	_eBasisShDayDIFF = (1 << 6),
	_eBasisShDeDayDIFF = (1 << 7),
	_eBasisShDeDayDEA = (1 << 8),
	_eBasisShDeDayKDJ_K = (1 << 9),
	_eBasisDeDayKDJ_D = (1 << 10),
	_eBasisDayCTO = (1 << 11),

	_eBasisDayKDJ_K = (1 << 12),
	_eBasisShDeDayBAR = (1 << 13),
	_eBasisShDeDayKDJ_D = (1 << 14),
	_eBasisDeDayBar = (1 << 15),
	_eBasisShDeKDJ_D = (1 << 16),
	_eBasisDeMa = (1 << 17),

	_eBasisDeDayCRMA = (1 << 18),
	_eBasisDayEMV = (1 << 19),

	_eBasisDeDEA = (1 << 20),
	_eBasisDeMonthDEA = (1 << 21),
	_eBasisDeMonthDIFF = (1 << 22),
	_eBasisDeDayMA4 = (1 << 23),

	_eBasisDeDayCR = (1 << 24),
	_eBasisDayRSI1 = (1 << 25),
	_eBasisDeDayRSI1 = (1 << 26),
	_eBasisFroDayRSI1 = (1 << 27),
	_eBasisDayVR = (1 << 28),
	_eBasisDeDayVR = (1 << 29)

};

enum PosNegType
{
	_eNegative = 0,

	_eBasisDayDIFF_P = (1 << 0),
	_eBasisDeDayDIFF_P = (1 << 1),
	_eBasisDayCR_P = (1 << 2),
	// 	_eBasisDeDeDayDIFF_P = (1 << 2),//
	_eBasisDayKDJ_D_P = (1 << 3),
	_eBasisDeDayKDJ_K_P = (1 << 4),
	_eBasisDeDayDEA_P = (1 << 5),

	_eBasisShDayDIFF_P = (1 << 6),
	_eBasisShDeDayDIFF_P = (1 << 7),
	_eBasisShDeDayDEA_P = (1 << 8),
	_eBasisShDeDayKDJ_K_P = (1 << 9),
	_eBasisDeDayKDJ_D_P = (1 << 10),
	_eBasisDayCTO_P = (1 << 11),

	_eBasisDayKDJ_K_P = (1 << 12),
	_eBasisShDeDayBAR_P = (1 << 13),
	_eBasisShDeDayKDJ_D_P = (1 << 14),
	_eBasisDeDayBar_P = (1 << 15),
	_eBasisShDeKDJ_D_P = (1 << 16),
	_eBasisupRate_P = (1 << 17),

	_eBasisDeDayCRMA_P = (1 << 18),
	_eBasisDayEMV_P = (1 << 19),

	_eBasisDeDEA_P = (1 << 20),
	_eBasisDeMonthDEA_P = (1 << 21),
	_eBasisDeMonthDIFF_P = (1 << 22),
	_eBasisDeDayMA4_P = (1 << 23),

	_eBasisDeDayCR_P = (1 << 24),
	_eBasisDayRSI1_P = (1 << 25),
	_eBasisDeDayRSI1_P = (1 << 26),
	_eBasisFroDayRSI1_P = (1 << 27),
	_eBasisDeDayVR_P = (1 << 28)
};

enum pSpecGroupType
{
	_eCTUnknow = 0,
	_eHighType = (1 << 0),
	_eLowType = (1 << 1),
	_eCloseType = (1 << 2),
	_eOpenType = (1 << 3),
	_eGroup1,
	_eGroup2,
	_eGroup3,
	_eGroup4,
	_eMixed = 0xeFFFFFFF
};

enum BasisCycleType
{
	eMinute5 = 1,
	eMinute15 = 2,
	eMinute30 = 3,
	eMinute60 = 4,
	eMinute120 = 5,
	eDay = 6,
	eWeek = 7,
	eMonth = 8,
	eSH = 1 << 8,
	eSZ = 2 << 8,
	eCY = 3 << 8,
	eResource
};


#endif
