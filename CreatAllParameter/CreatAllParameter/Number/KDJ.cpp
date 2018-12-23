#include "StdAfx.h"
#include <valarray>
#include <cmath>
#include "KDJ.h"

using namespace std;
CKDJ::CKDJ(void)
{
	_Nday = 9*5;
	_M1 = 3*5;
	_M2 = 3*5;
}

CKDJ::~CKDJ(void)
{
}

bool CKDJ::GetNextKDJ(const DatePriceData& OneDayData, KDJ& mFrontKDJ)
{
	if (_lHigh.size() != _lLow.size() || _lHigh.size() != _lClose.size())
		return false;
	if (_Nday == 0 || _M1 == 0 || _M2 == 0)
		return false;
	//
	StaticSizePush(OneDayData._High, _Nday, _lHigh);
	StaticSizePush(OneDayData._Low, _Nday, _lLow);
	StaticSizePush(OneDayData._Close, _Nday, _lClose);

	if (_lHigh.size() < _Nday)
	{
		mFrontKDJ.K_OF_KDJ = 100;
		mFrontKDJ.D_OF_KDJ = 100;
		mFrontKDJ.J_OF_KDJ = 100;
		return true;
	}
	//找到最大值和最小值
	float HighestN = *max_element(_lHigh.begin(), _lHigh.end());
	float LowestN = *min_element(_lLow.begin(), _lLow.end());
	KDJ m_kdj = mFrontKDJ;
	//计算KDJ
	float RSV = (OneDayData._Close - LowestN) / (HighestN - LowestN) * 100;
	mFrontKDJ.K_OF_KDJ = RSV / _M1 + (_M1 - 1) * m_kdj.K_OF_KDJ / _M1;
	mFrontKDJ.D_OF_KDJ = mFrontKDJ.K_OF_KDJ / _M2 + (_M2 - 1) * m_kdj.D_OF_KDJ / _M2;
	mFrontKDJ.J_OF_KDJ = 3 * mFrontKDJ.K_OF_KDJ - 2 * mFrontKDJ.D_OF_KDJ;
	//KDJ参数有门限在0到100之间
	if (mFrontKDJ.K_OF_KDJ > 100)
		mFrontKDJ.K_OF_KDJ = 100;
	if (mFrontKDJ.D_OF_KDJ > 100)
		mFrontKDJ.D_OF_KDJ = 100;
	if (mFrontKDJ.J_OF_KDJ > 100)
		mFrontKDJ.J_OF_KDJ = 100;
	if (mFrontKDJ.K_OF_KDJ < 0)
		mFrontKDJ.K_OF_KDJ = 0;
	if (mFrontKDJ.D_OF_KDJ < 0)
		mFrontKDJ.D_OF_KDJ = 0;
	if (mFrontKDJ.J_OF_KDJ < 0)
		mFrontKDJ.J_OF_KDJ = 0;


	return true;
}


void CKDJ::UpdatePathFilesRanks(string strFolderPath)
{
	WIN32_FIND_DATA p;
	string mFilePath = strFolderPath + "\\*.csv";
	//LPCWSTR filePath = mFilePath;
	HANDLE hFileHandle = FindFirstFile(mFilePath.c_str(), &p);
	RETURN_IFNULL(hFileHandle);
	//统计文件夹下的文件数
	float FileNumber = 0;
	while (FindNextFile(hFileHandle, &p))
	{
		FileNumber++;
	}
	hFileHandle = FindFirstFile(mFilePath.c_str(), &p);
	RETURN_IFNULL(hFileHandle);

	//遍历所有文件
	float FileIndex = 0;
	do
	{
		string filename(p.cFileName);
		RunTread(strFolderPath + "\\" + filename);
		FileIndex++;
		system("cls");
		string information;
		TO_STRING(<< "KDJ:" << strFolderPath << ":" << FileIndex * 100 / FileNumber << "%", information);
		ShowDebugInformationInCon(information);
	} while (FindNextFile(hFileHandle, &p));

}

void CKDJ::UpdateFileRanks(string strfilename)
{

	VStockData vCloseDatas,vHigh,vLow , vVolume;//文件读取的数据
	VStockData temphigh9, templow9;			//
	VStockData vK, vD, vJ;//计算结果
	vCloseDatas = ReadRanksStringFormFile(strfilename, File_Close_INDEX);
	vHigh = ReadRanksStringFormFile(strfilename, File_High_INDEX);
	vLow = ReadRanksStringFormFile(strfilename, File_Low_INDEX);
	vVolume = ReadRanksStringFormFile(strfilename, File_Volume_INDEX);
	if (vCloseDatas.size()<10 || vHigh.size()<10 || vLow.size()<10 || vVolume.size()<10)
	{
		return;
	}
	VStockData::iterator vCloseDatasbegin = vCloseDatas.begin();
	VStockData::iterator vHighbegin = vHigh.begin();
	VStockData::iterator vLowbegin = vLow.begin();
	VStockData::iterator vVolumebegin = vVolume.begin();
	VStockData::iterator vVolumeite = vVolume.begin();
// 	reverse(vCloseDatas.begin(), vCloseDatas.end());
// 	reverse(vHigh.begin(), vHigh.end());
// 	reverse(vLow.begin(), vLow.end());
// 	reverse(vVolume.begin(), vVolume.end());
	KDJ mKDJData;
	while (vCloseDatasbegin != vCloseDatas.end()
		&& vHighbegin != vHigh.end()
		&& vLowbegin != vLow.end()
		&& vVolumebegin != vVolume.end())
	{
		DatePriceData oneDayData;
		oneDayData._Close = *vCloseDatasbegin;
		oneDayData._High = *vHighbegin;
		oneDayData._Low = *vLowbegin;
		//获得之前9天数据
		if (*vVolumeite != 0)
		{
			//计算KDJ
			GetNextKDJ(oneDayData, mKDJData);
			vK.push_back(mKDJData.K_OF_KDJ);
			vD.push_back(mKDJData.D_OF_KDJ);
			vJ.push_back(mKDJData.J_OF_KDJ);
		}
		else//如果成交量为零，为停盘数据
		{
			vK.push_back(mKDJData.K_OF_KDJ);
			vD.push_back(mKDJData.D_OF_KDJ);
			vJ.push_back(mKDJData.J_OF_KDJ);
		}
		//所有迭代器后移
		++vHighbegin;
		++vLowbegin;
		++vVolumebegin;
		++vCloseDatasbegin;
	}

	ReSavefileRanks(strfilename, vK, KDJ_K);
	ReSavefileRanks(strfilename, vD, KDJ_D);
	ReSavefileRanks(strfilename, vJ, KDJ_J);
}

void CKDJ::UpdateFileColumn(string strFolderPath)
{

}

void CKDJ::StaticSizePush(tySData mdata, float msize, list<tySData>& mList)
{
	mList.push_back(mdata);
	if (mList.size() > msize)
		mList.pop_front();
}

void CKDJ::Inition()
{
	_lHigh.clear();
	_lLow.clear();
	_lClose.clear();
}
