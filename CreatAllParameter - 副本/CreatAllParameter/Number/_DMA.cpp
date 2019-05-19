#include "stdafx.h"
#include <numeric>
#include "_DMA.h"

CDMA::CDMA()
{
	Paremeter50 = 50;
	Paremeter10 = 10;
	closelyData50.clear();
}

CDMA::~CDMA()
{
}

// void CDMA::UpdateFileColumn(string strFolderPath)
// {
// 	WIN32_FIND_DATA p;
// 	string mFilePath = strFolderPath + "\\*.csv";
// 	//LPCWSTR findPath = mFilePath.c_str();
// 	HANDLE h = FindFirstFile(mFilePath.c_str(), &p);
// 	//统计文件夹下的文件数
// 	int FileNumber = 0;
// 	while (FindNextFile(h, &p))
// 	{
// 		FileNumber++;
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
// 		VStockData vDMAValue, vAMAValue;
// 		datas = ReadColumnStringFormFile(strFolderPath + "\\" + filename, File_Close_INDEX);
// 		vVolume = ReadColumnStringFormFile(strFolderPath + "\\" + filename, File_Volume_INDEX);
// 		// 		reverse(datas.begin(),datas.end()); // 反转数据顺序
// 		DMA_DATA mDMAData;
// 		VStockData::iterator vVolumeite = vVolume.begin();
// 		for (VStockData::iterator ite = datas.begin(); ite != datas.end(); ite++)
// 		{
// 			DatePriceData OneDayData;
// 			OneDayData._Close = *ite;
// 			if (*vVolumeite != 0)
// 			{
// 				GetNextDMA(OneDayData, mDMAData);
// 				vDMAValue.push_back(mDMAData._DDD);
// 				vAMAValue.push_back(mDMAData._AMA);
// 			}
// 			else
// 			{
// 				vDMAValue.push_back(mDMAData._DDD);
// 				vAMAValue.push_back(mDMAData._AMA);
// 			}
// 			vVolumeite++;
// 		}
// 		ReSavefileColumn(strFolderPath + "\\" + filename, vDMAValue, DMA_D);
// 		ReSavefileColumn(strFolderPath + "\\" + filename, vAMAValue, DMA_A);
// 		FileIndex++;
// 		system("cls");
// 		cout << strFolderPath << ":" << FileIndex * 100 / FileNumber << "%" << endl;
// 	} while (FindNextFile(h, &p));
// }
// 
// void CDMA::UpdatePathFilesRanks(string strPath)
// {
// 
// }
// 
// void CDMA::UpdateFileRanks(string strfilename)
// {
// 
// }

void CDMA::GetNextDMA(const DatePriceData& OneDayData, DMA_DATA& Dma)
{
	closelyData50.push_back(OneDayData._Close);
	if (closelyData50.size() < Paremeter50)
	{
		list<float>::iterator beginite = closelyData50.begin();
		float addin50 = accumulate(closelyData50.begin(), closelyData50.end(), 0.0f);
		Dma._AMA = 0;
		Dma._DDD = 0;
	}
	else if (closelyData50.size() == Paremeter50)
	{
		list<float>::iterator beginite = closelyData50.begin();
		list<float>::iterator midite = next(beginite,40);
		float addin50 = accumulate(closelyData50.begin(), closelyData50.end(), 0.0f);
		float addin10 = accumulate(midite, closelyData50.end(), 0.0f );
		closelyData50.pop_front();
		Dma._DDD = addin10 / Paremeter10 - addin50 / Paremeter50;
		frontDMAData.push_back(Dma._DDD);
		
		if (frontDMAData.size() >= Paremeter10)
		{
			list<float>::reverse_iterator dmaite = next(frontDMAData.rbegin(), Paremeter10);
			float dmaAddIn10 = accumulate(frontDMAData.rbegin(), dmaite, 0.0f);
			Dma._AMA = dmaAddIn10 / Paremeter10;
			frontDMAData.pop_front();
		}

	}
	return;
}

void CDMA::Inition()
{
	closelyData50.clear();
	frontDMAData.clear();
}
