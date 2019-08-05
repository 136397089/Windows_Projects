#pragma once
#ifndef SAVERESULT_H
#define SAVERESULT_H
#include <string>
#include "DateTool.h"

using namespace std;


class CSaveCodeList
{
public:
	CSaveCodeList();
	~CSaveCodeList();
	void ClearCodeList();

	void AddCode(const string& code);

	void SaveCode();

	bool CheckToday(const CDate& today);

};
#endif

