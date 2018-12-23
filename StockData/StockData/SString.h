#pragma once


#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>

#ifndef __JONES__SSTRING__
#define __JONES__SSTRING__

struct SStringData
{
	long nRefs; //���ü���
	int nDataLength; //�ַ�ʹ�ó���
	int nAllocLength; //���䳤��
	TCHAR* data() { return (TCHAR*)(this + 1); } //����ַ����ĵط� 
	//this+1 �൱����SStringData[1];����TCHAR* data()ָ����SStringData[1]�ĵ�ַ
};

class SString
{
public:
	//���캯��
	SString();
	SString(const SString& stringSrc);
	SString(TCHAR ch, int nLength = 1);
	SString(LPCTSTR lpsz); // SString(LPCSTR lpsz); ANSI�°汾
	//SString(LPCWSTR lpsz);UNICODE�°汾
	SString(LPCTSTR lpch, int nLength); //SString(LPCSTR lpch, int nLength);ANSI�°汾
	//SString(LPCWSTR lpch, int nLength);//UNICODE�°汾
	SString(const unsigned char* psz);
	~SString();
	//SStringData������
	int GetLength() const; //�õ��ַ�����
	int GetAllocLength() const; //�õ�������ڴ泤��
	BOOL IsEmpty() const; //�ж��ַ������Ƿ�Ϊ0
	operator LPCTSTR() const; //����ת��
	void Empty(); //���SStringData
	//����������
	const SString& operator=(const SString& stringSrc);
	const SString& operator=(LPCTSTR lpsz);
	const SString& operator=(TCHAR ch);
	const SString& operator+=(const SString& string);
	const SString& operator+=(TCHAR ch);
	const SString& operator+=(LPCTSTR lpsz);
	TCHAR operator[](int nIndex) const;

	friend SString operator+(const SString& string1, const SString& string2);
	friend SString operator+(const SString& string, TCHAR ch);
	friend SString operator+(TCHAR ch, const SString& string);
	friend SString operator+(const SString& string, LPCTSTR lpsz);
	friend SString operator+(LPCTSTR lpsz, const SString& string);

	//����,���빲�����ݿ�
	int Delete(int nIndex, int nCount = 1);//ɾ����nIndex��ʼ����ΪnCount������
	int Insert(int nIndex, TCHAR ch); //����һ���ַ�
	int Insert(int nIndex, LPCTSTR pstr); //����һ���ַ���
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew); //�滻����
	int Replace(TCHAR chOld, TCHAR chNew); //�滻����
	int Remove(TCHAR chRemove); //�Ƴ�һ���ַ�
	void TrimRight(LPCTSTR lpszTargetList);
	void TrimRight(TCHAR chTarget);//ȥ���ұ�chTarget
	void TrimRight(); //ȥ���ұ߿ո�
	void TrimLeft(LPCTSTR lpszTargets);
	void TrimLeft(TCHAR chTarget); //ȥ�����chTarget
	void TrimLeft(); //ȥ����߿ո�
	//ȡĳ���ַ���
	void SetAt(int nIndex, TCHAR ch);
	TCHAR GetAt(int nIndex) const;
	SString Mid(int nFirst) const; //ȡĳ���ַ���
	SString Mid(int nFirst, int nCount) const; //ȡĳ���ַ���
	SString Right(int nCount) const; //ȡ�ұ��ַ���
	SString Left(int nCount) const; //ȡ����ַ���
