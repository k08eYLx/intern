#include "StdAfx.h"
#include "Installer.h"

#include "Tlhelp32.h"

Installer::Installer(void)
	: vDesktop(NULL), hWnd(NULL)
{
}


Installer::~Installer(void)
{
}


void Installer::imitateLeftClick(int xPos, int yPos)
{
	TRACE("%s\n", __FUNCTION__);
	LONG pos = MAKELONG(xPos, yPos);
	::SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, pos);
	::SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, pos);
}


BOOL Installer::rmDir(string dirName)
{
	char sTempFileFind[MAX_PATH] = "";
	sprintf_s(sTempFileFind, "%s\\*.*", dirName.c_str());
	
	CFileFind tempFind;
	BOOL isFinded = tempFind.FindFile(sTempFileFind);
	while (isFinded) {
		isFinded = tempFind.FindNextFile();
		/*
		 * 跳过 每个文件夹下面都有的两个特殊子文件夹:
		 *	(1) .  表示本文件夹自己
		 *	(2) .. 表示本文件夹的父文件夹
		 */
		if (!tempFind.IsDots()) {
			char tempFileOrDir[MAX_PATH] = "";
			sprintf_s(tempFileOrDir, "%s\\%s", dirName.c_str(), tempFind.GetFileName().GetBuffer(MAX_PATH));
			if (tempFind.IsDirectory()) {
				rmDir(tempFileOrDir);
			}
			else {
				DeleteFile(tempFileOrDir);
			}
		}
	}
	tempFind.Close();

	return RemoveDirectory(dirName.c_str());
}


void Installer::deleteShortcuts(string name)
{
	deleteDesktopShortcut(name);
	deleteStartupMenuShortcutDir(name);
}


/*
 * C:\Users\XXX\Desktop
 * name 是 桌面快捷方式的名字
 */
void Installer::deleteDesktopShortcut(string name)
{
	//设置起始文件夹为桌面
	int nBeginAt = CSIDL_DESKTOPDIRECTORY;
	
	LPITEMIDLIST pidlBeginAt;
	char szShortcut[MAX_PATH] = "", szPath[MAX_PATH] = "";
	
	// 取得开始菜单或桌面的PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// 把PIDL转化成路径名
	SHGetPathFromIDList(pidlBeginAt, szPath);
	TRACE("%s\n", szPath);

	// 取得要删除的快捷方式的全路径
	sprintf_s(szShortcut, "%s\\%s.lnk", szPath, name.c_str());
	TRACE("%s\n", szShortcut);
	
	// 删除快捷方式
	::DeleteFile(szShortcut);
}


/*
 * C:\Users\XXX\AppData\Roaming\Microsoft\Windows\Start Menu\Programs
 * name 是 开始菜单快捷方式目录的名字
 */
void Installer::deleteStartupMenuShortcutDir(string dirName)
{
	//设置起始文件夹为"开始"菜单
	int nBeginAt = CSIDL_STARTMENU;

	LPITEMIDLIST pidlBeginAt;
	char szShortcut[MAX_PATH] = "", szPath[MAX_PATH] = "";
	
	// 取得开始菜单或桌面的PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// 把PIDL转化成路径名
	SHGetPathFromIDList(pidlBeginAt, szPath);
	TRACE("%s\n", szPath);

	// 取得要删除的快捷方式目录的全路径，有一个额外的Programs
	sprintf_s(szShortcut, "%s\\Programs\\%s", szPath, dirName.c_str());
	TRACE("%s\n", szShortcut);
	
	// 删除开始菜单快捷方式目录
	rmDir(szShortcut);
}


void Installer::killProcess(string name)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return;
	
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
}
