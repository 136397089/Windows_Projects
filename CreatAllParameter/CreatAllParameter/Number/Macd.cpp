#include "StdAfx.h"
#include <algorithm>
#include "Macd.h"



CMacdManager::CMacdManager(void)
{
}

CMacdManager::~CMacdManager(void)
{
}

// void CMacdManager::UpdateFileColumn( string strFolderPath )
// {
// 	WIN32_FIND_DATA p;
// 	string mFilePath = strFolderPath + "\\*.csv";
// 	//LPCWSTR findPath = mFilePath.c_str();
// 	HANDLE h = FindFirstFile(mFilePath.c_str(), &p);
// 	//统计文件夹下的文件数
// 	int FileNumber = 0;
// 	while(FindNextFile(h,&p))
// 	{
// 		FileNumber ++;
// 	}
// 
// 	h = FindFirstFile(mFilePath.c_str(), &p);
// 	if (NULL == h)
// 	{
// 		return;
// 	}
// 
// 	float FileIndex = 0;
// 	do 
// 	{
// 		string filename(p.cFileName);
// 		VStockData datas, vVolume;
// 		VStockData vMACDValue, Ma12, Ma26, Diff, DEA;
// 		datas = ReadColumnStringFormFile(strFolderPath+"\\"+filename,File_Close_INDEX);
// 		vVolume = ReadColumnStringFormFile(strFolderPath+"\\"+filename,File_Volume_INDEX);
// // 		reverse(datas.begin(),datas.end());
// 		Macd mMacdData;
// 		VStockData::iterator vVolumeite = vVolume.begin();
// 		for (VStockData::iterator ite = datas.begin(); ite != datas.end(); ite++)
// 		{
// 			DatePriceData OneDayData;
// 			OneDayData._Close = *ite;
// 			if (*vVolumeite != 0)
// 			{
// 				GetNextMacd(OneDayData, mMacdData);
// 				vMACDValue.push_back(mMacdData.bar);
// 				Ma12.push_back(mMacdData.m12);
// 				Ma26.push_back(mMacdData.m26);
// 				Diff.push_back(mMacdData.diff);
// 				DEA.push_back(mMacdData.dea);
// 			}
// 			else
// 			{
// 				vMACDValue.push_back(mMacdData.bar);
// 				Ma12.push_back(mMacdData.m12);
// 				Ma26.push_back(mMacdData.m26);
// 				Diff.push_back(mMacdData.diff);
// 				DEA.push_back(mMacdData.dea);
// 			}
// 			vVolumeite++;
// 		}
// 		FileIndex++;
// 		system("cls");
// 		cout<<strFolderPath<<":"<<FileIndex*100/FileNumber<<"%"<<endl;
// 	}while(FindNextFile(h,&p));
// }

bool CMacdManager::GetNextMacd(const DatePriceData& OneDayData, Macd& mMacd)
{
	Macd TempMacd;
	TempMacd = mMacd;
	mMacd.m12 = mMacd.m12 + (OneDayData._Close - mMacd.m12) * 2.0f / 13.0f;
	mMacd.m26 = mMacd.m26 + (OneDayData._Close - mMacd.m26) * 2.0f / 27.0f;
	mMacd.diff = TempMacd.m12 - TempMacd.m26;
	mMacd.dea = mMacd.dea*0.8f + TempMacd.diff*0.2f;
	mMacd.bar = 2.0f * (-TempMacd.dea + TempMacd.diff);
	return true;
}

// void CMacdManager::UpdatePathFilesRanks(string strFolderPath)
// {
// 	WIN32_FIND_DATA p;
// 	string mFilePath = strFolderPath + "\\*.csv";
// 	HANDLE hFileHandle = FindFirstFile(mFilePath.c_str(), &p);
// 	RETURN_IFNULL(hFileHandle);
// 	//统计文件夹下的文件数
// 	float FileNumber = 0;
// 	while (FindNextFile(hFileHandle, &p))
// 	{
// 		FileNumber++;
// 	}
// 	hFileHandle = FindFirstFile(mFilePath.c_str(), &p);
// 	RETURN_IFNULL(hFileHandle);
// 
// 	//遍历所有文件
// 	float FileIndex = 0;
// 	do
// 	{
// 		string filename(p.cFileName);
// 		RunTread(strFolderPath + "\\" + filename);
// 		FileIndex++;
// 		system("cls");
// 		cout <<"MACD :"<<strFolderPath << ":" << FileIndex * 100 / FileNumber << "%" << endl;
// 	} while (FindNextFile(hFileHandle, &p));
// 
// }

//////////////////////////////////////////////////////////////////////////
//更新CSV文件文件里的MACD值(天数按行排)
//strPath:文件路径
//////////////////////////////////////////////////////////////////////////
// void CMacdManager::UpdateFileRanks(string strfilename)
// {
// 	VStockData datas, vVolume;//文件读取的数据
// 	VStockData vMACDValue, Ma12, Ma26, Diff, DEA;//结果保存位置
// 	datas = ReadRanksStringFormFile(strfilename, File_Close_INDEX);
// 	vVolume = ReadRanksStringFormFile(strfilename, File_Volume_INDEX);
// 	// 		reverse(datas.begin(),datas.end());
// 	Macd mMacdData;
// // 	reverse(datas.begin(), datas.end());//反转数据顺序
// // 	reverse(vVolume.begin(), vVolume.end());
// 	VStockData::iterator vVolumeite = vVolume.begin();
// 	for (VStockData::iterator ite = datas.begin(); ite != datas.end(); ite++)
// 	{
// 		DatePriceData OneDayData;
// 		OneDayData._Close = *ite;
// 		if (*vVolumeite != 0)
// 		{
// 			GetNextMacd(OneDayData, mMacdData);
// 			vMACDValue.push_back(mMacdData.bar);
// 			Ma12.push_back(mMacdData.m12);
// 			Ma26.push_back(mMacdData.m26);
// 			Diff.push_back(mMacdData.diff);
// 			DEA.push_back(mMacdData.dea);
// 		}
// 		else
// 		{
// 			vMACDValue.push_back(mMacdData.bar);
// 			Ma12.push_back(mMacdData.m12);
// 			Ma26.push_back(mMacdData.m26);
// 			Diff.push_back(mMacdData.diff);
// 			DEA.push_back(mMacdData.dea);
// 		}
// 		vVolumeite++;
// 	}
// 	//ReSavefileRanks(strFolderPath + "\\" + filename, Ma12, MACD_MA12);
// 	//ReSavefileRanks(strFolderPath + "\\" + filename, Ma26, MACD_MA26);
// // 	reverse(Diff.begin(), Diff.end());
// // 	reverse(DEA.begin(), DEA.end());
// // 	reverse(vMACDValue.begin(), vMACDValue.end());
// }

