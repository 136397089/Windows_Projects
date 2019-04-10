#pragma once
#include "Number\NumberBase.h"

struct  Dmi
{
	StockDataType _PDI;
	StockDataType _NDI;
	StockDataType _ADX;
	StockDataType _ADXR;
	Dmi()
	{
		_PDI = 0.0f;
		_NDI = 0.0f;
		_ADX = 0.0f;
		_ADXR = 0.0f;
	}
};
struct DMTR
{
	StockDataType PDM;
	StockDataType NDM;
	StockDataType TR;
	StockDataType ADX;
	DMTR()
	{
		PDM = 0.0f;
		NDM = 0.0f;
		TR = 0.0f;
		ADX = 0.0f;
	}
};
class CDMI// :public CNumberManager
{
public:
	CDMI();
	~CDMI();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����MACDֵ(���ݰ�����)
	//strPath:Ŀ¼·��
	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdateFileColumn(string strFolderPath);
// 	//////////////////////////////////////////////////////////////////////////
// 	//����Ŀ¼������CSV�ļ����MACDֵ(����������)
// 	//strPath:Ŀ¼·��
// 	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdatePathFilesRanks(string strPath);
// 	//////////////////////////////////////////////////////////////////////////
// 	//���µ���CSV�ļ����MACDֵ(����������)
// 	//strPath:�ļ�·��
// 	//////////////////////////////////////////////////////////////////////////
// 	virtual void UpdateFileRanks(string strfilename);
	//////////////////////////////////////////////////////////////////////////
	//ͨ��ǰ�յ�MACD��ý��յ�MACDֵ
	//closeData:���̼�
	//mMacd:ǰ�յ�MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextDMI(const DatePriceData& OneDayData, Dmi& mDmi);

	bool Inition();
private:
	DatePriceData _FrontPriceData;
	//list<DMTR> _vFrontDMTRList;
	Dmi _FrontDMI;
	DMTR _FrontAvgDMTR;
	StockDataType M1;
	StockDataType M2;
	StockDataType _FrontDay;//֮ǰ�Ѿ��������ݵ�����
	StockDataType _FrontADXDay;
};

