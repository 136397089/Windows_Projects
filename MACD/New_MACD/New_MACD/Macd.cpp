#include "StdAfx.h"
#include "Macd.h"

CMacdCalculater::CMacdCalculater(void)
{
}

CMacdCalculater::~CMacdCalculater(void)
{
}

void CMacdCalculater::UpdateFile( string strPath )
{
	WIN32_FIND_DATA p;
	HANDLE h = FindFirstFile(strPath.c_str(),&p);
	float FileNumber = 0;
	while(FindNextFile(h,&p))
	{
		FileNumber ++;
	}
	string tempPath = strPath+"\\*.csv";
	h = FindFirstFile(tempPath.c_str(),&p);

	if (NULL == h)
	{
		return;
	}
	float FileIndex = 0;
	do 
	{
		string filename(p.cFileName);
		vector<double>datas,vVolume;
		vector<double> vMACDValue,Ma12,Ma26,Diff,DEA;
		datas = ReadStringFormFile(strPath+"\\"+filename,File_Close_INDEX);
		vVolume = ReadStringFormFile(strPath+"\\"+filename,File_Volume_INDEX);
// 		reverse(datas.begin(),datas.end());
		Macd mMacdData;
		vector<double>::iterator vVolumeite = vVolume.begin();
		for (vector<double>::iterator ite = datas.begin() ;ite!=datas.end() ;ite++)
		{
			if (*vVolumeite != 0)
			{
				mMacdData = GetNextMacd(*ite,mMacdData);
				vMACDValue.push_back(mMacdData.bar);
				Ma12.push_back(mMacdData.m12);
				Ma26.push_back(mMacdData.m26);
				Diff.push_back(mMacdData.diff);
				DEA.push_back(mMacdData.dea);
			}
			else
			{
				vMACDValue.push_back(mMacdData.bar);
				Ma12.push_back(mMacdData.m12);
				Ma26.push_back(mMacdData.m26);
				Diff.push_back(mMacdData.diff);
				DEA.push_back(mMacdData.dea);
			}
			vVolumeite++;
		}
		ReSavefileColumn(strPath+"\\"+filename,Ma12,MACD_MA12);
		ReSavefileColumn(strPath+"\\"+filename,Ma26,MACD_MA26);
		ReSavefileColumn(strPath+"\\"+filename,Diff,MACD_DIFF);
		ReSavefileColumn(strPath+"\\"+filename,DEA,MACD_DEA);
		ReSavefileColumn(strPath+"\\"+filename,vMACDValue,MACD_BAR);
		FileIndex++;
		system("cls");
		cout<<strPath<<":"<<FileIndex*100/FileNumber<<"%"<<endl;
	}while(FindNextFile(h,&p));
}






Macd CMacdCalculater::GetNextMacd( float closeData, Macd mMacd )
{
	Macd TempMacd;
	TempMacd = mMacd;
	TempMacd.m12 = mMacd.m12 + (closeData - mMacd.m12) * 2.0f/13.0f;
	TempMacd.m26 = mMacd.m26 + (closeData - mMacd.m26) * 2.0f/27.0f;
	TempMacd.diff = TempMacd.m12 - TempMacd.m26;
	TempMacd.dea = mMacd.dea*0.8f + TempMacd.diff*0.2f;
	TempMacd.bar = 2.0f * (-TempMacd.dea + TempMacd.diff);
	return TempMacd;
}

