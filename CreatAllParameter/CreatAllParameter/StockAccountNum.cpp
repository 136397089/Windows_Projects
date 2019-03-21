#include "stdafx.h"
#include "StockAccountNum.h"
#include "Number/commonfunction.h"
#include "glog/logging.h"

static const bool G_bay = true;
static const bool G_sell = false;

CStockAccount::CStockAccount(const string& StockDate, const double& InitCapital) :
_InitCapital(InitCapital),
_OwnStockDate(StockDate)
{
	_LastError = "";
	//_InitCapital = 1000;//初始资金
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_CurrentPosition = 0.0f;
	_minVolu = 100;
	_AllBusiness.clear();
}
CStockAccount::CStockAccount() :
_InitCapital(1000),
_OwnStockDate("000000")
{
	_LastError = "";
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_AllBusiness.clear();
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
	//买入股票错误,资金不足
	if (mbusiness._Price * mbusiness._Volume > _CurrentAvailableCapital && mbusiness._Volume > 0.0)
	{
		LOG(ERROR) << "Stock Account error:Lack of Available Capital.";
		LOG(INFO) << "Stock Account error:Lack of Available Capital.";
		_LastError = "Lack of Available Capital.";
		return false;
	}
	//卖出股票错误,股票不足
	if (_OwnStock + mbusiness._Volume < 0 && mbusiness._Volume < 0.0)
	{
		LOG(ERROR) << "Stock Account error:Lack of Operable Stock.";
		LOG(INFO) << "Stock Account error:Lack of Operable Stock.";
		_LastError = "Lack of Operable Stock.";
		return false;
	}
	//买入股票
	if (mbusiness._Price * mbusiness._Volume <= _CurrentAvailableCapital && mbusiness._Volume > 0.0)
	{
		_lastPrise = mbusiness._Price;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_OwnStock = _OwnStock + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//卖出股票
	else if (_OwnStock + mbusiness._Volume >= 0 && mbusiness._Volume < 0.0)
	{
		_lastPrise = mbusiness._Price;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_OwnStock = _OwnStock + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	LOG(ERROR) << "Stock Account error:Business do not finish.";//没有检查到出错的情况下，交易没有完成
	return false;
}

//获得当前的仓位，正常在[0，1]之间
double CStockAccount::GetPosition()
{
	return _CurrentPosition;
// 	if (_CurrentAvailableCapital + _OwnStock * _lastPrise > 0)
// 	{
// 		return  _OwnStock * _lastPrise / (_CurrentAvailableCapital + _OwnStock * _lastPrise);
// 	}
// 	else
// 	{
// 		LOG(ERROR) << "Stock Account error:Current Assets is negative.";//出错：资产是负值
// 		_LastError = "Current Assets is negative.";
// 		return -1;
// 	}
}
//
//卖出所有股票
bool CStockAccount::SellOutAll(const double& currentPrice, const string& date)
{
	if (!ChangeStockPosition(currentPrice, date, 0.0f))
	{
		return false;
	}

	return true;
}
//
//全部买入
bool CStockAccount::AllIn(const double& currentPrice, const string& date)
{
	ChangeStockPosition(currentPrice,date,1.0f);
	return true;
}
//
//修改仓位到目标仓位
bool CStockAccount::ChangeStockPosition(const double& currentPrice, const string& Date, const double& tagePosition)
{
	//1.0判断仓位是否出错
	double localTagePosition = tagePosition;
	if (localTagePosition > 1.0)
	{
		LOG(INFO) << "Stock Account error:Tage position outoff limit,set it to 1.0f.";
		localTagePosition = 1.0f;
	}
	if (localTagePosition < 0.0f)
	{
		LOG(INFO) << "Stock Account error:Tage position outoff limit,set it to 0.0f.";
		localTagePosition = 0.0f;
	}
	//1.1判断价格是否有问题
	if (currentPrice <= 0)
	{
		LOG(ERROR) << "Stock Account error:Price error.";
		_LastError = "Price error.";
		return false;
	}
	//1.2如果仓位相差太小，则不进行操作
	if (localTagePosition - GetPosition() <= 0.01 && localTagePosition - GetPosition() >= -0.01)
	{
		LOG(INFO) << "Stock Account Info: Position Change too small.";
		return true;
	}
	double currentPosition = GetPosition();
	//不管目标仓位是大于还是小于当前仓位，都是一样的计算公式
	TransactionData tempBusiness;
	if (localTagePosition > _CurrentPosition)
	{
		tempBusiness._Price = currentPrice;
		tempBusiness._Date.SetDay(Date);
		tempBusiness._Volume = _CurrentAvailableCapital * (localTagePosition - _CurrentPosition) / currentPrice - 1;
		tempBusiness._Volume = floor(tempBusiness._Volume / _minVolu) * _minVolu;
		LOG(INFO) << "Stock Account Info: Bay.";
		if (Trading(tempBusiness))
		{
			_CurrentPosition = localTagePosition;
		}
	}
	else
	{
		tempBusiness._Price = currentPrice;
		tempBusiness._Date.SetDay(Date);
		tempBusiness._Volume = _OwnStock * (localTagePosition - _CurrentPosition) / _CurrentPosition;
		LOG(INFO) << "Stock Account Info: sell.";
		if (Trading(tempBusiness))
		{
			_CurrentPosition = localTagePosition;
		}
	}
	return true;
}

double CStockAccount::GetProceedsRate(double currentPrice)
{
	if (_InitCapital != 0)
	{
		return (GetCurrentAssets(currentPrice) / _InitCapital) - 1;
	}
	else
	{
		return 0.0;
	}
}

double CStockAccount::GetCurrentAssets(double currentPrice)
{
	return _CurrentAvailableCapital + currentPrice * _OwnStock;
}

list<TransactionData> CStockAccount::GetAllBusiness()
{
	return _AllBusiness;
}

bool CStockAccount::Inition(double InitialFunding)
{
	_lastPrise = 0;
	_LastError = "";

	_InitCapital = InitialFunding;
	_CurrentAvailableCapital = InitialFunding;
	_OwnStock = 0.0f;
	_CurrentPosition = 0.0f;
	_AllBusiness.clear();
	return true;
}

void CStockAccount::PrintAllBusiness(const string& filePath)
{
	fstream outfile(filePath, ios::out | ios::app);
	if (outfile)
	{
		if (_AllBusiness.size() == 0)
		{
			LOG(INFO) << "Stock Account info: Business size is zero.";
			return;
		}
		outfile << _OwnStockDate << endl;
		outfile << "Date,Price,Volume"<<endl;
		for (list<TransactionData>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end();ite++)
			outfile << ite->_Date.GetDay() << ","<< ite->_Price << ","<< ite->_Volume << endl;
		outfile.close();
	}
}

double CStockAccount::GetLastPrice()
{
	return _lastPrise;
}


