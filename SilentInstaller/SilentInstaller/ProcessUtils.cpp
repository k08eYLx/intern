// ProcessUtils.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "ProcessUtils.h"


// ProcessUtils

IMPLEMENT_DYNAMIC(ProcessUtils, CWnd)

ProcessUtils::ProcessUtils()
{

}

ProcessUtils::~ProcessUtils()
{
}


bool ProcessUtils::kill(string name)
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(pe32);
	
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;
	
	BOOL isRunning = Process32First(hSnapshot, &pe32);
	while (isRunning) {
		if (pe32.szExeFile == name) {
			DWORD pid = pe32.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (hProcess != NULL) {
				::TerminateProcess(hProcess, 0);
				::CloseHandle(hProcess);
			}
		}
		isRunning = ::Process32Next(hSnapshot, &pe32);
	}

	return true;
}


/*
 * There are some basic steps to adjusting the tokens,:
 *
 * Call OpenProcessToken() with at least the TOKEN_ADJUST_PRIVILEGE and TOKEN_QUERY flags.
 * Use LookupPrivilegeValue() to get the LUID (Locally Unique Identifier) of the privilege you want to adjust.
 * Call AdjustTokenPrivileges() to adjust the tokens.
 * Do whatever calls you need to do to accomplish your task.
 * Call AdjustTokenPrivileges() again to set the old privileges back and leave the tokens as found.
 * Close the Token handle.
 */
bool ProcessUtils::adjustPrivilege()
{
	HANDLE hToken = NULL;

	// Open the access token associated with a specified process (the current).
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		TOKEN_PRIVILEGES tkp = { 0 };
		TOKEN_PRIVILEGES oldTkp = { 0 };
		// Retrieve the locally unique identifier (LUID) used to represent the specified privilege name.
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid)) {
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			// Enable privileges in the specified access token.
			if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, &oldTkp, 0)) {
				/*
				 * Reset the privilege.
				 * AdjustTokenPrivileges(hToken, FALSE, &oldTkp, 0, NULL, 0);
				 */
				CloseHandle(hToken);
				return true;
			}
			CloseHandle(hToken);
			return false;
		}
		CloseHandle(hToken);
		return false;
	}

	return (CloseHandle(hToken) == TRUE);
}


BEGIN_MESSAGE_MAP(ProcessUtils, CWnd)
END_MESSAGE_MAP()



// ProcessUtils message handlers


