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
	//���������ȫ������
	vector<vector<string>> resivedata;
	ShowDebugInformationInCon("in CGroupData By BAR.");
	//�Ӽ��а��ȡ����
	CNumberManager mmer;
	resivedata = mmer.ReadDataFromClickBoardAndCutRanks();
	if (resivedata.size()<3)
	{
		ShowDebugInformationInCon("Datasize is too small.");
		return false;
	}
	ShowDebugInformationInCon( "Check Resivedata size pass." );

	//�ҵ�Ŀ���е�λ��
	int index = 0;
	Return_IF_FALSE(FindTageIndex("BAR", resivedata[0], index))
	ShowDebugInformationInCon("Find tage index.");
	string DebugInformation;
	TO_STRING(<< "CloumnString=" <<" "<<index, DebugInformation);
	ShowDebugInformationInCon(DebugInformation);

	//���ӷ�����,resivedata[0]��ʾ��һ�����ݣ�Ϊ�б���
	vector<string> vCloumnString = resivedata[0];
	vCloumnString.push_back("BARChange");
	vector<vector<string>> SaveData;
	SaveData.push_back(vCloumnString);
	vCloumnString.clear();
	ShowDebugInformationInCon("Try to creat data.");

	//�������е�����
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
//resivedata����������
bool CGroupDataByBAR::CreatIndexData(const vector<vector<string>>& resivedata, const int index, vector<vector<string>>& SaveData)
{
	//����������
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
	//�������,��1��ʼ����
	int groupindexNum = 1;
	//Fronti���ڱ��Ŀ�������Ƿ��Ѿ���ת����ʼΪ1����-1
	float Fronti = resivedata[1][index].c_str() < 0 ? -1:1;

	//������з���
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
