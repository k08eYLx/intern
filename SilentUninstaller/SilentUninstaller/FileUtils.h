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
	// !!! Do NOT test this in your develop working directory.
	static void emptyCurDir();

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

	/*
	 * Copy file or directory.
	 * from is fully qualified paths.
	 * Each file name is terminated by a single NULL character. 
	 * The last file name is terminated with a double NULL character ("\0\0") to indicate the end of the buffer.
	 */
	static bool copy(string from, string to);

	/*
	 * from path is relative to the current directory.
	 * Do NOT support multiple file names.
	 */
	static bool copyRelatively(string from, string to);

	// name can be either file or dir.
	static bool hide(string name);

	static void batSelfDelete();

protected:
	DECLARE_MESSAGE_MAP()
};


