
#ifndef _STOCKACCOUNTNUM_H
#define _STOCKACCOUNTNUM_H
#include <list>
#include <string>
#include <fstream>
using namespace std;
#pragma once

//һ�ν���
struct Business
{
	//bool _IsBay;//�ǲ�������
	string _Date;
	double _Volume;//��������
	double _Price;//���׼۸�
};
//��ֻ��һ����Ʊ����
class CStockAccountNum
{
public:
	CStockAccountNum(const string& StockDate, const double& InitCapital);
	//CStockAccountNum(const CStockAccountNum& stockAccount);
	~CStockAccountNum();

private:
	CStockAccountNum();
public:
	//��ʼ���˺�
	bool InitAccount(double InitCapital);
	//����һ�β����������������
	bool DoBusiness(const Business& mbusiness);
	//�޸Ĳ�λ��Ŀ���λ
	bool ChangeStockPosition(const Business& mbusiness, const double& tagePosition);
	//��������
	bool SellOutAll(const double& currentPrice);
	//��ȡ��ǰ��λ
	double GetPosition(const double& currentPrice);
	//ȫ������
	bool AllIn(const double& currentPrice);
	//��ȡ��ǰ�ʲ���ֵ
	double GetCurrentAssets(double currentPrice);
	//��ȡ��ǰ������ ��ֵΪӯ������ֵΪ����
	double GetProceedsRate(double currentPrice);
	//������һ�γ�����Ϣ
	string GetLastError();
	//
	list<Business> GetAllBusiness();
	//
	void PrintAllBusiness(const string& filePath);
private:
	//��ʼ����������
	double _InitCapital;//��ʼ���ʽ�
	string _OwnStockDate;//�����Ĺ�Ʊ����
	//���̱������ɱ�
	double _CurrentAvailableCapital;//��ǰ�Ŀ����ʽ�
	double _OwnStock;//ӵ�еĹ�Ʊ����
	//���̼�¼
	list<Business> _AllBusiness;//�����������в���
	string _LastError;
};

#endif


