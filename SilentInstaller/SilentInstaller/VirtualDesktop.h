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
	static VirtualDesktop *vDesktop;    // the only VD
	
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;

	HDESK hDesktop;
	HDESK hOriginalThread;
	HDESK hOriginalInput;
	
	BOOL isOriginalDesktopActive;  // �Ƿ���ԭʼ����
	BOOL isInitialDesktop;	       // �Ƿ��ʼ������
	
	WindowFinder wndFinder;

private:
	BOOL terminateProcess();       // �����������ϴ����Ľ���
	void deleteDesktop();

public:
	static VirtualDesktop *getVirtualDesktop();
	void create();
	void destroy();
	void switchDesktop();

	void listWindows(CEdit *pEdit);

	HWND findWindow(string title);                      // Find window on VD
	friend DWORD WINAPI tpFindWindow(LPVOID pParam);    // Thread procedure for find window

	HWND fromPoint(POINT pt);                           // Find window by POINT on VD
	friend DWORD WINAPI tpFromPoint(LPVOID pParam);     // Thread procedure for find window by POINT

protected:
	DECLARE_MESSAGE_MAP()
};


