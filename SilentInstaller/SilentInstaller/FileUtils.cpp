// FileUtils.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "FileUtils.h"


// FileUtils

IMPLEMENT_DYNAMIC(FileUtils, CWnd)

FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{
}


BOOL FileUtils::rmDir(string dirName)
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


string FileUtils::getDesktopPath()
{
	//������ʼ�ļ���Ϊ����
	int nBeginAt = CSIDL_DESKTOPDIRECTORY;
	
	LPITEMIDLIST pidlBeginAt;
	char desktopPath[MAX_PATH] = "";
	
	// ȡ�ÿ�ʼ�˵��������PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// ��PIDLת����·����
	SHGetPathFromIDList(pidlBeginAt, desktopPath);
	TRACE("%s\n", desktopPath);

	return desktopPath;
}


string FileUtils::getStartMenuProgramsPath()
{
	//������ʼ�ļ���Ϊ"��ʼ"�˵�
	int nBeginAt = CSIDL_STARTMENU;

	LPITEMIDLIST pidlBeginAt;
	char startMenuPath[MAX_PATH] = "", startMenuProgramsPath[MAX_PATH] = "";
	
	// ȡ�ÿ�ʼ�˵��������PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// ��PIDLת����·����
	SHGetPathFromIDList(pidlBeginAt, startMenuPath);
	TRACE("%s\n", startMenuProgramsPath);

	// ��һ�������Programs
	sprintf_s(startMenuProgramsPath, "%s\\Programs", startMenuPath);

	return startMenuProgramsPath;
}


string FileUtils::getDesktopShortcut(string name)
{
	char szShortcut[MAX_PATH] = "";
	
	// ȡ��Ҫɾ���Ŀ�ݷ�ʽ��ȫ·��
	string desktopPath = FileUtils::getDesktopPath();
	sprintf_s(szShortcut, "%s\\%s.lnk", desktopPath.c_str(), name.c_str());
	TRACE("%s\n", szShortcut);

	return szShortcut;
}


void FileUtils::deleteShortcuts(string name)
{
	deleteDesktopShortcut(name);
	deleteStartMenuShortcutDir(name);
}


/*
 * name �� �����ݷ�ʽ�����֣�������.lnk��׺
 */
void FileUtils::deleteDesktopShortcut(string name)
{
	string shortcut = getDesktopShortcut(name);
	// ɾ����ݷ�ʽ
	::DeleteFile(shortcut.c_str());
}


/*
 * name �� ��ʼ�˵���ݷ�ʽֱ��Ŀ¼������
 */
void FileUtils::deleteStartMenuShortcutDir(string dirName)
{
	char szShortcutDir[MAX_PATH] = "";
	
	// ȡ��Ҫɾ���Ŀ�ݷ�ʽĿ¼��ȫ·��
	string startMenuProgramsPath = FileUtils::getStartMenuProgramsPath();
	sprintf_s(szShortcutDir, "%s\\%s", startMenuProgramsPath.c_str(), dirName.c_str());
	TRACE("%s\n", szShortcutDir);
	
	// ɾ����ʼ�˵���ݷ�ʽĿ¼
	rmDir(szShortcutDir);
}


bool FileUtils::isExist(string fullName)
{
	CFileFind finder;
	return (finder.FindFile(fullName.c_str()) == TRUE);
}


bool FileUtils::isDesktopShortcutExist(string name)
{
	string shortcut = getDesktopShortcut(name);
	return isExist(shortcut);
}


BEGIN_MESSAGE_MAP(FileUtils, CWnd)
END_MESSAGE_MAP()



// FileUtils message handlers

