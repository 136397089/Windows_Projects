#ifndef _LOCKER_H
#define _LOCKER_H

#include <windows.h>
#include <string>
using namespace std;
class CMutexLocker
{
public:
	CMutexLocker(const HANDLE& handleOfMutex, unsigned int time);
	~CMutexLocker();
	HANDLE CreateMutex(const string& MutexName);
private:
	CMutexLocker();
	HANDLE _hMutex;
};

class CSectionLocker
{
public:
	CSectionLocker(CRITICAL_SECTION* hSection);
	~CSectionLocker();

private:
	CSectionLocker();
	CRITICAL_SECTION* localSection;
};



#endif
