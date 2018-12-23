#include "stdafx.h"
#include <assert.h>
#include <string.h>
#include "SString.h"

// 
// SString::SString()
// {
// }
// 
// 
// SString::~SString()
// {
// }


TCHAR afxChNil = '/0';
int _afxInitData[] = { -1, 0, 0, 0 }; //��ʼ��SStringData�ĵ�ַ
SStringData* _afxDataNil = (SStringData*)&_afxInitData; //��ַת��ΪSStringData*
LPCTSTR _afxPchNil = (LPCTSTR)(((BYTE*)&_afxInitData) + sizeof(SStringData));
const SString&  AfxGetEmptyString()  //����һ���յ�SString
{
	return *(SString*)&_afxPchNil;
}

BOOL AfxIsValidString(LPCTSTR lpsz, int nLength /* = -1 */)
{
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtr(lpsz, nLength) == 0;
}

BOOL AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite /* = TRUE */)
{
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
}

void SString::Init()
{
	m_pchData = AfxGetEmptyString().m_pchData;
}


SString::SString()
{
	Init();
}

int SString::GetLength() const
{
	return GetData()->nDataLength;
}

int SString::GetAllocLength() const
{
	return GetData()->nAllocLength;
}

BOOL SString::IsEmpty() const
{
	return GetData()->nDataLength == 0;
}

SStringData* SString::GetData() const
{
	assert(m_pchData != NULL);
	return ((SStringData*)m_pchData) - 1;
}

SString::operator LPCTSTR() const
{
	return m_pchData;
}

int SString::SafeStrlen(LPCTSTR lpsz)
{
	return (lpsz == NULL) ? 0 : lstrlen(lpsz);
}

void SString::AllocBuffer(int nLen)
{
	assert(nLen >= 0);
	assert(nLen <= 2147483647 - 1);    // (signed) int �����ֵ

	if (nLen == 0)
		Init();
	else
	{
		SStringData* pData;
		{
			pData = (SStringData*)
				new BYTE[sizeof(SStringData) + (nLen + 1)*sizeof(TCHAR)];
			pData->nAllocLength = nLen;
		}
		pData->nRefs = 1;
		pData->data()[nLen] = '/0';
		pData->nDataLength = nLen;
		m_pchData = pData->data();
	}
}

void SString::FreeData(SStringData* pData)
{
	delete[](BYTE*)pData;
}

void SString::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		SStringData* pData = GetData();
		Release();
		AllocBuffer(pData->nDataLength);
		memcpy(m_pchData, pData->data(), (pData->nDataLength + 1)*sizeof(TCHAR));
	}
	assert(GetData()->nRefs <= 1);
}

void SString::AllocBeforeWrite(int nLen)
{
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		AllocBuffer(nLen);
	}
	assert(GetData()->nRefs <= 1);
}

void SString::AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
	AllocBeforeWrite(nSrcLen);
	memcpy(m_pchData, lpszSrcData, nSrcLen*sizeof(TCHAR));
	GetData()->nDataLength = nSrcLen;
	m_pchData[nSrcLen] = '/0';
}

void SString::AllocCopy(SString& dest, int nCopyLen, int nCopyIndex,
	int nExtraLen) const
{
	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		dest.AllocBuffer(nNewLen);
		memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen*sizeof(TCHAR));
	}
}


SString::~SString()
{
	if (GetData() != _afxDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
	}
}


SString::SString(const SString& stringSrc)
{
	assert(stringSrc.GetData()->nRefs != 0);
	if (stringSrc.GetData()->nRefs >= 0)
	{
		assert(stringSrc.GetData() != _afxDataNil);
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement(&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}

SString::SString(LPCTSTR lpsz)
{
	Init();
	int nLen = SafeStrlen(lpsz);
	if (nLen != 0)
	{
		AllocBuffer(nLen);
		memcpy(m_pchData, lpsz, nLen*sizeof(TCHAR));
	}
}

SString::SString(LPCTSTR lpch, int nLength)
{
	Init();
	if (nLength != 0)
	{
		assert(AfxIsValidAddress(lpch, nLength, FALSE));
		AllocBuffer(nLength);
		memcpy(m_pchData, lpch, nLength*sizeof(TCHAR));
	}
}


void SString::Release()
{
	if (GetData() != _afxDataNil)
	{
		assert(GetData()->nRefs != 0);
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
		Init();
	}
}

void SString::Release(SStringData* pData)
{
	if (pData != _afxDataNil)
	{
		assert(pData->nRefs != 0);
		if (InterlockedDecrement(&pData->nRefs) <= 0)
			FreeData(pData);
	}
}

void SString::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = &afxChNil;
	assert(GetData()->nDataLength == 0);
	assert(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}


const SString& SString::operator=(const SString& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _afxDataNil) ||
			stringSrc.GetData()->nRefs < 0)
		{
			//�½�һ������
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			//ֻ����ָ��
			Release();
			assert(stringSrc.GetData() != _afxDataNil);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
	}
	return *this;
}

