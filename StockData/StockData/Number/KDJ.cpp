#include "StdAfx.h"
#include <valarray>
#include <cmath>
#include "KDJ.h"

using namespace std;
CKDJIndexNumber::CKDJIndexNumber(void)
{
}

CKDJIndexNumber::~CKDJIndexNumber(void)
{
}

KDJ CKDJIndexNumber::GetNextKDJ( float closeData, vector<float> High,vector<float> Low ,KDJ mFrontKDJ)
{
	float H9 = *max_element(High.begin(),High.end());
	float L9 = *min_element(Low.begin(),Low.end());
	KDJ m_kdj;
	if (L9 == H9) 
	{
		return m_kdj;
	}

	float RSV = (closeData - L9)*100/(H9-L9);
	m_kdj.K_OF_KDJ = RSV/3+2*mFrontKDJ.K_OF_KDJ/3;
	m_kdj.D_OF_KDJ = m_kdj.K_OF_KDJ/3+2*mFrontKDJ.D_OF_KDJ/3;
	m_kdj.J_OF_KDJ = 3*m_kdj.D_OF_KDJ-2*m_kdj.K_OF_KDJ;

	return m_kdj;
}


void CKDJIndexNumber::UpdatePathFilesRanks(string strFolderPath)
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

void CKDJIndexNumber::UpdateFileRanks(string strfilename)
{

	vector<tySData>vCloseDatas,vHigh,vLow , vVolume;//文件读取的数据
	vector<tySData> temphigh9, templow9;			//
	vector<tySData> vK, vD, vJ;//计算结果
	vCloseDatas = ReadRanksStringFormFile(strfilename, File_Close_INDEX);
	vHigh = ReadRanksStringFormFile(strfilename, File_High_INDEX);
	vLow = ReadRanksStringFormFile(strfilename, File_Low_INDEX);
	vVolume = ReadRanksStringFormFile(strfilename, File_Volume_INDEX);
	if (vCloseDatas.size()<10 || vHigh.size()<10 || vLow.size()<10 || vVolume.size()<10)
	{
		return;
	}
	vector<tySData>::iterator vCloseDatasbegin = vCloseDatas.begin();
	vector<tySData>::iterator vHighbegin = vHigh.begin();
	vector<tySData>::iterator vHighend = vHighbegin + 8;
	vector<tySData>::iterator vLowbegin = vLow.begin();
	vector<tySData>::iterator vLowend = vLowbegin + 8;
	vector<tySData>::iterator vVolumebegin = vVolume.begin();
	vector<tySData>::iterator vVolumeite = vVolume.begin();
// 	reverse(vCloseDatas.begin(), vCloseDatas.end());
// 	reverse(vHigh.begin(), vHigh.end());
// 	reverse(vLow.begin(), vLow.end());
// 	reverse(vVolume.begin(), vVolume.end());
	KDJ mKDJData;
	while (vCloseDatasbegin != vCloseDatas.end()
		&& vHighend != vHigh.end()
		&& vLowend != vLow.end()
		&& vVolumebegin != vVolume.end())
	{
		//获得之前9天数据
		copy(vHighbegin, vHighend, back_inserter(temphigh9));
		copy(vLowbegin, vLowend, back_inserter(templow9));
		if (*vVolumeite != 0)
		{
			//计算KDJ
			mKDJData = GetNextKDJ(*vCloseDatasbegin, temphigh9, templow9, mKDJData);
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
		vHighbegin++; 
		vHighend++; 
		vLowbegin++; 
		vLowend++;
		vVolumebegin++;
		vCloseDatasbegin++;
	}
// 	reverse(Diff.begin(), Diff.end());
// 	reverse(DEA.begin(), DEA.end());
// 	reverse(vMACDValue.begin(), vMACDValue.end());
	ReSavefileRanks(strfilename, vK, KDJ_K);
	ReSavefileRanks(strfilename, vD, KDJ_D);
	ReSavefileRanks(strfilename, vJ, KDJ_J);
}

void CKDJIndexNumber::UpdateFileColumn(string strFolderPath)
{

}
