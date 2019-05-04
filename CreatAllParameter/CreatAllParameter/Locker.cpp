#include "stdafx.h"
#include "Locker.h"



CMutexLocker::CMutexLocker(const HANDLE& handleOfMutex, unsigned int timeOut)
{
	_hMutex = NULL;
	if (handleOfMutex != NULL)
	{
		_hMutex = handleOfMutex;
		WaitForSingleObject(handleOfMutex, timeOut);
	}
}

CMutexLocker::CMutexLocker()
{
}

CMutexLocker::~CMutexLocker()
{
	if (NULL != _hMutex)
	{
		ReleaseMutex(_hMutex);
		_hMutex = NULL;
	}

}

HANDLE CMutexLocker::CreateMutex(const string& MutexName)
{
	HANDLE _StockCSVFileMutex = CreateMutex( "StockCSVFileMutex");
	return _StockCSVFileMutex;
}

CSectionLocker::CSectionLocker(CRITICAL_SECTION* hSection)
{
	if (NULL == hSection)
		return;
	localSection = hSection;
	EnterCriticalSection(hSection);
}

CSectionLocker::~CSectionLocker()
{
	LeaveCriticalSection(localSection);
	localSection = NULL;
}
