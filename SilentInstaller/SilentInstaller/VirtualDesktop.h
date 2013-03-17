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
	
	BOOL isOriginalDesktopActive;	// �Ƿ���ԭʼ����
	BOOL isInitialDesktop;			// �Ƿ��ʼ������
	
	WindowFinder wndFinder;

private:
	HWND getHwndByPid(DWORD dwPid);
	BOOL terminateProcess();  // �����������ϴ����Ľ���
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


