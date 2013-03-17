#pragma once


#include "WindowFinder.h"

// VirtualDesktop

class VirtualDesktop : public CWnd
{
	DECLARE_DYNAMIC(VirtualDesktop)

private:
	VirtualDesktop();

public:
	virtual ~VirtualDesktop();

private:
	static VirtualDesktop *vDesktop;
	
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;

	HDESK hDesktop;
	HDESK hOriginalThread;
	HDESK hOriginalInput;
	
	BOOL isOriginalDesktopActive;	// 是否在原始桌面
	BOOL isInitialDesktop;			// 是否初始化桌面
	
	WindowFinder wndFinder;

private:
	HWND getHwndByPid(DWORD dwPid);
	BOOL terminateProcess();  // 在虚拟桌面上创建的进程
	void deleteDesktop();

public:
	static VirtualDesktop *getVirtualDesktop();
	void create();
	void destroy();
	void switchDesktop();

	void listWindows(CEdit *pEdit);

protected:
	DECLARE_MESSAGE_MAP()
};


