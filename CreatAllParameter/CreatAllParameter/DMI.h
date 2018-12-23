#pragma once
#include "Number\IndexNumber.h"

struct  Dmi
{
	tySData _PDI;
	tySData _NDI;
	tySData _ADX;
	tySData _ADXR;
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
	tySData PDM;
	tySData NDM;
	tySData TR;
	tySData ADX;
	DMTR()
	{
		PDM = 0.0f;
		NDM = 0.0f;
		TR = 0.0f;
		ADX = 0.0f;
	}
};
class CDMI :
	public CNumberManager
{
public:
	CDMI();
	~CDMI();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����MACDֵ(���ݰ�����)
	//strPath:Ŀ¼·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ¼������CSV�ļ����MACDֵ(����������)
	//strPath:Ŀ¼·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath);
	//////////////////////////////////////////////////////////////////////////
	//���µ���CSV�ļ����MACDֵ(����������)
	//strPath:�ļ�·��
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
	//////////////////////////////////////////////////////////////////////////
	//ͨ��ǰ�յ�MACD��ý��յ�MACDֵ
	//closeData:���̼�
	//mMacd:ǰ�յ�MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextDMI(const DatePriceData& OneDayData, Dmi& mDmi);
private:
	DatePriceData _FrontPriceData;
	//list<DMTR> _vFrontDMTRList;
	Dmi _FrontDMI;
	DMTR _FrontAvgDMTR;
	tySData M1;
	tySData M2;
	tySData _FrontDay;//֮ǰ�Ѿ��������ݵ�����
	tySData _FrontADXDay;
};

