// ProcessSupervisionTool.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>  
#include <vector>
#include <windows.h>  
#include <TlHelp32.h>
#include <string>
#include <iostream>

using namespace std;

int GetProcessList(vector<PROCESSENTRY32>& processList);
void Wchar_tToString(std::string& szDst, wchar_t *wchar);
bool EnableShutdownPrivilege();
void OnMenuShutdown();
bool WaitForControl();


int _tmain(int argc, _TCHAR* argv[])
{
	vector<PROCESSENTRY32> processList;
	bool CannotFindProcess = true;
	while (true)
	{
		Sleep(10);
		CannotFindProcess = true;
		processList.clear();
		GetProcessList(processList);
		for (unsigned int i = 0; i != processList.size(); i++)
		{
			string ProcessName;
			Wchar_tToString(ProcessName, processList[i].szExeFile);
			if (ProcessName == "CreatAllParameter.exe"){
				CannotFindProcess = false;
			}
		}
		if (CannotFindProcess && WaitForControl() && EnableShutdownPrivilege())
		{
			OnMenuShutdown();
		}
	}

	return 0;
}


int GetProcessList(vector<PROCESSENTRY32>& processList)
{
	// Ϊ���̵������߳��ĸ�����  
	HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshort == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot����ʧ�ܣ�\n");
		return -1;
	}
	// ����߳��б������¼���̵߳���ϸ��Ϣ����ʹ��Thread32First��Thread32Next���������м�¼��ÿ���߳���Ϣ  
	PROCESSENTRY32 stcProcessInfo;
	stcProcessInfo.dwSize = sizeof(stcProcessInfo);
	BOOL  bRet = Process32First(hSnapshort, &stcProcessInfo);
	//printf("������\t\t\t ����ID\t �߳���\t ������ID\n");
	while (bRet){
		processList.push_back(stcProcessInfo);
//		printf("%ls\t\t %d\t %d\t %d\n", stcProcessInfo.szExeFile, stcProcessInfo.th32ProcessID, stcProcessInfo.cntThreads, stcProcessInfo.th32ParentProcessID);
		bRet = Process32Next(hSnapshort, &stcProcessInfo);
	}

	CloseHandle(hSnapshort);

	return 0;
}

void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
}

bool EnableShutdownPrivilege()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = { 0 };
	TOKEN_PRIVILEGES stToken_Privileges = { 0 };
	hProcess = ::GetCurrentProcess(); //��ȡ��ǰӦ�ó�����̾��
	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) //�򿪵�ǰ���̵ķ������ƾ��(OpenProcessToken��������ʧ�ܷ���ֵΪ��)
		return false;
	if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID)) //��ȡȨ������Ϊ"SeShutdownPrivilege"��LUID(LookupPrivilegeValue��������ʧ�ܷ���ֵΪ��)
		return false;
	stToken_Privileges.PrivilegeCount = 1; //��������Ȩ�޸���
	stToken_Privileges.Privileges[0].Luid = uID; //Ȩ�޵�LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //Ȩ�޵�����,SE_PRIVILEGE_ENABLEDΪʹ�ܸ�Ȩ��
	if (!::AdjustTokenPrivileges(hToken, FALSE, &stToken_Privileges, sizeof stToken_Privileges, NULL, NULL)) //���������������ָ��Ȩ��(AdjustTokenPrivileges��������ʧ�ܷ���ֵΪ��)
		return false;

	if (::GetLastError() != ERROR_SUCCESS) //�鿴Ȩ���Ƿ�����ɹ�
		return false;
	::CloseHandle(hToken);
	return true;
}

void OnMenuShutdown()
{
	// TODO: �ڴ���������������
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return;
	}
	// Get the LUID for the shutdown privilege.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; // one privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// Get the shutdown privilege for this process.
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	// Shut down the system and force all applications to close.
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0); //����ֻ��Ҫ��EWX_SHUTDOWN��ΪEWX_REBOOT
}

bool WaitForControl()
{
	int CountDown = 60;
	while (true)
	{
		cout << "����"<<CountDown<<"���ػ�����Ҫȡ����ֱ�ӹرձ�����"<<endl;
		Sleep(1000);
		CountDown--;
		if (0 == CountDown)
		{
			return true;
		}
	}
}
