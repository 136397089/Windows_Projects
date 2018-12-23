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
	//生成新列，新列以“BAR”正负变化作为依据
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
