#include "StockAccountNum.h"
#include "CommonFunction.h"


static const bool G_bay = true;
static const bool G_sell = false;

CStockAccountNum::CStockAccountNum(const string& StockDate, const double& InitCapital) :
_InitCapital(InitCapital),
_OwnStockDate(StockDate)
{
	_LastError = "";
	//_InitCapital = 1000;//��ʼ�ʽ�
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_AllBusiness.clear();
}
CStockAccountNum::CStockAccountNum() :
_InitCapital(1000),
_OwnStockDate("000000")
{
	_LastError = "";
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_AllBusiness.clear();
}

// CStockAccountNum::CStockAccountNum(const CStockAccountNum& stockAccount) :
// _InitCapital(stockAccount._InitCapital),
// _OwnStockDate(stockAccount._OwnStockDate)
// {
// 	this->_AllBusiness = stockAccount._AllBusiness;
// }


CStockAccountNum::~CStockAccountNum()
{
}

string CStockAccountNum::GetLastError()
{
	return _LastError;
}

//���һ�β���
bool CStockAccountNum::DoBusiness(const Business& mbusiness)
{
	//�����Ʊ
	if (mbusiness._Price * mbusiness._Volume < _CurrentAvailableCapital && mbusiness._Volume > 0.0)
	{
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_OwnStock = _OwnStock + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//������Ʊ
	else if (_OwnStock+mbusiness._Volume > 0  && mbusiness._Volume < 0.0)
	{
		_CurrentAvailableCapital = _CurrentAvailableCapital - mbusiness._Price * mbusiness._Volume;
		_OwnStock = _OwnStock + mbusiness._Volume;
		_AllBusiness.push_back(mbusiness);
		return true;
	}
	//�����Ʊ����
	if (mbusiness._Price * mbusiness._Volume > _CurrentAvailableCapital && mbusiness._Volume > 0.0)
	{
		_LastError = "Lack of Available Capital.";
	}
	//������Ʊ����
	if (_OwnStock + mbusiness._Volume > 0  && mbusiness._Volume < 0.0)
	{
		_LastError = "Lack of Operable Stock.";
	}

	return false;
}

//��õ�ǰ�Ĳ�λ��������[0��1]֮��
double CStockAccountNum::GetPosition(const double& currentPrice)
{
	if (_CurrentAvailableCapital + _OwnStock * currentPrice > 0)
	{
		return  _OwnStock * currentPrice / (_CurrentAvailableCapital + _OwnStock * currentPrice);
	}
	else
	{
		_LastError = "Assets is negative.";//�����ʲ��Ǹ�ֵ
		return -1;
	}
}
//
//�������й�Ʊ
bool CStockAccountNum::SellOutAll(const double& currentPrice)
{
	Business tempBusiness;
	tempBusiness._Price = currentPrice;
	tempBusiness._Volume = _OwnStock;

	_CurrentAvailableCapital = _CurrentAvailableCapital + currentPrice * _OwnStock;
	_OwnStock = _OwnStock - _OwnStock;
	_AllBusiness.push_back(tempBusiness);
	return true;
}
//
//ȫ������
bool CStockAccountNum::AllIn(const double& currentPrice)
{
	Business tempBusiness;
	tempBusiness._Price = currentPrice;
	tempBusiness._Volume = _CurrentAvailableCapital / currentPrice;

	_CurrentAvailableCapital = 0;
	_OwnStock = _CurrentAvailableCapital / currentPrice;

	_AllBusiness.push_back(tempBusiness);
	return true;
}
//
//�޸Ĳ�λ��Ŀ���λ
bool CStockAccountNum::ChangeStockPosition(const Business& mbusiness, const double& tagePosition)
{
	if (tagePosition>1.0 || tagePosition < 0)
	{
		_LastError = "Tage position error.";
		return true;
	}
	if (mbusiness._Price <= 0)
	{
		_LastError = "Price error.";
		return false;
	}
	if (tagePosition - GetPosition(mbusiness._Price) <= 0.02 && tagePosition - GetPosition(mbusiness._Price) >= -0.02)
	{
		return true;
	}

	double currentPosition = GetPosition(mbusiness._Price);
	//����Ŀ���λ�Ǵ��ڻ���С�ڵ�ǰ��λ������һ���ļ��㹫ʽ
	Business tempBusiness;
	tempBusiness._Price = mbusiness._Price;
	tempBusiness._Date = mbusiness._Date;
	tempBusiness._Volume = (tagePosition - currentPosition) * GetCurrentAssets(mbusiness._Price) / mbusiness._Price;

	_OwnStock += tempBusiness._Volume;
	_CurrentAvailableCapital -= (tempBusiness._Volume * tempBusiness._Price);
	_AllBusiness.push_back(tempBusiness);

	return true;
}

double CStockAccountNum::GetProceedsRate(double currentPrice)
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

double CStockAccountNum::GetCurrentAssets(double currentPrice)
{
	return _CurrentAvailableCapital + currentPrice * _OwnStock;
}

list<Business> CStockAccountNum::GetAllBusiness()
{
	return _AllBusiness;
}

bool CStockAccountNum::InitAccount(double InitCapital)
{
	_LastError = "";
	_CurrentAvailableCapital = _InitCapital;
	_OwnStock = 0;
	_AllBusiness.clear();
	return true;
}

void CStockAccountNum::PrintAllBusiness(const string& filePath)
{
	fstream outfile(filePath, ios::out | ios::app);
	if (outfile)
	{
		if (_AllBusiness.empty())
		{
			return;
		}
		double currentPrice = _AllBusiness.rbegin()->_Price;
		outfile << "Date,Price,Volume,AvailableCapital,_OwnStock,Asset"<<endl;
		for (list<Business>::iterator ite = _AllBusiness.begin(); ite != _AllBusiness.end();ite++)
		{
			outfile << ite->_Date << "," 
				<< ite->_Price << "," 
				<< ite->_Volume << ","
				<< _CurrentAvailableCapital << ","
				<< _OwnStock << ","
				<<GetCurrentAssets(currentPrice) << endl;
		}
		outfile.close();
	}
}


