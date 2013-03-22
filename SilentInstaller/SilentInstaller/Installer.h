#pragma once

#include "VirtualDesktop.h"


class Installer
{
public:
	Installer(void);
	virtual ~Installer(void);

public:
	virtual bool install(VirtualDesktop *vDesktop, string path) = 0;

protected:
	HWND hWnd;

private:
	BOOL rmDir(string dirName);  // Recursively, delete non-empty dir is supported.

protected:
	void imitateLeftClick(int xPos, int yPos);
	void deleteShortcuts(string name);
	void deleteDesktopShortcut(string name);
	void deleteStartupMenuShortcutDir(string dirName);
};