const SString& SString::operator=(LPCTSTR lpsz)
{
	assert(lpsz == NULL || AfxIsValidString(lpsz));
	AssignCopy(SafeStrlen(lpsz), lpsz);
	return *this;
}

const SString& SString::operator=(TCHAR ch)
{
	AssignCopy(1, &ch);
	return *this;
}


int SString::Delete(int nIndex, int nCount /* = 1 */)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite(); //���빲�����ݿ飬
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
		//�ƶ�����
		memcpy(m_pchData + nIndex,
			m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
		GetData()->nDataLength = nNewLength - nCount;
	}
	return nNewLength;
}


int SString::Insert(int nIndex, TCHAR ch)
{
	CopyBeforeWrite(); //���빲������

	if (nIndex < 0)
		nIndex = 0;

	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;

	if (GetData()->nAllocLength < nNewLength)
	{ //��̬�����ڴ�,������ԭ��������
		SStringData* pOldData = GetData();
		LPTSTR pstr = m_pchData;
		AllocBuffer(nNewLength);
		memcpy(m_pchData, pstr, (pOldData->nDataLength + 1)*sizeof(TCHAR));
		SString::Release(pOldData);
	}
	//��������
	memcpy(m_pchData + nIndex + 1,
		m_pchData + nIndex, (nNewLength - nIndex)*sizeof(TCHAR));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;

	return nNewLength;
}


int SString::Insert(int nIndex, LPCTSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;

	int nInsertLength = SafeStrlen(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite(); //���빲������
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;

		if (GetData()->nAllocLength < nNewLength)
		{ //��̬�����ڴ�,������ԭ��������
			SStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, (pOldData->nDataLength + 1)*sizeof(TCHAR));
			SString::Release(pOldData);
		}

		//�ƶ����ݣ����������λ��moveҲ����
		memcpy(m_pchData + nIndex + nInsertLength,
			m_pchData + nIndex,
			(nNewLength - nIndex - nInsertLength + 1)*sizeof(TCHAR));
		//��������
		memcpy(m_pchData + nIndex,
			pstr, nInsertLength*sizeof(TCHAR));
		GetData()->nDataLength = nNewLength;
	}

	return nNewLength;
}

int SString::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;
	if (chOld != chNew) //�滻�Ĳ�����ͬ
	{
		CopyBeforeWrite();
		LPTSTR psz = m_pchData;
		LPTSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			if (*psz == chOld) //�滻
			{
				*psz = chNew;
				nCount++;
			}
			psz = _tcsinc(psz); //�൱��++psz,����ҪUNICODE�°汾���õ�
		}
	}
	return nCount;
}

int SString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0) //Ҫ�滻�Ĳ���Ϊ��
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd) //����Ҫ�滻�ĸ���
	{
		while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += lstrlen(lpszStart) + 1;
	}


	if (nCount > 0)
	{
		CopyBeforeWrite();
		int nOldLength = GetData()->nDataLength;
		int nNewLength = nOldLength + (nReplacementLen - nSourceLen)*nCount; //�滻�Ժ�ĳ���
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{ //����ԭ�����ڴ泤�ȶ�̬����
			SStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
			SString::Release(pOldData);
		}

		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;


		while (lpszStart < lpszEnd) //���ѭ������ûʲô��
		{
			while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL) //��ʼ�滻
			{
				int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen); //Ҫ�����Ƶĳ���
				//�ƶ����ݣ����������λ��
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(TCHAR));
				//�����滻����
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '/0';
				nOldLength += (nReplacementLen - nSourceLen); //�������ݳ���
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		assert(m_pchData[nNewLength] == '/0');
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}


