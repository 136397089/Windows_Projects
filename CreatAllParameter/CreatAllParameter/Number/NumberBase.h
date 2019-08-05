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
	_ePriceChangeRate,
	_eVolChangeRate,
	_eDataEnable,
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
	_eMACD_K,
	_eMACD_D,
	_eMACD_J,
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
	_eCDP_LOWEST,
	//
	_eAR,
	_eBR,
	_eCR,
	_eCRMA1,
	_eCRMA2,
	_eCRMA3,
	_eCRMA4,
	_eVR,
	_eVRMA,
	_ePSY,
	_eLimitUpTime,
	_eLimitDownTime,
	//
	_eEMV,
	_eEMVMA,
	//
	_eBOOLMid,
	_eBOOLUp,
	_eBOOLSTD,
	_eBOOLDown,
	_eBOOLCDP,
	_eBOOLCDPAUP,
	_eBOOLCDPNUP,
	_eBOOLCDPNDown,
	_eBOOLCDPADown,
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
	ARBRCRVRPSY() :AR(0), BR(0), VR(0), CR(0), CRMa1(0), CRMa2(0), CRMa3(0), CRMa4(0), PSY(0), VRMA(0){}
	StockDataType AR;
	StockDataType BR;
	StockDataType VR;
	StockDataType VRMA;
	StockDataType CR;
	StockDataType CRMa1;
	StockDataType CRMa2;
	StockDataType CRMa3;
	StockDataType CRMa4;
	StockDataType PSY;
	StockDataType LimitUpTimes;
	StockDataType LimitDownTimes;
};
struct GeneralIndicators
{
	GeneralIndicators(): PriceChangeRate(0),VolChangerate(0),Dataable(0){}
	StockDataType PriceChangeRate;
	StockDataType VolChangerate;
	StockDataType Dataable;
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
	StockDataType _Lowest;

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

};

struct EMV
{
	StockDataType emv;
	StockDataType emvma;
	EMV() :emv(0), emvma(0){}
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
	StockDataType cdp;
	StockDataType cdpNUp;
	StockDataType cdpNDown;
	StockDataType cdpAUp;
	StockDataType cdpADown;
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
	DayPrice() :_closeData(0), _highData(0), _lowData(0),_openData(0),
		_frontClose(0), _frontHigh(0), _frontLow(0), _frontOpen(0) {}
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

enum HigherOrderType : unsigned int
{
	_eDPUnknow = 0,

	DIP = (1 << 0),
	DIN = (1 << 1),
	DeDIN = (1 << 2),//
	DeDIPN = (1 << 3),
	ADX = (1 << 4),
	DeADX = (1 << 5),

	BAR = (1 << 6),
	DeBAR = (1 << 7),
	Dif = (1 << 8),
	DeDif = (1 << 9),
	DEA = (1 << 10),
	DeDEA = (1 << 11),

	KDJ_K = (1 << 12),
	KDJ_D = (1 << 13),
	DeKDJ_K = (1 << 14),
	DeKDJ_D = (1 << 15),
	DeKDJ_J = (1 << 16),


	HOVR = (1 << 17),
	HOVRMA = (1 << 18),
	DeVR = (1 << 19),
	HOCR = (1 << 20),
	DeCR = (1 << 21),
	DeCRMA = (1 << 22),


	Able = (1 << 23),
	WeekKDJ_D = (1 << 24),
	DeWeekKDJ_D = (1 << 25),
	MonthKDJ_K = (1 << 26),
	DeMonthKDJ_K = (1 << 27),
	MonthKDJ_D = (1 << 28),
	DeMonthKDJ_D = (1 << 29)
};


enum ENotesType
{
	_eCTUnknow = 0,
	_Mid2MidType = 1<<0,
	_BClose = 1 << 1,
	_BHigh = 1 << 2,
	_BLow = 1 << 3,
	_BFClose = 1 << 4,
	_BFHigh = 1 << 5,
	_BFLow = 1 << 6,
	_EHigh = 1 << 7,
	_EClose = 1 << 8,
	_ELow = 1 << 9,
	_EFClose = 1 << 10,
	_EFHigh = 1 << 11,
	_EFLow = 1 << 12,
	_Max = 1<<13,
	_Min = 1<<14,
	_eGroup1,
	_eGroup2,
	_eGroup3,
	_eGroup4,
	_eMixed = 0xeFFFFFFF
};

enum CycleType
{
	eMinute5 = 1,
	eMinute15 = 2,
	eMinute30 = 3,
	eMinute60 = 4,
	eMinute120 = 5,
	eDay = 6,
	eWeek = 7,
	eMonth = 8,
	eMarketDay = 9,
	eMarketWeek = 10,
	eMarketMonth = 11,
	eUnkCycle
};


#endif
