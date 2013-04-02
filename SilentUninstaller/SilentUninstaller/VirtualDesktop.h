#pragma once


#include "WindowFinder.h"

// VirtualDesktop

struct ThreadData {
	void*   pVoid;    // ָ�����ʵ��
	HANDLE  hEvent;   // ͬ���¼�
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
	CString virDesktopName;
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
	bool runProcess(string process);
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


