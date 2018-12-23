#include "stdafx.h"
#include <sstream>
#include <iostream>
#include "CGroupDataByBAR.h"
#include "CommonFunction.h"
#include "Number/IndexNumber.h"

using namespace std;
CGroupDataByBAR::CGroupDataByBAR()
{
}


CGroupDataByBAR::~CGroupDataByBAR()
{
}

//
//
//
bool CGroupDataByBAR::GetGroupData()
{
	//保存读出的全部数据
	vector<vector<string>> resivedata;
	ShowDebugInformationInCon("in CGroupData By BAR.");
	//从剪切板读取数据
	CNumberManager mmer;
	resivedata = mmer.ReadDataFromClickBoardAndCutRanks();
	if (resivedata.size()<3)
	{
		ShowDebugInformationInCon("Datasize is too small.");
		return false;
	}
	ShowDebugInformationInCon( "Check Resivedata size pass." );

	//找到目标列的位置
	int index = 0;
	Return_IF_FALSE(FindTageIndex("BAR", resivedata[0], index))
	ShowDebugInformationInCon("Find tage index.");
	string DebugInformation;
	TO_STRING(<< "CloumnString=" <<" "<<index, DebugInformation);
	ShowDebugInformationInCon(DebugInformation);

	//增加分组列,resivedata[0]表示第一行数据，为列标题
	vector<string> vCloumnString = resivedata[0];
	vCloumnString.push_back("BARChange");
	vector<vector<string>> SaveData;
	SaveData.push_back(vCloumnString);
	vCloumnString.clear();
	ShowDebugInformationInCon("Try to creat data.");

	//生成新列的数据
	Return_IF_FALSE(CreatIndexData(resivedata, index, SaveData));
	ShowDebugInformationInCon("Try to SaveDataToClickBoard.");
	Return_IF_FALSE(mmer.SaveDataToClickBoard(SaveData));
	ShowDebugInformationInCon( "Add cloumnstring success." );

	return false;
}
//////////////////////////////////////////////////////////////////////////
//
//
//
bool CGroupDataByBAR::FindTageIndex(const string& tageIndexString, vector<string> vCloumnString, int& index)
{
	for (index = 0; index < vCloumnString.size(); index++)
	{
		string debugInformation;
		TO_STRING(<<"vCloumnString[index]=" << vCloumnString[index], debugInformation);
		ShowDebugInformationInCon(debugInformation);
		TO_STRING( <<"tageIndexString=" << tageIndexString, debugInformation);
		ShowDebugInformationInCon(debugInformation);
		TO_STRING(<< "tageIndexString=" << tageIndexString, debugInformation);
		ShowDebugInformationInCon(debugInformation);

		if (vCloumnString[index] == tageIndexString)
		{
			ShowDebugInformationInCon("tageIndexString was found.");
			break;
		}
		else if (index == vCloumnString.size() - 1)
		{
			ShowDebugInformationInCon("Can not find index.");
			return false;
		}
	}
	ShowDebugInformationInCon("Find index success.");
	return true;
}
//////////////////////////////////////////////////////////////////////////
//
//
//resivedata，所有数据
bool CGroupDataByBAR::CreatIndexData(const vector<vector<string>>& resivedata, const int index, vector<vector<string>>& SaveData)
{
	//输入参数检查
	if (resivedata.size() < 3)
	{
		ShowDebugInformationInCon("Fail :Resivedata is too small.");
		return false;
	}
	if (index >= resivedata[0].size())
	{
		ShowDebugInformationInCon("Fail :Index is out of range.");
		return false;
	}

	vector<string> vCloumnString;
	//分组计数,从1开始计数
	int groupindexNum = 1;
	//Fronti用于标记目标数据是否已经反转，开始为1或者-1
	float Fronti = resivedata[1][index].c_str() < 0 ? -1:1;

	//计算进行分组
	for (int i = 1; i < resivedata.size() - 1; i++)
	{
		vCloumnString = resivedata[i];
		float tempResiveData = atof(resivedata[i][index].c_str());
		if (tempResiveData * Fronti < 0)
		{
			ShowDebugInformationInCon("Date symbol change.");
			Fronti = tempResiveData;
			groupindexNum++;
		}
		stringstream stream;
		stream << groupindexNum;
		string tempdata = stream.str();
		vCloumnString.push_back(tempdata);
		SaveData.push_back(vCloumnString);
	}
	return true;
}
