#pragma once
#ifndef _GROUPDATA_H
#define _GROUPDATA_H
#include <vector>
#include <string>
using namespace std;
class CGroupDataByBAR
{
public:
	CGroupDataByBAR();
	~CGroupDataByBAR();
	//
	//
	//
	//�������У������ԡ�BAR�������仯��Ϊ����
	bool GetGroupData();
	//
	//
	//
private:
	bool FindTageIndex(const string& tageIndexString, vector<string> vCloumnString,int& index);
	//
	//
	//
	bool CreatIndexData(const vector<vector<string>>& resivedata, const int index, vector<vector<string>>& SaveData);
};

#endif //#ifndef _GROUPDATA_H
