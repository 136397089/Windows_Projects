#include "stdafx.h"
#include "TrendAnal.h"
#include <algorithm>

CTrendAnal::CTrendAnal()
{
	beginIndex = 540;
}


CTrendAnal::~CTrendAnal()
{
}

bool CTrendAnal::GetTrand(
	const VStockData& vcloseData,
	const VStockData& vhighData,
	const VStockData& vlowData,
	const vector<string>& vTimeDay,
	TrendPere& OutputPara)
{
	if (vcloseData.size() != vhighData.size()
		|| vhighData.size() != vlowData.size()
		|| vlowData.size() != vTimeDay.size())
		return false;
	if (vcloseData.size() <= beginIndex)
		return false;

	VStockData::const_iterator maxIte = max_element(vhighData.begin() + beginIndex, vhighData.end());
	unsigned int maxDistence = distance(vhighData.begin(), maxIte);
	VStockData::const_iterator minIte = min_element(vlowData.begin() + beginIndex, vlowData.end());
	unsigned int minDistence = distance(vlowData.begin(), minIte);


	float maxPointUpLineSlope = -999;
	float maxPointDownLineSlope = 0;
	float maxPointUpLineP = 0;
	float maxPointDownLineP = 0;

	float minPointUpLineSlope = 999;
	float minPointDownLineSlope = 0;
	float minPointUpLineP = 0;
	float minPointDownLineP = 0;

	//计算最大斜率和最小斜率
	for (unsigned int index = 0; index < vcloseData.size(); ++index)
	{
		float tempSlope = 0;
		if (index > maxDistence)
		{
			if ((vhighData[index] - *maxIte) / (float)(index - maxDistence) > maxPointUpLineSlope)
			{
				maxPointUpLineSlope = (vhighData[index] - *maxIte) / (float)(index - maxDistence);
				maxPointUpLineP = (float)index;
			}
			if ((vlowData[index] - *maxIte) / (float)(index - maxDistence) < maxPointDownLineSlope)
			{
				maxPointDownLineSlope = (vlowData[index] - *maxIte) / (float)(index - maxDistence);
				maxPointDownLineP = (float)index;
			}
		}
		if (index > minDistence)
		{
			if (vhighData[index] - *minIte / (float)(index - minDistence) < minPointUpLineSlope)
			{
				minPointUpLineSlope = vhighData[index] - *minIte / (float)(index - minDistence);
				minPointUpLineP = (float)index;
			}	
			if ((vlowData[index] - *minIte) / (float)(index - minDistence) > minPointDownLineSlope)
			{
				minPointDownLineSlope = (vlowData[index] - *minIte) / (float)(index - minDistence);
				minPointDownLineP = (float)index;
			}
		}
	}

	if (LogFile)
	{
		LogFile << "MinPoint: " << vTimeDay[minDistence] << "---" << vlowData[minDistence] << endl;
		LogFile << "MaxPoint: " << vTimeDay[maxDistence] << "---" << vhighData[maxDistence] << endl;
		LogFile << "MinUpPoint:" << vTimeDay[minPointUpLineP] << "---" << vhighData[minPointUpLineP] << endl;
		LogFile << "MinDownPoint:" << vTimeDay[minPointDownLineP] << "---" << vlowData[minPointDownLineP] << endl;
		LogFile << "MaxUpPoint:" << vTimeDay[maxPointUpLineP] << "---" << vhighData[maxPointUpLineP] << endl;
		LogFile << "MaxDownPoint:" << vTimeDay[maxPointDownLineP] << "---" << vlowData[maxPointDownLineP] << endl;
	}
	return true;
}

bool CTrendAnal::Inition()
{
	LogFile.open("D:\\StockFile\\TrendLog.txt",fstream::app);

	return true;
}

bool CTrendAnal::UnInition()
{
	if (LogFile)
	{
		LogFile.close();
	}
	return true;
}
