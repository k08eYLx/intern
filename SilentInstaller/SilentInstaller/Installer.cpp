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
		 * ���� ÿ���ļ������涼�е������������ļ���:
		 *	(1) .  ��ʾ���ļ����Լ�
		 *	(2) .. ��ʾ���ļ��еĸ��ļ���
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
 * name �� �����ݷ�ʽ������
 */
void Installer::deleteDesktopShortcut(string name)
{
	//������ʼ�ļ���Ϊ����
	int nBeginAt = CSIDL_DESKTOPDIRECTORY;
	
	LPITEMIDLIST pidlBeginAt;
	char szShortcut[MAX_PATH] = "", szPath[MAX_PATH] = "";
	
	// ȡ�ÿ�ʼ�˵��������PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// ��PIDLת����·����
	SHGetPathFromIDList(pidlBeginAt, szPath);
	TRACE("%s\n", szPath);

	// ȡ��Ҫɾ���Ŀ�ݷ�ʽ��ȫ·��
	sprintf_s(szShortcut, "%s\\%s.lnk", szPath, name.c_str());
	TRACE("%s\n", szShortcut);
	
	// ɾ����ݷ�ʽ
	::DeleteFile(szShortcut);
}


/*
 * C:\Users\XXX\AppData\Roaming\Microsoft\Windows\Start Menu\Programs
 * name �� ��ʼ�˵���ݷ�ʽĿ¼������
 */
void Installer::deleteStartupMenuShortcutDir(string dirName)
{
	//������ʼ�ļ���Ϊ"��ʼ"�˵�
	int nBeginAt = CSIDL_STARTMENU;

	LPITEMIDLIST pidlBeginAt;
	char szShortcut[MAX_PATH] = "", szPath[MAX_PATH] = "";
	
	// ȡ�ÿ�ʼ�˵��������PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// ��PIDLת����·����
	SHGetPathFromIDList(pidlBeginAt, szPath);
	TRACE("%s\n", szPath);

	// ȡ��Ҫɾ���Ŀ�ݷ�ʽĿ¼��ȫ·������һ�������Programs
	sprintf_s(szShortcut, "%s\\Programs\\%s", szPath, dirName.c_str());
	TRACE("%s\n", szShortcut);
	
	// ɾ����ʼ�˵���ݷ�ʽĿ¼
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
