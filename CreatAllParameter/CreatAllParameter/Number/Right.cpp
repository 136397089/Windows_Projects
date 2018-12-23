#include "stdafx.h"
#include <numeric>
#include "Right.h"

CRight::CRight()
{
	closelyData50.clear();
}

CRight::~CRight()
{
}

void CRight::UpdateFileColumn(string strFolderPath)
{
	WIN32_FIND_DATA p;
	string mFilePath = strFolderPath + "\\*.csv";
	//LPCWSTR findPath = mFilePath.c_str();
	HANDLE h = FindFirstFile(mFilePath.c_str(), &p);
	//统计文件夹下的文件数
	int FileNumber = 0;
	while (FindNextFile(h, &p))
	{
		FileNumber++;
	}

	h = FindFirstFile(mFilePath.c_str(), &p);
	if (NULL == h)
	{
		return;
	}

	float FileIndex = 0;
	do
	{
		string filename(p.cFileName);
		VStockData datas, vVolume;
		VStockData vDMAValue, vAMAValue;
		datas = ReadColumnStringFormFile(strFolderPath + "\\" + filename, File_Close_INDEX);
		vVolume = ReadColumnStringFormFile(strFolderPath + "\\" + filename, File_Volume_INDEX);
		// 		reverse(datas.begin(),datas.end()); // 反转数据顺序
		DMA_DATA mDMAData;
		VStockData::iterator vVolumeite = vVolume.begin();
		for (VStockData::iterator ite = datas.begin(); ite != datas.end(); ite++)
		{
			if (*vVolumeite != 0)
			{
				GetNextDMA(*ite, mDMAData);
				vDMAValue.push_back(mDMAData._DDD);
				vAMAValue.push_back(mDMAData._AMA);
			}
			else
			{
				vDMAValue.push_back(mDMAData._DDD);
				vAMAValue.push_back(mDMAData._AMA);
			}
			vVolumeite++;
		}
		ReSavefileColumn(strFolderPath + "\\" + filename, vDMAValue, DMA_D);
		ReSavefileColumn(strFolderPath + "\\" + filename, vAMAValue, DMA_A);
		FileIndex++;
		system("cls");
		cout << strFolderPath << ":" << FileIndex * 100 / FileNumber << "%" << endl;
	} while (FindNextFile(h, &p));
}

void CRight::UpdatePathFilesRanks(string strPath)
{

}

void CRight::UpdateFileRanks(string strfilename)
{

}

void CRight::GetNextDMA(float CloseData, DMA_DATA Dma)
{
	if (closelyData50.size() < 50)
	{
		closelyData50.push_back(CloseData);
		Dma._AMA = 0;
		Dma._DDD = 0;
	}
	else if (closelyData50.size() >= 50)
	{
		closelyData50.pop_front();
		closelyData50.push_back(CloseData);
		list<float>::iterator beginite = closelyData50.begin();
		list<float>::iterator midite = next(beginite,30);
		float addin50 = accumulate(closelyData50.begin(), closelyData50.end(), 0.0f);
		float addin20 = accumulate(midite, closelyData50.end(), 0.0f );
		Dma._AMA = addin20 / 20;
		Dma._DDD = addin50 / 50;
	}
	return;
}
