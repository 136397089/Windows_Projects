#pragma once

#ifndef GROUP_H
#define GROUP_H
#include <map>
#include "number/commonfunction.h"
#include "number/NumberBase.h"
#include "FreqStatistice.h"
#include "MeanVariance.h"
using namespace std;

enum BasisType
{
	_eDPUnknow = 0,

	_eBasisDayDIFF = (1<<0),
	_eBasisDeDayDIFF = (1 << 1),
	_eBasisDayCR = (1 << 2),//
// 	_eBasisDeDeDayDIFF = (1 << 2),//
    _eBasisMonthKDJ_KJ = (1 << 3),
	_eBasisDeDayKDJ_K = (1 << 4),
// 	_eBasisDeDeDayKDJ_K = (1 << 5),

	_eBasisShDayDIFF = (1 << 6),
	_eBasisShDeDayDIFF = (1 << 7),
// 	_eBasisShDeDeDayDIFF = (1 << 8),
	_eBasisShDeDayKDJ_K = (1 << 9),
// 	_eBasisShDeDeDayKDJ_K = (1 << 10),
	_eBasisDayMean = (1 << 11),

	_eBasisMonthDIFF = (1 << 12),
	_eBasisDeMonthDIFF = (1 << 13),
// 	_eBasisDeDeMonthDIFF = (1 << 14),
	_eBasisMonthKDJ_K = (1 << 15),
	_eBasisDeMonthKDJ_K = (1 << 16),
// 	_eBasisDeDeMonthKDJ_K = (1 << 17),

	_eBasisDeDayCRMA = (1 << 18),
	_eBasisShDeDayCRMA = (1 << 19),

	_eBasisDeDayMA1 = (1 << 20),
	_eBasisDeDayMA2 = (1 << 21),
	_eBasisDeDayMA3 = (1 << 22),
	_eBasisDeDayMA4 = (1 << 23),

	_eBasisDeDayCR = (1 << 24),
	_eBasisShDeDayCR = (1 << 25),
	_eBasisDayRate = (1 << 26),
	_eBasisDeDayRate = (1 << 27),
	_eBasisDeDayVR = (1 << 28)

};

enum PosNegType
{
	_eNegative = 0,

	_eBasisDayDIFF_P = (1 << 0),
	_eBasisDeDayDIFF_P = (1 << 1),
	_eBasisDayCR_P = (1 << 2),
// 	_eBasisDeDeDayDIFF_P = (1 << 2),//
	_eBasisMonthKDJ_KJ_P = (1 << 3),
	_eBasisDeDayKDJ_K_P = (1 << 4),
// 	_eBasisDeDeDayKDJ_K_P = (1 << 5),//

	_eBasisShDayDIFF_P = (1 << 6),
	_eBasisShDeDayDIFF_P = (1 << 7),
// 	_eBasisShDeDeDayDIFF_P = (1 << 8),
	_eBasisShDeDayKDJ_K_P = (1 << 9),
// 	_eBasisShDeDeDayKDJ_K_P = (1 << 10),
	_eBasisDayMean_P = (1 << 11),

	_eBasisMonthDIFF_P = (1 << 12),
	_eBasisDeMonthDIFF_P = (1 << 13),
// 	_eBasisDeDeMonthDIFF_P = (1 << 14),
	_eBasisMonthKDJ_K_P = (1 << 15),
	_eBasisDeMonthKDJ_K_P = (1 << 16),
// 	_eBasisDeDeMonthKDJ_K_P = (1 << 17),

	_eBasisDeDayCRMA_P = (1 << 18),
	_eBasisShDeDayCRMA_P = (1 << 19),

	_eBasisDeDayMA1_P = (1 << 20),
	_eBasisDeDayMA2_P = (1 << 21),
	_eBasisDeDayMA3_P = (1 << 22),
	_eBasisDeDayMA4_P = (1 << 23),

	_eBasisDeDayCR_P = (1 << 24),
	_eBasisShDeDayCR_P = (1 << 25),
	_eBasisDayRate_P = (1 << 26),
	_eBasisDeDayRate_P = (1 << 27),
	_eBasisDeDayVR_P = (1 << 28)

};

enum pChangeType
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

#define _Positive 1
#define _Negative 2
#define _NotSet 0
#define  MinimumChangeRate -11
#define  MaximumChangeRate 11
#define  ChangeRateStep 0.1

//外部输入的数据
struct NumberEvent
{
	BasisType NumberType;
	StockDataType number;
	NumberEvent IniData(BasisType NumberType, StockDataType number);
};

struct Group
{
	BasisType NumberType;
	PosNegType NPType;
	pChangeType changeRateType;
	int pChangeGroupIndex;
	void Inition(BasisType _NumberType, PosNegType _NPType, pChangeType _changeRateType, int _pChangeGroupIndex);
	Group()
		:NumberType(_eDPUnknow),
		NPType(_eNegative),
		pChangeGroupIndex(0),
		changeRateType(_eCTUnknow)
	{}
	bool operator < (const Group & tempdata) const;
	bool operator== (const Group & tempdata) const;
};

class CHistoryGroup
{
public:
	CHistoryGroup();
	~CHistoryGroup();

	//记录数据，进行分组
	void RecordDataFreq(const vector<NumberEvent>& _data, StockDataType _pChange, pChangeType _changeType);
	//记录数据对应的日期
	void RecordDay(const vector<NumberEvent>& _data, const CDate& _date);
	//输入一个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType, StockDataType _pChange);
	//输入两个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, int _Groupinedx);
	//输入三个指标和价格变化率，返回分组类型
	Group GetGroup(const NumberEvent& _numType1, const NumberEvent& _numType2, const NumberEvent& _numType3, int _Groupinedx);
	//输入分组类型，返回对应分组在不同收益率的频次
	bool GetNumberFreqData(Group _GroupType, vector<unsigned int>& _numdata);
	//输入分组类型，返回对应分组的总频数、收益率均值、收益率方差
	bool GetGroupMeanVar(Group _GroupType,MeanVar& returnMeanVar);
	//
	vector<Group> vkeyGroupType;
	//
	CFreqStatistice FreqTool;

	map<Group, MeanVar> meanVarData;
private:
	//记录数据对应的总频数、收益率均值、收益率方差
	bool SetGroupMeanVar(Group _GroupType, StockDataType _pChange);
	//设置需要特别关注的分组
	void SetEspecialObserveKeyType(const Group& _GroupType);
	//存放所有的分组频次数据
	map<Group, unsigned int> feqData;
	//存在分组的总频数、收益率均值、收益率方差
	//用于计算总频数、收益率均值、收益率方差的工具对象
	CMeanVariance MeanVarTool;
	//
	BasisType dateRecordBasisType;
	PosNegType dateRecordPosNegType;
	//记录需要关注的指标类型分组日期
	vector<CDate> dateRecord;
	//
	pChangeType changeRateType;
};

class COnedayGrouping
{
public:
	COnedayGrouping(){}
	~COnedayGrouping(){}
public:
	void Inition();
	bool GroupingNumbers(
		const string& stockCode,
		const vector<NumberEvent>& _data,
		pChangeType _changeType);

	bool GetGroup(BasisType NumberType, PosNegType NPType, vector<string>& TageGroup);
	map<string, Group> stockGroupResult;
};


#endif
