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
	if (isInitialDesktop) {	   // 第一次运行时初始化程序
		// 创建名称为“siVirtualDesktop”的虚拟桌面（与CloseDesktop相对应）
		CString desktopName = "siVirtualDesktop";
		hDesktop = CreateDesktop(desktopName, NULL, NULL, 0, GENERIC_ALL , NULL);	

		// 初始化创建进程必须的结构体
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		ZeroMemory(&processInfo, sizeof(processInfo));
		startupInfo.cb         = sizeof(startupInfo);
		startupInfo.lpDesktop  = desktopName.GetBuffer();    // 默认桌面叫"Default"
		desktopName.ReleaseBuffer();

		// 记录原始桌面的句柄
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput  = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP);

		/*::CreateProcess(NULL, "C:\\Windows\\explorer.exe", NULL, NULL, TRUE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);*/

		CString commandLine = "C:\\Users\\lihongzhen\\AppData\\Roaming\\Baidu\\BaiduYun\\baiduyun.exe"; // "BaiduYun.exe";
		::CreateProcess(NULL, (CT2A)commandLine, NULL, NULL, TRUE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);

		// 完成初始化，以后不会再次被调用，防止打开虚拟桌面出现资源管理器
		isInitialDesktop = !isInitialDesktop;
	}
}


BOOL VirtualDesktop::terminateProcess()
{
	// 获取虚拟桌面上的进程句柄
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.dwProcessId);
	if (hProcess) {
		TRACE("%d\n", processInfo.dwProcessId);
		::TerminateProcess(hProcess, EXIT_SUCCESS);	   // 结束虚拟桌面上的进程
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
	// 关闭由本程序创建的在虚拟桌面上运行的进程
	if (vDesktop != NULL) {
		terminateProcess();
		deleteDesktop();
	}
}

/*
 * For debug only.
 * switch只需负责切换桌面，没有必要再创建或关闭Virtual Desktop
 */
void VirtualDesktop::switchDesktop()
{
	if (isOriginalDesktopActive) {	   // 是否在原始桌面
		//hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// 创建虚拟桌面
		SetThreadDesktop(hDesktop);    // 设置桌面活动焦点是虚拟桌面 
		SwitchDesktop(hDesktop);
	}
	else {
		SetThreadDesktop(hOriginalThread); // 设置桌面活动焦点是原始桌面 
		SwitchDesktop(hOriginalInput);     // 切换回原始桌面
		//CloseDesktop(hDesktop);	       // 关闭虚拟桌面
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
 * 新创建一个线程，
 * 在线程内执行SetThreadDesktop关联到指定桌面，
 * 然后针对此桌面使用FindWindow。
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
 * 新创建一个线程，
 * 在线程内执行SetThreadDesktop关联到指定桌面，
 * 然后针对此桌面使用FindWindow。
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

