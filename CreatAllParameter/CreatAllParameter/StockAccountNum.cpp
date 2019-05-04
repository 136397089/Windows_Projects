#include "stdafx.h"
#include "StockAccountNum.h"
#include "Number/commonfunction.h"
#include "glog/logging.h"

static const bool G_bay = true;
static const bool G_sell = false;

CStockAccount::CStockAccount(const string& StockDate, const float& InitCapital) :
_InitCapital(InitCapital),
_OwnStockCode(StockDate)
{
	_LastError = "";
	//_InitCapital = 1000;//��ʼ�ʽ�
	_CurrentAvailableCapital = _InitCapital;
	_StockQuantityOwned = 0;
	_minVolu = 10;
	_lastSellPrise = 0;
	_lastBuyPrise = 0;
	_AllBusiness.clear();
}
CStockAccount::CStockAccount() :
_InitCapital(1000),
_OwnStockCode("000000")
{
	_LastError = "";
	_CurrentAvailableCapital = _InitCapital;
	_StockQuantityOwned = 0;
	_lastSellPrise = 0;
	_lastBuyPrise = 0;
	_AllBusiness.clear();
}


CStockAccount::~CStockAccount()
{
}

string CStockAccount::GetLastError()
{
	return _LastError;
}

//���һ����������
bool CStockAccount::Trading(const TransactionData& mbusiness)
{
	//�����Ʊ����,�ʽ���
	if (mbusiness._Price * mbusiness._Volume > _CurrentAvailableCapital && mbusiness._Volume > 0.0)
	{
		//LOG(ERROR) << "Stock Account error:Lack of Available Capital.";
		_LastError = "Lack of Available Capital.";
		return false;
	}
	//������Ʊ����,��Ʊ����
	if (_StockQuantityOwned + mbusiness._Volume < 0 && mbusiness._Volume < 0.0)
	{
		//LOG(ERROR) << "Stock Account error:Lack of Operable Stock.";
		_LastError = "Lack of Operable Stock.";
		return false;
	}
	//�����Ʊ
	if (mbusiness._Volume > 0.0)
	{
		_lastBuyPrise = mbusiness._Price;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_StockQuantityOwned = _StockQuantityOwned + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//������Ʊ
	if (mbusiness._Volume < 0.0)
	{
		_lastSellPrise = mbusiness._Price;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_StockQuantityOwned = _StockQuantityOwned + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//LOG(ERROR) << "Stock Account error:Business do not finish.";//û�м�鵽���������£�����û�����
	return false;
}

//��õ�ǰ�Ĳ�λ��������[0��1]֮��
float CStockAccount::GetPosition(float price)
{
//	return _CurrentPosition;
	if (_CurrentAvailableCapital + _StockQuantityOwned * price > 0)
	{
		return  _StockQuantityOwned * price / (_CurrentAvailableCapital + _StockQuantityOwned * price);
	}
	else
	{
		LOG(ERROR) << "Stock Account error:Current Assets is negative.";//�����ʲ��Ǹ�ֵ
		_LastError = "Current Assets is negative.";
		return -1;
	}
}
//
//�������й�Ʊ
bool CStockAccount::SellOutAll(const float& currentPrice, const string& date)
{
	TransactionData mbusiness;
	mbusiness._Price = currentPrice;
	mbusiness._Date.SetDay(date);
	mbusiness._strStockCode = _OwnStockCode;
	mbusiness._Volume = -_StockQuantityOwned;
	if (!Trading(mbusiness))
	{
		return false;
	}

	return true;
}
//
//ȫ������
bool CStockAccount::AllIn(const float& currentPrice, const string& date)
{
	TransactionData mbusiness;
	mbusiness._Price = currentPrice;
	mbusiness._Date.SetDay(date);
	mbusiness._strStockCode = _OwnStockCode;
	mbusiness._Volume = floor(_CurrentAvailableCapital / currentPrice / _minVolu)*_minVolu;
	if (!Trading(mbusiness))
	{
		return false;
	}
	return true;
}
//
//�޸Ĳ�λ��Ŀ���λ
bool CStockAccount::ChangeStockPosition(const float& currentPrice, const string& Date, const float& tagePosition)
{
	//1.0�жϲ�λ�Ƿ����
	float localTagePosition = tagePosition;
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
	//1.1�жϼ۸��Ƿ�������
	if (currentPrice <= 0)
	{
		LOG(ERROR) << "Stock Account error:Price error.";
		_LastError = "Price error.";
		return false;
	}
	//1.2�����λ���̫С���򲻽��в���
	if (localTagePosition - GetPosition(currentPrice) <= 0.01 && localTagePosition - GetPosition(currentPrice) >= -0.01)
	{
		LOG(INFO) << "Stock Account Info: Position Change too small.";
		return true;
	}
	float currentPosition = GetPosition(currentPrice);
	//����Ŀ���λ�Ǵ��ڻ���С�ڵ�ǰ��λ������һ���ļ��㹫ʽ
	TransactionData tempBusiness;
	if (localTagePosition > currentPosition)
	{
		tempBusiness._Price = currentPrice;
		tempBusiness._Date.SetDay(Date);
		tempBusiness._Volume = tagePosition*(_CurrentAvailableCapital / currentPrice + _StockQuantityOwned) - _StockQuantityOwned;
		tempBusiness._Volume = floor(tempBusiness._Volume / _minVolu) * _minVolu;//�����Ʊ������ȡ��
		Trading(tempBusiness);
		LOG(INFO) << "Stock Account Info: Bay.";

	}
	else
	{
		tempBusiness._Price = currentPrice;
		tempBusiness._Date.SetDay(Date);
		tempBusiness._Volume = tagePosition*(_CurrentAvailableCapital / currentPrice + _StockQuantityOwned) - _StockQuantityOwned;
		tempBusiness._Volume = floor(tempBusiness._Volume / _minVolu) * _minVolu;//������Ʊ������ȡ��
		Trading(tempBusiness);
		LOG(INFO) << "Stock Account Info: sell.";
	}
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
	return _CurrentAvailableCapital + currentPrice * _StockQuantityOwned;
}

list<TransactionData> CStockAccount::GetAllBusiness()
{
	return _AllBusiness;
}

bool CStockAccount::Inition(float InitialFunding)
{
	_lastSellPrise = 0;
	_lastBuyPrise = 0;

	_LastError = "";

	_InitCapital = InitialFunding;
	_CurrentAvailableCapital = InitialFunding;
	_StockQuantityOwned = 0.0f;
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
		outfile << _OwnStockCode << endl;
		outfile << "Date,Price,Volume"<<endl;
		for (list<TransactionData>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end();ite++)
			outfile << ite->_Date.GetDay() << ","<< ite->_Price << ","<< ite->_Volume << endl;
		outfile.close();
	}
}

float CStockAccount::GetLastBuyPrice()
{
	return _lastBuyPrise;
}

bool CStockAccount::BuyStock(const TransactionData& mbusiness)
{
	if (mbusiness._Volume < 0)
	{
		LOG(ERROR) << "Stock Account error:Volume error.";
		_LastError = "Volume error.";
		return false;
	}
	if (mbusiness._Price * mbusiness._Volume > _CurrentAvailableCapital)//�ʽ𲻹�������ô��Ĺ�Ʊ
	{
		LOG(ERROR) << "Stock Account error:Lack of Available Capital.";
		_LastError = "Lack of Available Capital.";
		return false;
	}
	return true;
}

bool CStockAccount::SellStock(const TransactionData& mbusiness)
{
	TransactionData business = mbusiness;
	if (mbusiness._Volume < 0)//����
	{
		LOG(ERROR) << "Stock Account error:Volume error.";
		_LastError = "Volume error.";
		return false;
	}
	if (business._Volume > _StockQuantityOwned)//ӵ�еĹ�Ʊ��Ҫ�������Ĺ�Ʊ��
	{
		business._Volume = _StockQuantityOwned;
	}
	return true;
}

float CStockAccount::GetStockQuantityOwned()
{
	return _StockQuantityOwned;
}

float CStockAccount::GetLastSellPrice()
{
	return _lastSellPrise;
}


