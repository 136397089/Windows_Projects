#include "stdafx.h"
#include "Locker.h"



CLocker::CLocker(const HANDLE& handleOfMutex, unsigned int timeOut)
{
	if (!handleOfMutex)
	{
		_hMutex = handleOfMutex;
		WaitForSingleObject(handleOfMutex, timeOut);
	}
}

CLocker::CLocker()
{
	if (!_hMutex)
	{
		ReleaseMutex(_hMutex);
		_hMutex = nullptr;
	}
}

CLocker::~CLocker()
{
}
