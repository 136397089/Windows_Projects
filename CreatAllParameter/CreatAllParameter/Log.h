#pragma once
#ifndef _LOG_H
#define _LOG_H
#include <string>
#include <fstream>
#include <crtdefs.h >
#include <yvals.h  >
#include <ios>

using namespace std;
class CLog
{
public:
	CLog();
	~CLog();
// 	void SetStartTime();
// 	void OutIndexTime(const char* inf);
// 	void Inition();
	fstream& get();
private:
// 	Clog& operator <<(const int& infor);
// 	Clog& operator <<(const string& infor);
// 	Clog& operator <<(const float& infor);
// 	Clog& operator <<(const double& infor);
// 	Clog& operator <<(const basic_ostream<_Elem, _Traits>& ends);
};

#endif
