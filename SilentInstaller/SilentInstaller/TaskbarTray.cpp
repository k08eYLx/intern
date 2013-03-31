// TaskbarTray.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "TaskbarTray.h"


// TaskbarTray

IMPLEMENT_DYNAMIC(TaskbarTray, CWnd)

TaskbarTray::TaskbarTray()
{

}

TaskbarTray::~TaskbarTray()
{
}


// 获取普通托盘区窗口句柄
HWND TaskbarTray::findTrayWnd()
{
	HWND hWnd = NULL;
	
	if ((hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL)) != NULL) {
		if ((hWnd = ::FindWindowEx(hWnd, 0, _T("TrayNotifyWnd"), NULL)) != NULL) {
			HWND hWndPaper = ::FindWindowEx(hWnd, 0, _T("SysPager"), NULL);
			if (hWndPaper != NULL) hWnd = hWndPaper;
			hWnd = ::FindWindowEx(hWnd, 0, _T("ToolbarWindow32"), NULL);
		}
	}

	return hWnd;
}


// 获取溢出托盘区窗口句柄
HWND TaskbarTray::findNotifyIconOverflowWindow()
{
	HWND hWnd = NULL;

	hWnd = ::FindWindow(_T("NotifyIconOverflowWindow"), NULL);
	if (hWnd != NULL) {
		hWnd = ::FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);
	}
	
	return hWnd;
}


// 获取托盘区域数据
RECT TaskbarTray::getTrayRect()
{
	RECT rect = { 0 };
	HWND hWnd = NULL;

	hWnd = findTrayWnd();
	if (hWnd != NULL) {
		if (!enumNotifyWindow(rect, hWnd)) {        // 如果没在普通托盘区
			hWnd = findNotifyIconOverflowWindow();  // 在溢出区（win7）
			if (hWnd != NULL) {
				enumNotifyWindow(rect, hWnd);
			}
		}
	}

	return rect;
}


// 枚举获取托盘区域位置
bool TaskbarTray::enumNotifyWindow(RECT &rect, HWND hWnd)
{
	unsigned long lngPid = 0;
	long ret = 0, lngButtons = 0;
	long lngHwndAdr = 0, lngHwnd = 0;
	HANDLE hProcess = NULL;
	LPVOID lngAddress = NULL, lngRect = NULL;

	if (hWnd != NULL) {
		ret = GetWindowThreadProcessId(hWnd, &lngPid);
		if (ret != 0 && lngPid != 0) {
			hProcess = OpenProcess(
				PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
				0, lngPid);
			if (hProcess != NULL) {
				lngAddress = VirtualAllocEx(hProcess, 0, 0x4096, MEM_COMMIT, PAGE_READWRITE);
				lngRect = VirtualAllocEx(hProcess, 0, sizeof(RECT), MEM_COMMIT, PAGE_READWRITE);
				lngButtons = ::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);  // 发送消息获取托盘button数量
				if (lngAddress != NULL  && lngRect != NULL) {
					if (lngButtons == 0) return false;
					for (int i = 0; i < lngButtons; ++i) {
						RECT rc = { 0 };
						ret = ::SendMessage(hWnd, TB_GETBUTTON, i, long(lngAddress)); // 发送消息获取托盘项数据起始地址
						ret = ReadProcessMemory(hProcess, LPVOID(long(lngAddress) + 12), &lngHwndAdr, 4, 0);
						if (ret != 0 && lngHwndAdr != -1) {
							ret = ReadProcessMemory(hProcess, LPVOID(lngHwndAdr), &lngHwnd, 4, 0); // 获取句柄
							ret = ::SendMessage(hWnd, TB_GETITEMRECT, (WPARAM)i, (LPARAM)lngRect); // 发送消息获取托盘项区域数据
							ret = ReadProcessMemory(hProcess, lngRect, &rc, sizeof(rc), 0);        // 读取托盘区域数据
							if (ret != 0) {
								// Do something useful here.

								// FALSE show    TRUE hide
								::SendMessage(hWnd, TB_HIDEBUTTON, i, FALSE);

								CWnd::FromHandle(hWnd)->ClientToScreen(&rc);
								rect = rc;
							}
						}
					}
				}

				if (lngAddress != NULL) {
					VirtualFreeEx(hProcess, lngAddress, 0x4096, MEM_DECOMMIT);
					VirtualFreeEx(hProcess, lngAddress, 0, MEM_RELEASE);
				}
				
				if (lngRect != NULL) {
					VirtualFreeEx(hProcess, lngRect, sizeof(RECT), MEM_DECOMMIT);
					VirtualFreeEx(hProcess, lngRect, 0, MEM_RELEASE);
				}
				
				CloseHandle(hProcess);
			}
		}
	}

	return true;
}


BEGIN_MESSAGE_MAP(TaskbarTray, CWnd)
END_MESSAGE_MAP()



// TaskbarTray message handlers


