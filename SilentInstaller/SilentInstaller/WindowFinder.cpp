// WindowFinder.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "WindowFinder.h"


// WindowFinder

IMPLEMENT_DYNAMIC(WindowFinder, CWnd)

WindowFinder::WindowFinder()
{

}

WindowFinder::~WindowFinder()
{
}


bool WindowFinder::appendText(CEdit *pEdit, HWND hwnd)
{
	CWnd *pWnd = CWnd::FromHandle(hwnd);
	if (pWnd != NULL) {
		CString title;
		pWnd->GetWindowText(title);

		if (title.IsEmpty()) {
			pWnd->SendMessage(WM_GETTEXT, MAX_PATH, (LPARAM)title.GetBuffer(MAX_PATH));
			title.ReleaseBuffer();
			//*
			RECT rect;
			pWnd->GetWindowRect(&rect);

			char classname[MAX_PATH] = { 0 };
			::GetClassName(hwnd, classname, sizeof(classname) - 1);
			TRACE("%d ===> %s\n", (rect.right - rect.left), classname);//*/
			if (strcmp(classname, "BaseGui") == 0) {
				::EnumChildWindows(hwnd, EnumChildProc, (LPARAM)pEdit);
			}
		}

		if (!title.IsEmpty()) {
			title.Append("\n");
			int nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);
			pEdit->SetSel(nLength, nLength);
			pEdit->ReplaceSel(title);
		}
		return true;
	}
	return false;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, DWORD lParam)
{
	CEdit *pEdit = (CEdit *)lParam;
	if (pEdit == NULL) return FALSE;

    // 窗口是否可视
    if (!IsWindowVisible(hwnd)) return TRUE;
 
    // 窗口是否可激活
    if (!IsWindowEnabled(hwnd)) return TRUE;
 
	WindowFinder::appendText(pEdit, hwnd);
 
    return TRUE;
}

void WindowFinder::listWindows(CEdit *pEdit)
{
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)pEdit); 
}

void WindowFinder::listWindows(HDESK hDesktop, CEdit *pEdit)
{
	if (hDesktop == NULL) {
		listWindows(pEdit);
	}
	else {
		EnumDesktopWindows(hDesktop, (WNDENUMPROC)EnumWindowsProc, (LPARAM)pEdit); 
	}
}


BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	CEdit *pEdit = (CEdit *)lParam;
	WindowFinder::appendText(pEdit, hWndChild);
	
    return TRUE; 
}


void WindowFinder::listChildWindows(HWND hWndParent, CEdit *pEdit)
{
	if (hWndParent == NULL) return;
	EnumChildWindows(hWndParent, EnumChildProc, (LPARAM)pEdit);
}


BOOL CALLBACK EnumWndInfoProc(HWND hWnd, LPARAM lParam)
{
	ProcedureData *pData = (ProcedureData *)lParam;

	CWnd *pWnd = CWnd::FromHandle(hWnd);
	if (pWnd != NULL) {
		WndInfo wInfo;
		ZeroMemory(&wInfo, sizeof(WndInfo));
		wInfo.hWnd = hWnd;

		CString title;
		pWnd->GetWindowText(title);
		if (title.IsEmpty()) {
			pWnd->SendMessage(WM_GETTEXT, MAX_PATH, (LPARAM)title.GetBuffer(MAX_PATH));
			title.ReleaseBuffer();
		}

		char clsName[MAX_PATH] = { 0 };
		::GetClassName(hWnd, clsName, sizeof(clsName) - 1);
		
		if (pData->clsName.empty()) {
			RECT rect;
			pWnd->GetWindowRect(&rect);
			wInfo.length = rect.right - rect.left;
			pData->wInfos.push_back(wInfo);
		}
		else if (clsName == pData->clsName) {
			pData->wInfos.push_back(wInfo);
		}
	}
    return TRUE; 
}


void WindowFinder::findWindows(HDESK hDesktop, ProcedureData *pData)
{
	if (hDesktop != NULL) {
		EnumDesktopWindows(hDesktop, (WNDENUMPROC)EnumWndInfoProc, (LPARAM)pData); 
	}
}


BOOL CALLBACK EnumChildInfoProc(HWND hWndChild, LPARAM lParam)
{
	ProcedureData *pData = (ProcedureData *)lParam;

	CWnd *pWnd = CWnd::FromHandle(hWndChild);
	if (pWnd != NULL) {
		WndInfo wInfo;
		ZeroMemory(&wInfo, sizeof(WndInfo));
		wInfo.hWnd = hWndChild;

		CString title;
		pWnd->GetWindowText(title);
		if (title.IsEmpty()) {
			pWnd->SendMessage(WM_GETTEXT, MAX_PATH, (LPARAM)title.GetBuffer(MAX_PATH));
			title.ReleaseBuffer();
		}

		char clsName[MAX_PATH] = { 0 };
		::GetClassName(hWndChild, clsName, sizeof(clsName) - 1);
		
		if (pData->clsName.empty()) {
			RECT rect;
			pWnd->GetWindowRect(&rect);
			wInfo.length = rect.right - rect.left;
			pData->wInfos.push_back(wInfo);
		}
		else if (clsName == pData->clsName) {
			pData->wInfos.push_back(wInfo);
		}
	}
    return TRUE; 
}


void WindowFinder::findChildWindows(HWND hWndParent, ProcedureData *pData)
{
	if (hWndParent == NULL) return;
	EnumChildWindows(hWndParent, EnumChildInfoProc, (LPARAM)pData);
}


HWND WindowFinder::find(CString name)
{
	//CWnd *pWnd = CWnd::FromHandle(hWnd);
	return ::FindWindow(NULL, name);
}


BEGIN_MESSAGE_MAP(WindowFinder, CWnd)
END_MESSAGE_MAP()



// WindowFinder message handlers


