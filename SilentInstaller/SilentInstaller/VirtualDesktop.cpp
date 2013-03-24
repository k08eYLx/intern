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

		CString commandLine = "C:\\Users\\lihongzhen\\AppData\\Roaming\\Baidu\\BaiduYun\\baiduyun.exe"; // "BaiduYun.exe";
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


void VirtualDesktop::findWindows(ProcedureData &pd)
{
	wndFinder.findWindows(hDesktop, &pd);
}


void VirtualDesktop::findChildWindows(HWND hWndParent, ProcedureData &pd)
{
	wndFinder.findChildWindows(hWndParent, &pd);
}


DWORD WINAPI tpFindWindow(LPVOID pParam)
{
	ThreadData *pTd = (ThreadData *)pParam;
	if (pTd == NULL || pTd->hEvent == NULL) return 1;
	::SetThreadDesktop(pTd->wi.hDesktop);
	pTd->wi.hWnd = ::FindWindow(NULL, (pTd->wi.title).c_str());
	::SetEvent(pTd->hEvent);
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
	ThreadData td;
	ZeroMemory(&td, sizeof(ThreadData));
	td.hEvent = hEvent;
	td.wi.hDesktop = hDesktop;
	td.wi.title = title;
	HANDLE hThread = CreateThread(NULL, 0,
		tpFindWindow, (LPVOID)&td, 0, NULL);
	WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);
	return td.wi.hWnd;
}


/*
 * �´���һ���̣߳�
 * ���߳���ִ��SetThreadDesktop������ָ�����棬
 * Ȼ����Դ�����ʹ��FindWindow��
 */
HWND VirtualDesktop::fromPoint(POINT pt)
{
	HWND hWnd = NULL;

	switchDesktop();
	hWnd = ::WindowFromPoint(pt);
	switchDesktop();

	// caption
    CString caption;
	DWORD dwResult = 0;
    SendMessageTimeout(hWnd, WM_GETTEXT, 256, (LPARAM)caption.GetBuffer(256),
        SMTO_ABORTIFHUNG, 100, &dwResult);
    caption.ReleaseBuffer();
	TRACE("===> %s\n", caption);

	// class name
	char clsName[MAX_PATH] = { 0 };
	::GetClassName(hWnd, clsName, sizeof(clsName) - 1);
	TRACE("%d, %d ===> %s\n", pt.x, pt.y, clsName);

	return hWnd;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

