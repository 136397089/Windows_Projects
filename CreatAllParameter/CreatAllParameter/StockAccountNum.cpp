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
	//_InitCapital = 1000;//��ʼ�ʽ�
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_CurrentPosition = 0.0f;
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

//���һ����������
bool CStockAccount::_���н���(const ��������& mbusiness)
{
	//�����Ʊ����,�ʽ���
	if (mbusiness._���׼۸� * mbusiness._�ɽ��� > _CurrentAvailableCapital && mbusiness._�ɽ��� > 0.0)
	{
		LOG(ERROR) << "Stock Account error:Lack of Available Capital.";
		LOG(INFO) << "Stock Account error:Lack of Available Capital.";
		_LastError = "Lack of Available Capital.";
		return false;
	}
	//������Ʊ����,��Ʊ����
	if (_OwnStock + mbusiness._�ɽ��� < 0 && mbusiness._�ɽ��� < 0.0)
	{
		LOG(ERROR) << "Stock Account error:Lack of Operable Stock.";
		LOG(INFO) << "Stock Account error:Lack of Operable Stock.";
		_LastError = "Lack of Operable Stock.";
		return false;
	}
	//�����Ʊ
	if (mbusiness._���׼۸� * mbusiness._�ɽ��� <= _CurrentAvailableCapital && mbusiness._�ɽ��� > 0.0)
	{
		_lastPrise = mbusiness._���׼۸�;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._���׼۸� * mbusiness._�ɽ���;
		_OwnStock = _OwnStock + mbusiness._�ɽ���;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//������Ʊ
	else if (_OwnStock + mbusiness._�ɽ��� >= 0  && mbusiness._�ɽ��� < 0.0)
	{
		_lastPrise = mbusiness._���׼۸�;
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._���׼۸� * mbusiness._�ɽ���;
		_OwnStock = _OwnStock + mbusiness._�ɽ���;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	LOG(ERROR) << "Stock Account error:Business do not finish.";//û�м�鵽���������£�����û�����
	return false;
}

//��õ�ǰ�Ĳ�λ��������[0��1]֮��
double CStockAccount::GetPosition()
{
	return _CurrentPosition;
// 	if (_CurrentAvailableCapital + _OwnStock * _lastPrise > 0)
// 	{
// 		return  _OwnStock * _lastPrise / (_CurrentAvailableCapital + _OwnStock * _lastPrise);
// 	}
// 	else
// 	{
// 		LOG(ERROR) << "Stock Account error:Current Assets is negative.";//�����ʲ��Ǹ�ֵ
// 		_LastError = "Current Assets is negative.";
// 		return -1;
// 	}
}
//
//�������й�Ʊ
bool CStockAccount::SellOutAll(const double& currentPrice, const string& date)
{
	if (!ChangeStockPosition(currentPrice, date, 0.0f))
	{
		return false;
	}

	return true;
}
//
//ȫ������
bool CStockAccount::AllIn(const double& currentPrice, const string& date)
{
	ChangeStockPosition(currentPrice,date,1.0f);
	return true;
}
//
//�޸Ĳ�λ��Ŀ���λ
bool CStockAccount::ChangeStockPosition(const double& currentPrice, const string& Date, const double& tagePosition)
{
	//1.0�жϲ�λ�ǲ�����
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
	//1.1�жϼ۸��Ƿ�������
	if (currentPrice <= 0)
	{
		LOG(ERROR) << "Stock Account error:Price error.";
		_LastError = "Price error.";
		return false;
	}
	//1.2�����λ���̫С���򲻽��в���
	if (localTagePosition - GetPosition() <= 0.01 && localTagePosition - GetPosition() >= -0.01)
	{
		LOG(INFO) << "Stock Account Info: Position Change too small.";
		return true;
	}
	double currentPosition = GetPosition();
	//����Ŀ���λ�Ǵ��ڻ���С�ڵ�ǰ��λ������һ���ļ��㹫ʽ
	�������� tempBusiness;
	if (localTagePosition > _CurrentPosition)
	{
		tempBusiness._���׼۸� = currentPrice;
		tempBusiness._�������� = Date;
		tempBusiness._�ɽ��� = _CurrentAvailableCapital * (localTagePosition - _CurrentPosition) / currentPrice - 1;
		LOG(INFO) << "Stock Account Info: Bay.";
		if (_���н���(tempBusiness))
		{
			_CurrentPosition = localTagePosition;
		}
	}
	else
	{
		tempBusiness._���׼۸� = currentPrice;
		tempBusiness._�������� = Date;
		tempBusiness._�ɽ��� = _OwnStock * (localTagePosition - _CurrentPosition) / _CurrentPosition;
		LOG(INFO) << "Stock Account Info: sell.";
		if (_���н���(tempBusiness))
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

list<��������> CStockAccount::GetAllBusiness()
{
	return _AllBusiness;
}

bool CStockAccount::��ʼ���˺ź���(double ��ʼ�ʽ�)
{
	_lastPrise = 0;
	_LastError = "";

	_InitCapital = ��ʼ�ʽ�;
	_CurrentAvailableCapital = ��ʼ�ʽ�;
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
		for (list<��������>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end();ite++)
			outfile << ite->_�������� << ","<< ite->_���׼۸� << ","<< ite->_�ɽ��� << endl;
		outfile.close();
	}
}

double CStockAccount::GetLastPrice()
{
	return _lastPrise;
}


