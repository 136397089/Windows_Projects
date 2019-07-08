#include "stdafx.h"
#include "StockAccountNum.h"
#include "Number/commonfunction.h"
#include "glog/logging.h"


CStockAccount::CStockAccount( const float& InitCapital) :
_InitCapital(InitCapital)
{
	Inition(InitCapital);
}

bool CStockAccount::Inition(float InitialFunding)
{
	HoldStock._iStockCode = 0;
	HoldStock._strStockCode = "";
	HoldStock.BuyPrice = -1.0f;
	HoldStock.SellPrice = -1.0f;
	HoldStock.TransactionVolume = 0;

	_LastError = "";
	_InitCapital = InitialFunding;
	_CurrentAvailableCapital = InitialFunding;
	_AllBusiness.clear();
	return true;
}



CStockAccount::~CStockAccount()
{
}

string CStockAccount::GetLastError()
{
	return _LastError;
}

//完成一次买卖操作
bool CStockAccount::Trading(const TransactionData& mbusiness)
{
	if (_CurrentAvailableCapital > mbusiness.BuyPrice * mbusiness.TransactionVolume)
	{
		StockDataType FrontCost = HoldStock.BuyPrice * HoldStock.TransactionVolume;
		HoldStock.BuyDate = mbusiness.BuyDate;
		HoldStock.BuyPrice = mbusiness.BuyPrice;
		HoldStock._iStockCode = mbusiness._iStockCode;
		HoldStock._strStockCode = mbusiness._strStockCode;
		HoldStock.TransactionVolume = mbusiness.TransactionVolume;
		_CurrentAvailableCapital -= mbusiness.BuyPrice * mbusiness.TransactionVolume;
	}
	return true;
}

//获得当前的仓位，正常在[0，1]之间
float CStockAccount::GetPosition(float price)
{
	return 0;
}
//
//卖出所有股票
bool CStockAccount::SellOutAll(const float& currentPrice, const string& date)
{
	return true;
}
//
//全部买入
bool CStockAccount::AllIn(const float& currentPrice, const string& date)
{
	return true;
}
//
//修改仓位到目标仓位
bool CStockAccount::ChangeStockPosition(const float& currentPrice, const string& Date, const float& tagePosition)
{
	return true;
}

float CStockAccount::GetProceedsRate(float currentPrice)
{
	if (_InitCapital != 0)
	{
		return (GetCurrentAssets(currentPrice) / _InitCapital - 1.0f);
	}
	else
	{
		return 0.0;
	}
}

float CStockAccount::GetCurrentAssets(float currentPrice)
{
	return 0;
}

list<TransactionData> CStockAccount::GetAllBusiness()
{
	return _AllBusiness;
}


void CStockAccount::PrintAllBusiness(const string& filePath)
{
}

TransactionData CStockAccount::GetLastTransaction()
{
	return HoldStock;
}

bool CStockAccount::BuyStock(const TransactionData& mbusiness)
{
	return true;
}

bool CStockAccount::SellStock(const TransactionData& mbusiness)
{
	return true;
}

float CStockAccount::GetStockQuantityOwned()
{
	return 0;
}





CSimpleAccount::CSimpleAccount(const string& Stockcode)
{
	HoldStock._strStockCode = Stockcode;
	_AllBusiness.clear();
	ClearHoldStockPrice();
}

CSimpleAccount::~CSimpleAccount()
{
}

void CSimpleAccount::Inition(const string& Stockcode)
{
	HoldStock._strStockCode = Stockcode;
	_AllBusiness.clear();
	ClearHoldStockPrice();
}

bool CSimpleAccount::BuyStock(StockDataType price, const CDate& date)
{
	if (HoldStock.BuyPrice > 0 || price < 0)
	{
		LOG(ERROR) << "error: Has bought shares";
		return false;
	}

	HoldStock.BuyDate = date;
	HoldStock.BuyPrice = price;
	return true;
}

bool CSimpleAccount::SellStock(StockDataType price, const CDate& date)
{
	if (HoldStock.BuyPrice > 0)
	{
		HoldStock.SellPrice = price;
		HoldStock.SellDate = date;
		_AllBusiness.push_back(HoldStock);
		ClearHoldStockPrice();
		return true;
	}
	LOG(ERROR) << "error: Not buying stocks";
	return false;
}

void CSimpleAccount::ClearHoldStockPrice()
{
	CDate iniDate;
	HoldStock.BuyPrice = 0.0f;
	HoldStock.BuyDate = iniDate;
	HoldStock.SellPrice = 0.0f;
	HoldStock.SellDate = iniDate;
	HoldStock.TransactionVolume = 0.0f;
}

bool CSimpleAccount::IsHoldStock()
{
	if (HoldStock.BuyPrice > 0.001f)
		return true;
	return false;
}

StockDataType CSimpleAccount::GetCurrentIncomeRate()
{
	StockDataType IncomeRate = 1;
	for (list<TransactionData>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end();ite++)
	{
		IncomeRate = IncomeRate * ite->SellPrice / ite->BuyPrice;
	}
	return IncomeRate;
}

unsigned int CSimpleAccount::GetCount()
{
	return _AllBusiness.size();
}

void CSimpleAccount::GetIncomeRateList(vector<StockDataType>& rateList)
{
	for (list<TransactionData>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end(); ite++)
	{
		rateList.push_back(ite->SellPrice / ite->BuyPrice - 0.001);
		if (ite->SellPrice / ite->BuyPrice < 0.8)
		{
			LOG(INFO) 
				<< "Fail operation:" << ite->_strStockCode 
				<< " begin at:" << ite->BuyDate.GetDay()
				<< " Buy price:" << ite->BuyPrice
				<< " end at:" << ite->SellDate.GetDay()
				<< " Sell price" << ite->SellPrice
				<< " IncomeRate:" << (ite->SellPrice / ite->BuyPrice);
		}
	}
}

StockDataType CSimpleAccount::CheckHoldIncomeRate(StockDataType CurrentPrice)
{
	if (HoldStock.BuyPrice <=0.001)
	{
		return 1.0f;
	}
	return CurrentPrice / HoldStock.BuyPrice;
}
