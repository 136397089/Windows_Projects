// ProcessSupervisionTool.cpp : 定义控制台应用程序的入口点。
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
	// 为进程的所有线程拍个快照  
	HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshort == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot调用失败！\n");
		return -1;
	}
	// 获得线程列表，里面记录了线程的详细信息，再使用Thread32First和Thread32Next遍历快照中记录的每个线程信息  
	PROCESSENTRY32 stcProcessInfo;
	stcProcessInfo.dwSize = sizeof(stcProcessInfo);
	BOOL  bRet = Process32First(hSnapshort, &stcProcessInfo);
	//printf("进程名\t\t\t 进程ID\t 线程数\t 父进程ID\n");
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
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
}

bool EnableShutdownPrivilege()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = { 0 };
	TOKEN_PRIVILEGES stToken_Privileges = { 0 };
	hProcess = ::GetCurrentProcess(); //获取当前应用程序进程句柄
	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) //打开当前进程的访问令牌句柄(OpenProcessToken函数调用失败返回值为零)
		return false;
	if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID)) //获取权限名称为"SeShutdownPrivilege"的LUID(LookupPrivilegeValue函数调用失败返回值为零)
		return false;
	stToken_Privileges.PrivilegeCount = 1; //欲调整的权限个数
	stToken_Privileges.Privileges[0].Luid = uID; //权限的LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //权限的属性,SE_PRIVILEGE_ENABLED为使能该权限
	if (!::AdjustTokenPrivileges(hToken, FALSE, &stToken_Privileges, sizeof stToken_Privileges, NULL, NULL)) //调整访问令牌里的指定权限(AdjustTokenPrivileges函数调用失败返回值为零)
		return false;

	if (::GetLastError() != ERROR_SUCCESS) //查看权限是否调整成功
		return false;
	::CloseHandle(hToken);
	return true;
}

void OnMenuShutdown()
{
	// TODO: 在此添加命令处理程序代码
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
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0); //重启只需要把EWX_SHUTDOWN改为EWX_REBOOT
}

bool WaitForControl()
{
	int CountDown = 60;
	while (true)
	{
		cout << "将在"<<CountDown<<"秒后关机。需要取消可直接关闭本程序。"<<endl;
		Sleep(1000);
		CountDown--;
		if (0 == CountDown)
		{
			return true;
		}
	}
}
