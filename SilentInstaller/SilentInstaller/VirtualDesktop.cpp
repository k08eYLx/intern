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

		CString commandLine = "BaiduYun.exe";    // "explorer"
		::CreateProcess(NULL, (CT2A)commandLine, NULL, NULL, TRUE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);

		// ��ɳ�ʼ�����Ժ󲻻��ٴα����ã���ֹ���������������Դ������
		isInitialDesktop = !isInitialDesktop;
	}
}

HWND VirtualDesktop::getHwndByPid(DWORD dwPid)
{
    HWND hWnd = ::GetTopWindow(NULL);
	while (hWnd != NULL) {
		/*
		 * Retrieves the identifier of the thread that created the specified window and, 
		 * optionally, the identifier of the process that created the window.
		 */
        DWORD pid = 0;
        DWORD dwTid = GetWindowThreadProcessId(hWnd, &pid);

        if (dwTid != 0) {
            if (pid == dwPid/*input process id*/) {
                // hWnd is the handle to the window
                return hWnd;
            }
        }

        hWnd = ::GetNextWindow(hWnd , GW_HWNDNEXT);
    }

    return NULL;
}

BOOL VirtualDesktop::terminateProcess()
{
	// ��ȡ���������ϵĽ��̾��
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.dwProcessId);
	if (hProcess) {
		TRACE("%d", processInfo.dwProcessId);
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
		SwitchDesktop(hDesktop);       // �л�����������
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


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

