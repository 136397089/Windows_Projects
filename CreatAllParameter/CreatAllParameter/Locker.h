#ifndef _LOCKER_H
#define _LOCKER_H

#include <windows.h>

class CLocker
{
public:
	CLocker(const HANDLE& handleOfMutex, unsigned int time);
	~CLocker();
private:
	CLocker();
	HANDLE _hMutex;
};




#endif
