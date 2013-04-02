// FileUtils.cpp : implementation file
//

#include "stdafx.h"
#include "SilentUninstaller.h"
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

			if (tempFind.IsReadOnly()) ::SetFileAttributes(tempFileOrDir, FILE_ATTRIBUTE_NORMAL);
			
			if (tempFind.IsDirectory()) rmDir(tempFileOrDir);
			else ::DeleteFile(tempFileOrDir);
		}
	}
	tempFind.Close();

	return ::RemoveDirectory(dirName.c_str());
}


void FileUtils::emptyCurDir()
{
	char curDir[MAX_PATH] = { 0 };
	::GetCurrentDirectory(MAX_PATH, curDir);
	rmDir(curDir);
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


bool FileUtils::copy(string from, string to)
{
	SHFILEOPSTRUCT sfos;
	ZeroMemory(&sfos, sizeof(SHFILEOPSTRUCT));

	sfos.hNameMappings = NULL;
	sfos.lpszProgressTitle = NULL;
	sfos.hwnd = HWND_DESKTOP;
	sfos.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sfos.wFunc = FO_COPY;
	// Make "pFrom" to be double-null terminated.
	from.append(2, '\0');
	sfos.pFrom = from.c_str();  // This string must be double-null terminated.
	sfos.pTo = to.c_str();
	
	return (::SHFileOperation(&sfos) == 0);
}


bool FileUtils::copyRelatively(string from, string to)
{
	// Make the from path to fully qualified.
	char buf[MAX_PATH] = { 0 };             // ���·���ı���
	::GetCurrentDirectory(MAX_PATH, buf);   // ��ȡ����ĵ�ǰĿ¼
	
	/*
	// ����GetCurrentDirectory��ȡ�����λ���ǲ�һ����
	::GetModuleFileName(NULL, buf, MAX_PATH);
	::PathRemoveFileSpec(buf);*/

	sprintf_s(buf, "%s\\%s", buf, from.c_str());
	
	return copy(buf, to);
}


bool FileUtils::hide(string name)
{
	DWORD dwFileAttributes = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN;
	return (TRUE == ::SetFileAttributes(name.c_str(), dwFileAttributes));
}


void FileUtils::batSelfDelete()
{
	char fileName[MAX_PATH] = { 0 };
	char name[MAX_PATH] = { 0 };
	char batName[MAX_PATH] = { 0 };

    char *ptr = NULL;
    char r = '"', k = '%';
    
	::GetModuleFileName(NULL, fileName, MAX_PATH);

    ptr = strrchr(fileName, '\\');
    if(ptr != NULL) strcpy_s(name, ptr+1);
	
    FILE *pTmp = NULL;
	sprintf_s(batName, "%s.bat", fileName);
    if (fopen_s(&pTmp, batName, "w") != 0) return;
	
    fprintf(pTmp, ":1\n");
    fprintf(pTmp, "taskkill /F /IM %s\n", name);
    fprintf(pTmp, "del %c%s%c\n", r, fileName, r);
    fprintf(pTmp, "if exist %c%s%c goto 1\n", r, fileName, r);
    fprintf(pTmp, "del %c0\n", k);
    
    fclose(pTmp);

    WinExec(batName, SW_HIDE);
}


BEGIN_MESSAGE_MAP(FileUtils, CWnd)
END_MESSAGE_MAP()



// FileUtils message handlers