//	void SString::MakeUpper(); //��д
//	void SString::MakeLower(); //Сд
	void SString::MakeReverse(); //????��֪����ʲô�� strrev
	//����
	int Find(TCHAR ch) const;
	int Find(TCHAR ch, int nStart) const;
	int ReverseFind(TCHAR ch) const;
	int Find(LPCTSTR lpszSub) const;
	int Find(LPCTSTR lpszSub, int nStart) const;
	int FindOneOf(LPCTSTR lpszCharSet) const;//�õ���һ��ƥ��lpszCharSet������һ���ַ���λ�� ����_tcspbrk
	//�߼�����
	LPTSTR GetBuffer(int nMinBufLength); //���·����ڴ�,�ڿ���ԭ��������
	void ReleaseBuffer(int nNewLength = -1); //��[nNewLength]='/0',���ڴ��Сû�иı�
	LPTSTR GetBufferSetLength(int nNewLength); //���·����ڴ�,�ڿ���ԭ��������
	void FreeExtra(); //����Լ�,Ȼ��--ԭ�������ü�����
	LPTSTR LockBuffer(); //���ü�����=-1,����
	void UnlockBuffer(); //����,���ü�����=1
	//�Ƚ�
	int Compare(LPCTSTR lpsz) const; //���ִ�Сд�Ƚ�
	int CompareNoCase(LPCTSTR lpsz) const; //�����ִ�Сд�Ƚ�
	//�Ƚ��ٶ�û��Compare��
	int Collate(LPCTSTR lpsz) const; //���ִ�Сд�Ƚ�
	int CollateNoCase(LPCTSTR lpsz) const; //�����ִ�Сд�Ƚ�
	//��ʽ���ַ���
	void Format(LPCTSTR lpszFormat, ...);//CSting����ĺ�����,��ȫ���Լ�������(ţ��)

private:
	void Init();
	SStringData* GetData() const; //ͨ��m_pchData-1 �õ�SStringData
	void AllocBuffer(int nLen); //��SStringData�����ڴ�,����������
	void CopyBeforeWrite(); //�����ü����ĸ����Լ����
	void AllocBeforeWrite(int nLen); //��SStringData�����ڴ�,��������
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);//�����ڴ�,������lpszSrcData����
	//��nCopyIndex��ʼ��nCopyLen���ȵ����ݿ�����dest,nExtraLen����ĳ���,�κ�������û������
	void AllocCopy(SString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void Release(); //--���ü��������ж��Ƿ�ɾ���ڴ�,��ɾ������ʼ��
	void FormatV(LPCTSTR lpszFormat, va_list argList);//��ʽ���ַ���
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data,
		int nSrc2Len, LPCTSTR lpszSrc2Data);//��������lpszSrc1Data+lpszSrc2Data
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData); //�����ַ���

	static void  Release(SStringData* pData); //--���ü��������ж��Ƿ�ɾ���ڴ�
	static void FreeData(SStringData* pData); //�ͷ��ڴ�
	static int SafeStrlen(LPCTSTR lpsz); //�õ�����
	LPTSTR m_pchData; //ָ��SStringData��������
};

/*����SString::Compare�Ƚϴ�С,����Ƚ�����CStirng�Ļ���
����operator LPCTSTR()ת������ΪLPCTSTR
*/
bool operator==(const SString& s1, const SString& s2);
bool operator==(const SString& s1, LPCTSTR s2);
bool operator==(LPCTSTR s1, const SString& s2);
bool operator!=(const SString& s1, const SString& s2);
bool operator!=(const SString& s1, LPCTSTR s2);
bool operator!=(LPCTSTR s1, const SString& s2);
bool operator<(const SString& s1, const SString& s2);
bool operator<(const SString& s1, LPCTSTR s2);
bool operator<(LPCTSTR s1, const SString& s2);
bool operator>(const SString& s1, const SString& s2);
bool operator>(const SString& s1, LPCTSTR s2);
bool operator>(LPCTSTR s1, const SString& s2);
bool operator<=(const SString& s1, const SString& s2);
bool operator<=(const SString& s1, LPCTSTR s2);
bool operator<=(LPCTSTR s1, const SString& s2);
bool operator>=(const SString& s1, const SString& s2);
bool operator>=(const SString& s1, LPCTSTR s2);
bool operator>=(LPCTSTR s1, const SString& s2);

//////////////////////////////////////////////////////////////////////
//���lpsz�Ƿ���Ч,������IsBadStringPtr
BOOL AfxIsValidString(LPCTSTR lpsz, int nLength = -1);
//���lp�Ƿ��ܶ�дȨ��,������IsBadReadPtr,IsBadStringPtr
BOOL AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = TRUE);

//CStirng�������
void ConstructElements(SString* pElements, int nCount); //��ʼ��CStirng����
void DestructElements(SString* pElements, int nCount); //ɾ��CStirng����
void CopyElements(SString* pDest, const SString* pSrc, int nCount); //SString���鿽��

#endif //__JONES__SSTRING__

