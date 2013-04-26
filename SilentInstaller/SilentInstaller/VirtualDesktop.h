#pragma once


#include "WindowFinder.h"

// VirtualDesktop

struct ThreadData {
	void*   pVoid;    // 指向类的实例
	HANDLE  hEvent;   // 同步事件
	WndInfo wi;
};

class VirtualDesktop : public CWnd
{
	DECLARE_DYNAMIC(VirtualDesktop)

private:
	VirtualDesktop();

public:
	virtual ~VirtualDesktop();

private:
	static VirtualDesktop *vDesktop;    // the only VD
	CString tmpExe;

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;

	HDESK hDesktop;
	HDESK hOriginalThread;
	HDESK hOriginalInput;
	
	BOOL isOriginalDesktopActive;  // 是否在原始桌面
	BOOL isInitialDesktop;	       // 是否初始化桌面
	
	WindowFinder wndFinder;

private:
	BOOL terminateProcess();       // 在虚拟桌面上创建的进程
	void deleteDesktop();

public:
	static VirtualDesktop *getVirtualDesktop();
	void create();
	void destroy();
	void switchDesktop();

	void listWindows(CEdit *pEdit);

	void findWindows(ProcedureData &pd);
	void findChildWindows(HWND hWndParent, ProcedureData &pd);

	HWND findWindow(string title);                      // Find window on VD
	friend DWORD WINAPI tpFindWindow(LPVOID pParam);    // Thread procedure for find window

protected:
	DECLARE_MESSAGE_MAP()
};