int SString::Remove(TCHAR chRemove)
{
	CopyBeforeWrite();

	LPTSTR pstrSource = m_pchData;
	LPTSTR pstrDest = m_pchData;
	LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource; //�Ѳ��Ƴ������ݿ���
			pstrDest = _tcsinc(pstrDest);
		}
		pstrSource = _tcsinc(pstrSource);//++pstrSource
	}
	*pstrDest = '/0';
	int nCount = pstrSource - pstrDest; //�Ƚϱ�̬�ļ����滻����,
	GetData()->nDataLength -= nCount;

	return nCount;
}


SString SString::Mid(int nFirst) const
{
	return Mid(nFirst, GetData()->nDataLength - nFirst);
}

SString SString::Mid(int nFirst, int nCount) const
{
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;

	assert(nFirst >= 0);
	assert(nFirst + nCount <= GetData()->nDataLength);

	//ȡȥ��������
	if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
		return *this;

	SString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}


SString SString::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	SString dest;
	AllocCopy(dest, nCount, GetData()->nDataLength - nCount, 0);
	return dest;
}

SString SString::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	SString dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}


int SString::ReverseFind(TCHAR ch) const
{
	//��������
	LPTSTR lpsz = _tcsrchr(m_pchData, (_TUCHAR)ch);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int SString::Find(TCHAR ch) const
{
	return Find(ch, 0);
}

int SString::Find(TCHAR ch, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;

	LPTSTR lpsz = _tcschr(m_pchData + nStart, (_TUCHAR)ch);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int SString::Find(LPCTSTR lpszSub) const
{
	return Find(lpszSub, 0);
}

int SString::Find(LPCTSTR lpszSub, int nStart) const
{
	assert(AfxIsValidString(lpszSub));

	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;

	LPTSTR lpsz = _tcsstr(m_pchData + nStart, lpszSub);

	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int SString::FindOneOf(LPCTSTR lpszCharSet) const
{
	assert(AfxIsValidString(lpszCharSet));
	LPTSTR lpsz = _tcspbrk(m_pchData, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

// void SString::MakeUpper()
// {
// 	CopyBeforeWrite();
// 	_tcsupr(m_pchData);
// }

// void SString::MakeLower()
// {
// 	CopyBeforeWrite();
// 	_tcslwr(m_pchData);
// }

void SString::MakeReverse()
{
	CopyBeforeWrite();
	_tcsrev(m_pchData);
}

void SString::SetAt(int nIndex, TCHAR ch)
{
	assert(nIndex >= 0);
	assert(nIndex < GetData()->nDataLength);

	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

void SString::TrimRight(LPCTSTR lpszTargetList)
{
	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '/0')
	{
		if (_tcschr(lpszTargetList, *lpsz) != NULL)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _tcsinc(lpsz);
	}

	if (lpszLast != NULL)
	{
		*lpszLast = '/0';
		GetData()->nDataLength = lpszLast - m_pchData;
	}
}

void SString::TrimRight(TCHAR chTarget)
{
	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '/0')
	{
		if (*lpsz == chTarget)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _tcsinc(lpsz);
	}

	if (lpszLast != NULL)
	{
		*lpszLast = '/0';
		GetData()->nDataLength = lpszLast - m_pchData;
	}
}

void SString::TrimRight()
{
	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '/0')
	{
		if (_istspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _tcsinc(lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '/0';
		GetData()->nDataLength = lpszLast - m_pchData;
	}
}

void SString::TrimLeft(LPCTSTR lpszTargets)
{
	// if we're not trimming anything, we're not doing any work
	if (SafeStrlen(lpszTargets) == 0)
		return;

	CopyBeforeWrite();
	LPCTSTR lpsz = m_pchData;

	while (*lpsz != '/0')
	{
		if (_tcschr(lpszTargets, *lpsz) == NULL)
			break;
		lpsz = _tcsinc(lpsz);
	}

	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1)*sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}
}

void SString::TrimLeft(TCHAR chTarget)
{
	CopyBeforeWrite();
	LPCTSTR lpsz = m_pchData;

	while (chTarget == *lpsz)
		lpsz = _tcsinc(lpsz);

	if (lpsz != m_pchData)
	{
		int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1)*sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}
}

void SString::TrimLeft()
{
	CopyBeforeWrite();
	LPCTSTR lpsz = m_pchData;

	while (_istspace(*lpsz))
		lpsz = _tcsinc(lpsz);

	if (lpsz != m_pchData)
	{
		int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1)*sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}
}

#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char

struct _AFX_DOUBLE  { BYTE doubleBits[sizeof(double)]; };

#ifdef _X86_
#define DOUBLE_ARG  _AFX_DOUBLE
#else
#define DOUBLE_ARG  double
#endif

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

void SString::FormatV(LPCTSTR lpszFormat, va_list argList)
{
	assert(AfxIsValidString(lpszFormat));

	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '/0'; lpsz = _tcsinc(lpsz))
	{
		//����%,��%%���ڲ��ҷ�Χ
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
		{
			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;

		//%����ĸ�ʽ�ж�
		int nWidth = 0;
		for (; *lpsz != '/0'; lpsz = _tcsinc(lpsz))
		{
			if (*lpsz == '#')
				nMaxLen += 2;   // 16���� '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '/0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
		assert(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '/0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
			assert(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			//ASSERT(FALSE);
#endif
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

			// strings
		case 's':
		{
			LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
			if (pstrNextArg == NULL)
				nItemLen = 6;  // "(null)"
			else
			{
				nItemLen = lstrlen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
		}
		break;

		case 'S':
		{
#ifndef _UNICODE
			LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			if (pstrNextArg == NULL)
				nItemLen = 6;  // "(null)"
			else
			{
				nItemLen = wcslen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
#else
			LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			if (pstrNextArg == NULL)
				nItemLen = 6; // "(null)"
			else
			{
				nItemLen = lstrlenA(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
#endif
		}
		break;

		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:
		{
			LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			if (pstrNextArg == NULL)
				nItemLen = 6; // "(null)"
			else
			{
				nItemLen = lstrlenA(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
		}
		break;

		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
		{
			LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			if (pstrNextArg == NULL)
				nItemLen = 6; // "(null)"
			else
			{
				nItemLen = wcslen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
		}
		break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			case 'f':
			{
				double f;
				LPTSTR pszTemp;

				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == max precision of a double
				// 6 == adjustment in case precision is not specified,
				//   which means that the precision defaults to 6
				pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));

				f = va_arg(argList, double);
				_stprintf(pszTemp, _T("%*.*f"), nWidth, nPrecision + 6, f);
				nItemLen = _tcslen(pszTemp);
			}
			break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

				// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				assert(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	GetBuffer(nMaxLen);
	//VERIFY(_vstprintf(m_pchData, lpszFormat, argListSave) <= GetAllocLength());
	_vstprintf(m_pchData, lpszFormat, argListSave);
	ReleaseBuffer();

	va_end(argListSave);
}

void SString::Format(LPCTSTR lpszFormat, ...)
{
	assert(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

void SString::ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data)
{
	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		AllocBuffer(nNewLen);
		memcpy(m_pchData, lpszSrc1Data, nSrc1Len*sizeof(TCHAR));
		memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len*sizeof(TCHAR));
	}
}

void SString::ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
{
	if (nSrcLen == 0)
		return;

	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{//��̬����
		SStringData* pOldData = GetData();
		ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
		assert(pOldData != NULL);
		SString::Release(pOldData);
	}
	else
	{//ֱ���������
		memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen*sizeof(TCHAR));
		GetData()->nDataLength += nSrcLen;
		assert(GetData()->nDataLength <= GetData()->nAllocLength);
		m_pchData[GetData()->nDataLength] = '/0';
	}
}

const SString& SString::operator+=(LPCTSTR lpsz)
{
	assert(lpsz == NULL || AfxIsValidString(lpsz));
	ConcatInPlace(SafeStrlen(lpsz), lpsz);
	return *this;
}

const SString& SString::operator+=(TCHAR ch)
{
	ConcatInPlace(1, &ch);
	return *this;
}

const SString& SString::operator+=(const SString& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}


LPTSTR SString::GetBuffer(int nMinBufLength)
{
	assert(nMinBufLength >= 0);
	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{ //���¶�̬����
		SStringData* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;
		AllocBuffer(nMinBufLength);
		memcpy(m_pchData, pOldData->data(), (nOldLen + 1)*sizeof(TCHAR));
		GetData()->nDataLength = nOldLen;
		SString::Release(pOldData);
	}
	assert(GetData()->nRefs <= 1);
	assert(m_pchData != NULL);
	return m_pchData;
}

void SString::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  //���빲�����ݿ飬

	if (nNewLength == -1)
		nNewLength = lstrlen(m_pchData); // zero terminated

	assert(nNewLength <= GetData()->nAllocLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '/0';
}

LPTSTR SString::GetBufferSetLength(int nNewLength)
{
	assert(nNewLength >= 0);

	GetBuffer(nNewLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '/0';
	return m_pchData;
}

void SString::FreeExtra()
{
	assert(GetData()->nDataLength <= GetData()->nAllocLength);
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		SStringData* pOldData = GetData();
		AllocBuffer(GetData()->nDataLength);
		memcpy(m_pchData, pOldData->data(), pOldData->nDataLength*sizeof(TCHAR));
		assert(m_pchData[GetData()->nDataLength] == '/0');
		SString::Release(pOldData);
	}
	assert(GetData() != NULL);
}

LPTSTR SString::LockBuffer()
{
	LPTSTR lpsz = GetBuffer(0);
	GetData()->nRefs = -1;
	return lpsz;
}

void SString::UnlockBuffer()
{
	assert(GetData()->nRefs == -1);
	if (GetData() != _afxDataNil)
		GetData()->nRefs = 1;
}


int SString::Compare(LPCTSTR lpsz) const
{
	assert(AfxIsValidString(lpsz));
	return _tcscmp(m_pchData, lpsz);
}

int SString::CompareNoCase(LPCTSTR lpsz) const
{
	assert(AfxIsValidString(lpsz));
	return _tcsicmp(m_pchData, lpsz);
}

// SString::Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
int SString::Collate(LPCTSTR lpsz) const
{
	assert(AfxIsValidString(lpsz));
	return _tcscoll(m_pchData, lpsz);
}


int SString::CollateNoCase(LPCTSTR lpsz) const
{
	assert(AfxIsValidString(lpsz));
	return _tcsicoll(m_pchData, lpsz);
}


TCHAR SString::GetAt(int nIndex) const
{
	assert(nIndex >= 0);
	assert(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}


TCHAR SString::operator[](int nIndex) const
{
	assert(nIndex >= 0);
	assert(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
SString operator+(const SString& string1, const SString& string2)
{
	SString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData,
		string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

SString operator+(const SString& string, LPCTSTR lpsz)
{
	assert(lpsz == NULL || AfxIsValidString(lpsz));
	SString s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData,
		SString::SafeStrlen(lpsz), lpsz);
	return s;
}

SString operator+(LPCTSTR lpsz, const SString& string)
{
	assert(lpsz == NULL || AfxIsValidString(lpsz));
	SString s;
	s.ConcatCopy(SString::SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength,
		string.m_pchData);
	return s;
}

bool operator==(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) == 0;
}

bool operator==(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) == 0;
}

bool operator==(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) == 0;
}

bool operator!=(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) != 0;
}

bool operator!=(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) != 0;
}

bool operator!=(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) != 0;
}

bool operator<(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) < 0;
}

bool operator<(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) < 0;
}

bool operator<(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) > 0;
}

bool operator>(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) > 0;
}

bool operator>(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) > 0;
}

