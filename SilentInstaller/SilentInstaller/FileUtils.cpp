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


string FileUtils::getDesktopPath()
{
	//设置起始文件夹为桌面
	int nBeginAt = CSIDL_DESKTOPDIRECTORY;
	
	LPITEMIDLIST pidlBeginAt;
	char desktopPath[MAX_PATH] = "";
	
	// 取得开始菜单或桌面的PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// 把PIDL转化成路径名
	SHGetPathFromIDList(pidlBeginAt, desktopPath);
	TRACE("%s\n", desktopPath);

	return desktopPath;
}


string FileUtils::getStartMenuProgramsPath()
{
	//设置起始文件夹为"开始"菜单
	int nBeginAt = CSIDL_STARTMENU;

	LPITEMIDLIST pidlBeginAt;
	char startMenuPath[MAX_PATH] = "", startMenuProgramsPath[MAX_PATH] = "";
	
	// 取得开始菜单或桌面的PIDL
	SHGetSpecialFolderLocation(HWND_DESKTOP, nBeginAt, &pidlBeginAt);

	// 把PIDL转化成路径名
	SHGetPathFromIDList(pidlBeginAt, startMenuPath);
	TRACE("%s\n", startMenuProgramsPath);

	// 有一个额外的Programs
	sprintf_s(startMenuProgramsPath, "%s\\Programs", startMenuPath);

	return startMenuProgramsPath;
}


string FileUtils::getDesktopShortcut(string name)
{
	char szShortcut[MAX_PATH] = "";
	
	// 取得要删除的快捷方式的全路径
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
 * name 是 桌面快捷方式的名字，不包括.lnk后缀
 */
void FileUtils::deleteDesktopShortcut(string name)
{
	string shortcut = getDesktopShortcut(name);
	// 删除快捷方式
	::DeleteFile(shortcut.c_str());
}


/*
 * name 是 开始菜单快捷方式直属目录的名字
 */
void FileUtils::deleteStartMenuShortcutDir(string dirName)
{
	char szShortcutDir[MAX_PATH] = "";
	
	// 取得要删除的快捷方式目录的全路径
	string startMenuProgramsPath = FileUtils::getStartMenuProgramsPath();
	sprintf_s(szShortcutDir, "%s\\%s", startMenuProgramsPath.c_str(), dirName.c_str());
	TRACE("%s\n", szShortcutDir);
	
	// 删除开始菜单快捷方式目录
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


