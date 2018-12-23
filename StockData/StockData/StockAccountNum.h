
#ifndef _STOCKACCOUNTNUM_H
#define _STOCKACCOUNTNUM_H
#include <list>
#include <string>
#include <fstream>
using namespace std;
#pragma once

//一次交易
struct Business
{
	//bool _IsBay;//是不是买入
	string _Date;
	double _Volume;//交易数据
	double _Price;//交易价格
};
//类只对一个股票操作
class CStockAccountNum
{
public:
	CStockAccountNum(const string& StockDate, const double& InitCapital);
	//CStockAccountNum(const CStockAccountNum& stockAccount);
	~CStockAccountNum();

private:
	CStockAccountNum();
public:
	//初始化账号
	bool InitAccount(double InitCapital);
	//进行一次操作，买入或者卖出
	bool DoBusiness(const Business& mbusiness);
	//修改仓位到目标仓位
	bool ChangeStockPosition(const Business& mbusiness, const double& tagePosition);
	//卖出所有
	bool SellOutAll(const double& currentPrice);
	//获取当前仓位
	double GetPosition(const double& currentPrice);
	//全部买入
	bool AllIn(const double& currentPrice);
	//获取当前资产总值
	double GetCurrentAssets(double currentPrice);
	//获取当前收益率 正值为盈利，负值为亏损
	double GetProceedsRate(double currentPrice);
	//获得最后一次出错信息
	string GetLastError();
	//
	list<Business> GetAllBusiness();
	//
	void PrintAllBusiness(const string& filePath);
private:
	//初始变量，不变
	double _InitCapital;//初始的资金
	string _OwnStockDate;//操作的股票代码
	//过程变量，可变
	double _CurrentAvailableCapital;//当前的可用资金
	double _OwnStock;//拥有的股票数量
	//过程记录
	list<Business> _AllBusiness;//发生过的所有操作
	string _LastError;
};

#endif