bool operator>(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) < 0;
}

bool operator<=(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) <= 0;
}

bool operator<=(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) <= 0;
}

bool operator<=(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) >= 0;
}

bool operator>=(const SString& s1, const SString& s2)
{
	return s1.Compare(s2) >= 0;
}

bool operator>=(const SString& s1, LPCTSTR s2)
{
	return s1.Compare(s2) >= 0;
}

bool operator>=(LPCTSTR s1, const SString& s2)
{
	return s2.Compare(s1) <= 0;
}

////////////////////////////////////////////////////////////////////////////////

void ConstructElements(SString* pElements, int nCount)
{
	assert(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(SString)));

	for (; nCount--; ++pElements)
		memcpy(pElements, &AfxGetEmptyString(), sizeof(*pElements));
}


void DestructElements(SString* pElements, int nCount)
{
	assert(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(SString)));

	for (; nCount--; ++pElements)
		pElements->~SString();
}



void CopyElements(SString* pDest, const SString* pSrc, int nCount)
{
	assert(nCount == 0 ||
		AfxIsValidAddress(pDest, nCount * sizeof(SString)));
	assert(nCount == 0 ||
		AfxIsValidAddress(pSrc, nCount * sizeof(SString)));

	for (; nCount--; ++pDest, ++pSrc)
		*pDest = *pSrc;
}


