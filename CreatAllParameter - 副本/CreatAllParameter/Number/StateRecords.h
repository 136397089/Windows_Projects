#ifndef _STATERECOEDS_H
#define  _STATERECOEDS_H
#include <assert.h>  
#include "../StateB.h"




//
//构造不需要参数
//类功能，根据输入的参数指标，计算指标的趋势形态和静态形态
//有6个public函数
//
// class StateRecords
// {
// public:
// 	StateRecords();
// 	~StateRecords();
// 	///////////////////////////////////////////////////////////////////////////
// 	//输入数据，进行记录
// 	//////////////////////////////////////////////////////////////////////////
// 	void DailyStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData);
// 	//////////////////////////////////////////////////////////////////////////
// 	//返回趋势分析结果
// 	//////////////////////////////////////////////////////////////////////////
// 	TrendState GetNowTrendnamicState();
// 	//////////////////////////////////////////////////////////////////////////
// 	//对最终的数据进行趋势判断
// 	//////////////////////////////////////////////////////////////////////////
// 	bool GetFinalTrend(const StockDataPointer& pointers);
// 	//////////////////////////////////////////////////////////////////////////
// 	//初始化，清除所有内部趋势结果
// 	//////////////////////////////////////////////////////////////////////////
// 	void Inition();
// 	//////////////////////////////////////////////////////////////////////////
// 	//返回最后一次出错信息
// 	//////////////////////////////////////////////////////////////////////////
// 	string GetLastError();
// 	//////////////////////////////////////////////////////////////////////////
// 	//
// 	//////////////////////////////////////////////////////////////////////////
// 	void ResultPrint();
// private:
// 	/*
// 	单日分析中使用到的函数
// 	*/
// 	//输入数据，进行单日趋势分析
// 	//////////////////////////////////////////////////////////////////////////
// 	void DailyTrendStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData);
// 	///////////////////////////////////////////////////////////////////////////
// 	//单日单指标趋势分析
// 	//////////////////////////////////////////////////////////////////////////
// 	void TrendStateRecordSingleStep(
// 		const IndexGenre& valueType,
// 		const tySData& dayValue,
// 		const string& day);
// 	/*
// 	最终分析的使用到的函数
// 	*/
// 	//////////////////////////////////////////////////////////////////////////
// 	//检查所有容器指针是否正常，
// 	//////////////////////////////////////////////////////////////////////////
// 	bool ChckeAllPointer(const StockDataPointer& pointers);
// 	//////////////////////////////////////////////////////////////////////////
// 	//最终分析中，查找局部最低点的index
// 	//////////////////////////////////////////////////////////////////////////
// 	bool GetDownIndex(
// 		const VStockData& pointers, 
// 		const unsigned int parameter, 
// 		vector<unsigned int>& indexVector,
// 		VStockData& LowValue);
// 	//////////////////////////////////////////////////////////////////////////
// 	//最终分析中，查找局部最高点的index
// 	//////////////////////////////////////////////////////////////////////////
// 	bool GetUpIndex(
// 		const VStockData& pointers, 
// 		const unsigned int parameter, 
// 		vector<unsigned int>& indexVector,
// 		VStockData& HighValue);
// 	//////////////////////////////////////////////////////////////////////////
// 	//将静态分析的结果输出到Log文件
// 	//////////////////////////////////////////////////////////////////////////
// 	//void StaticResultPrint();
// 	//////////////////////////////////////////////////////////////////////////
// 	//将局部分析的结果输出到Log文件
// 	//////////////////////////////////////////////////////////////////////////
// // 	void LocalResultPrint();
// 	//////////////////////////////////////////////////////////////////////////
// 	//将趋势分析的结果输出到Log文件
// 	//////////////////////////////////////////////////////////////////////////
// 	void TrendResultPrint();
// private:
// 	/*
// 	趋势分析中使用的数据
// 	*/
// 	//
// 	TrendState _NowState_Trend;
// 	//TrendState _BottonState_Tr;
// 	unsigned int _TrAnaBeginSize;
// 	vector<TrendStateTpye> _vIndexRecord_Trend;
// 	//单日分析中，记录当前的Mark
// 	unsigned int _CurrentMark;
// 	//CLog log;
// 	//最后一次出错信息
// 	string _LastError;
// };


#endif