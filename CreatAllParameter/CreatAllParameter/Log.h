#pragma once
#ifndef _LOG_H
#define _LOG_H
#include <map>
#include <vector>
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
	CLog(string filename);
	~CLog();
private:
	string logfilename;
	string logfilePath;
	template<class T>
	bool SaveCsvFile(const vector<T>& dataTosave);
// 	bool SaveCSVFile(map<string, map<string, float>> dataToSave);
// 	bool SaveCSVFile(map<string, map<string, int>> dataToSave);
// 	bool SaveCSVFile(map<string, map<string, string>> dataToSave);
// 	bool SaveCSVFile(map<string, map<string, float>> dataToSave);
};

template<class T>
bool CLog::SaveCsvFile(const vector<T>& dataTosave)
{
	fstream LogFile;
	LogFile.open(logfilePath + logfilename, fstream::out);
	return true;
}

#endif
