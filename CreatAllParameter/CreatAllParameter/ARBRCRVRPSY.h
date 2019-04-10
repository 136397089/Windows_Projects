#pragma once
#include <queue>
#include <list>
#include "Number/NumberBase.h"

struct ARBRCRVRPSY
{
	StockDataType AR;
	StockDataType BR;
	StockDataType VR;
	StockDataType CR;
	StockDataType CRMa1;
	StockDataType CRMa2;
	StockDataType CRMa3;
	StockDataType CRMa4;
	StockDataType PSY;
};

class CArBrCrVrPsy
{
public:
	CArBrCrVrPsy();
	~CArBrCrVrPsy();
	bool GetNextArBrVrPsy(const DatePriceData& OneDayData, ARBRCRVRPSY& mLastData);

	StockDataType GetAR();
	StockDataType GetBR();
	StockDataType GetCR();
	StockDataType GetCRMA(unsigned int maPara,list<StockDataType>& crmalist);
	StockDataType GetVR();
	StockDataType GetPSY();

	list<DatePriceData> TemporaryStorageData;
	unsigned int VRParameter;
	unsigned int ARBRParameter;
	unsigned int CRParameter;
	unsigned int PSYParameter;

	unsigned int CR_Close_Weight;
	unsigned int CR_Open_Weight;
	unsigned int CR_High_Weight;
	unsigned int CR_Low_Weight;
	unsigned int CR_MA1Para;
	unsigned int CR_MA2Para;
	unsigned int CR_MA3Para;
	unsigned int CR_MA4Para;
	list<StockDataType> CR_List;
	list<StockDataType> CRMa1_List;
	list<StockDataType> CRMa2_List;
	list<StockDataType> CRMa3_List;
	list<StockDataType> CRMa4_List;

};





/*
AR���㹫ʽ
AR=((H - O)26��֮��/(O - L)26��֮��) * 100
H������֮��߼�
L������֮��ͼ�
O������֮���̼�

BR���㹫ʽ
BR=((H - PC)26��֮��/(PC - L)26��֮��) * 100
H������֮��߼�
L������֮��ͼ�
PC������֮���̼�

CR���㹫ʽ
CR��N�գ�=P1��P2��100
ʽ�У�P1=����H��YM������ʾN�������෽�������ܺͣ�P2=����YM��L������ʾN�������շ��������ܺ͡�
H��ʾ���յ���߼ۣ�L��ʾ���յ���ͼ�YM��ʾ���գ���һ�������գ����м�ۡ�
CR���㹫ʽ�е��м����ʵҲ��һ��ָ�꣬����ͨ�������գ�YM�����׵���߼ۡ���ͼۡ����̼ۺ����̼۽��м�Ȩƽ�����õ��ģ���ÿ���۸��Ȩ�ؿ�����Ϊ��ѡ����
�Ƚϳ��õ��м�ۼ��㷽�������֣�
1��M=��2C+H+L����4
2��M=��C+H+L+O����4
3��M=��C+H+L����3
4��M=��H+L����2

VR���㹫ʽ
1��24���������ǹɼ�������һ��ĳɽ�������ΪAV����24���ڵ�AV�ܺ���Ӻ��ΪAVS��
2��24���������ǹɼ��µ���һ��ĳɽ�������ΪBV����24���ڵ�BV�ܺ���Ӻ��ΪBVS��
3��24���������ǹɼ۲��ǲ���������һ��ĳɽ�������ΪCV����24���ڵ�CV�ܺ���Ӻ��ΪCVS��
4�� 24�쿪ʼ���㣺
VR=��AVS+1/2CVS��/��BVS+1/2CVS��
5�� ����������24������޸ģ��������ڲ���С��12�����򣬲������������������ƫ�

PSY���㹫ʽ
1.PSY=N������������/N*100
2.PSYMA=PSY��M�ռ��ƶ�ƽ��
3.����N����Ϊ12�գ�����M����Ϊ6��
*/