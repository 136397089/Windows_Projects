#pragma once
#ifndef COMTIME_H
#define COMTIME_H
#include <time.h>
#include <windows.h>
#include <string>
#include "glog/logging.h"
using namespace std;
class CComTime
{
public:
	CComTime();
	void TimeBegin();
	void TimeEnd(const string& indexname);
	~CComTime();
private:
	SYSTEMTIME beginTime;
	SYSTEMTIME endTime;
};

#endif

