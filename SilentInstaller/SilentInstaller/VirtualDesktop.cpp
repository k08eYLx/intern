// VirtualDesktop.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "VirtualDesktop.h"


// VirtualDesktop

VirtualDesktop * VirtualDesktop::vDesktop = NULL;

IMPLEMENT_DYNAMIC(VirtualDesktop, CWnd)

VirtualDesktop::VirtualDesktop()
{
	isOriginalDesktopActive = TRUE;
	isInitialDesktop = TRUE;
}

VirtualDesktop::~VirtualDesktop()
{
}


VirtualDesktop *VirtualDesktop::getVirtualDesktop()
{
	if (vDesktop == NULL) {
		vDesktop = new VirtualDesktop();
	}
	return vDesktop;
}

void VirtualDesktop::create()
{
	if (isInitialDesktop) {	   // ��һ������ʱ��ʼ������
		// ��������Ϊ��siVirtualDesktop�����������棨��CloseDesktop���Ӧ��
		CString desktopName = "siVirtualDesktop";
		hDesktop = CreateDesktop(desktopName, NULL, NULL, 0, GENERIC_ALL , NULL);	

		// ��ʼ���������̱���Ľṹ��
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		ZeroMemory(&processInfo, sizeof(processInfo));
		startupInfo.cb         = sizeof(startupInfo);
		startupInfo.lpDesktop  = desktopName.GetBuffer();    // Ĭ�������"Default"
		desktopName.ReleaseBuffer();

		// ��¼ԭʼ����ľ��
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput  = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP);

		/*::CreateProcess(NULL, "C:\\Windows\\explorer.exe", NULL, NULL, TRUE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);*/

		CString commandLine = "BaiduYun.exe";    // "explorer"
		::CreateProcess(NULL, (CT2A)commandLine, NULL, NULL, TRUE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);

		// ��ɳ�ʼ�����Ժ󲻻��ٴα����ã���ֹ���������������Դ������
		isInitialDesktop = !isInitialDesktop;
	}
}


BOOL VirtualDesktop::terminateProcess()
{
	// ��ȡ���������ϵĽ��̾��
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.dwProcessId);
	if (hProcess) {
		TRACE("%d\n", processInfo.dwProcessId);
		::TerminateProcess(hProcess, EXIT_SUCCESS);	   // �������������ϵĽ���
	}
	return ::CloseHandle(hProcess);
}

void VirtualDesktop::deleteDesktop()
{
	::CloseDesktop(hDesktop);
	delete vDesktop;
	vDesktop = NULL;
}

void VirtualDesktop::destroy()
{
	// �ر��ɱ����򴴽������������������еĽ���
	if (vDesktop != NULL) {
		terminateProcess();
		deleteDesktop();
	}
}

/*
 * For debug only.
 * switchֻ�踺���л����棬û�б�Ҫ�ٴ�����ر�Virtual Desktop
 */
void VirtualDesktop::switchDesktop()
{
	if (isOriginalDesktopActive) {	   // �Ƿ���ԭʼ����
		//hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// ������������
		SetThreadDesktop(hDesktop);    // ���������������������� 
		SwitchDesktop(hDesktop);
	}
	else {
		SetThreadDesktop(hOriginalThread); // ��������������ԭʼ���� 
		SwitchDesktop(hOriginalInput);     // �л���ԭʼ����
		//CloseDesktop(hDesktop);	       // �ر���������
	}
	isOriginalDesktopActive = !isOriginalDesktopActive;
}


void VirtualDesktop::listWindows(CEdit *pEdit)
{
	wndFinder.listWindows(hDesktop, pEdit);
}


DWORD WINAPI tpFindWindow(LPVOID pParam)
{
	WndInfo *pWndInfo = (WndInfo *)pParam;
	if (pWndInfo == NULL || pWndInfo->hEvent == NULL) return 1;
	::SetThreadDesktop(pWndInfo->hDesktop);
	pWndInfo->hWnd = ::FindWindow(NULL, (pWndInfo->title).c_str());
	::SetEvent(pWndInfo->hEvent);
	return 0;
}

/*
 * �´���һ���̣߳�
 * ���߳���ִ��SetThreadDesktop������ָ�����棬
 * Ȼ����Դ�����ʹ��FindWindow��
 */
HWND VirtualDesktop::findWindow(string title)
{
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, "SyncEvent");
	WndInfo *pWndInfo = new WndInfo();
	ZeroMemory(pWndInfo, sizeof(WndInfo));
	pWndInfo->hEvent = hEvent;
	pWndInfo->hDesktop = hDesktop;
	pWndInfo->title = title;
	HANDLE hThread = CreateThread(NULL, 0,
		tpFindWindow, (LPVOID)pWndInfo, 0, NULL);
	WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);
	HWND hWnd = pWndInfo->hWnd;
	if (pWndInfo != NULL) delete pWndInfo;
	return hWnd;
}


DWORD WINAPI tpFromPoint(LPVOID pParam)
{
	WndInfo *pWndInfo = (WndInfo *)pParam;
	if (pWndInfo == NULL || pWndInfo->hEvent == NULL) return 1;
	
	VirtualDesktop *vDesktop = (VirtualDesktop *)pWndInfo->pVoid;
	vDesktop->switchDesktop();

	HWND hWnd = ::WindowFromPoint(pWndInfo->pt);
	pWndInfo->hWnd = hWnd;

	// caption
    CString caption;
	DWORD dwResult = 0;
    SendMessageTimeout(hWnd, WM_GETTEXT, 256, (LPARAM)caption.GetBuffer(256),
        SMTO_ABORTIFHUNG, 100, &dwResult);
    caption.ReleaseBuffer();
	TRACE("===> %s\n", caption);

	char classname[MAX_PATH] = { 0 };
	::GetClassName(hWnd, classname, sizeof(classname) - 1);
	TRACE("%d, %d ===> %s\n", pWndInfo->pt.x, pWndInfo->pt.y, classname);
	
	CWnd *pWnd = CWnd::FromHandle(pWndInfo->hWnd);
	CString title;
	pWnd->GetWindowText(title);
	TRACE("%d ===> %s\n", pWndInfo->hWnd, title);
	
	vDesktop->switchDesktop();

	::SetEvent(pWndInfo->hEvent);
	return 0;
}

/*
 * �´���һ���̣߳�
 * ���߳���ִ��SetThreadDesktop������ָ�����棬
 * Ȼ����Դ�����ʹ��FindWindow��
 */
HWND VirtualDesktop::fromPoint(POINT pt)
{
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, "SyncEvent");
	WndInfo *pWndInfo = new WndInfo();
	ZeroMemory(pWndInfo, sizeof(WndInfo));
	pWndInfo->pVoid = this;
	pWndInfo->hEvent = hEvent;
	pWndInfo->hDesktop = hDesktop;
	pWndInfo->pt = pt;
	HANDLE hThread = CreateThread(NULL, 0,
		tpFromPoint, (LPVOID)pWndInfo, 0, NULL);
	WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);
	HWND hWnd = pWndInfo->hWnd;
	if (pWndInfo != NULL) delete pWndInfo;
	return hWnd;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

