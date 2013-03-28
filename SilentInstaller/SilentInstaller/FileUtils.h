#pragma once


// FileUtils

class FileUtils : public CWnd
{
	DECLARE_DYNAMIC(FileUtils)

public:
	FileUtils();
	virtual ~FileUtils();

public:
	// Recursively, delete non-empty dir is also supported.
	static BOOL rmDir(string dirName);

	// C:\Users\XXX\Desktop
	static string getDesktopPath();
	// C:\Users\XXX\AppData\Roaming\Microsoft\Windows\Start Menu\Programs
	static string getStartMenuProgramsPath();
	
	static string getDesktopShortcut(string name);

	static void deleteShortcuts(string name);
	static void deleteDesktopShortcut(string name);
	static void deleteStartMenuShortcutDir(string dirName);

	static bool isExist(string fullName);
	static bool isDesktopShortcutExist(string name);

protected:
	DECLARE_MESSAGE_MAP()
};


