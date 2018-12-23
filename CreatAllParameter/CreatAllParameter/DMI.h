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
	//更新目录下所有CSV文件里的MACD值(数据按列排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的MACD值(天数按行排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath);
	//////////////////////////////////////////////////////////////////////////
	//更新单个CSV文件里的MACD值(天数按行排)
	//strPath:文件路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
	//////////////////////////////////////////////////////////////////////////
	//通过前日的MACD获得今日的MACD值
	//closeData:收盘价
	//mMacd:前日的MACD
	//////////////////////////////////////////////////////////////////////////
	bool GetNextDMI(const DatePriceData& OneDayData, Dmi& mDmi);
private:
	DatePriceData _FrontPriceData;
	//list<DMTR> _vFrontDMTRList;
	Dmi _FrontDMI;
	DMTR _FrontAvgDMTR;
	tySData M1;
	tySData M2;
	tySData _FrontDay;//之前已经加入数据的天数
	tySData _FrontADXDay;
};

