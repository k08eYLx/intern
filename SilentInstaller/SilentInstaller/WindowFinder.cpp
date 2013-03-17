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

		/*if (title.CompareNoCase(_T("下一步(&N) >")) == 0) {
			int id = pWnd->GetDlgCtrlID();
			::SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(250, 200));
			::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(250, 200));
		}*/

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
	if (pEdit == NULL) return FALSE;

	WindowFinder::appendText(pEdit, hWndChild);

    return TRUE; 
}

void WindowFinder::listChildWindows(HWND hWndParent, CEdit *pEdit)
{
	if (hWndParent == NULL) return;
	EnumChildWindows(hWndParent, EnumChildProc, (LPARAM)pEdit);
}

HWND WindowFinder::find(CString name)
{
	//CWnd *pWnd = CWnd::FromHandle(hWnd);
	return ::FindWindow(NULL, name);
}

HWND WindowFinder::findChild(CString name)
{
	return NULL;
}


BEGIN_MESSAGE_MAP(WindowFinder, CWnd)
END_MESSAGE_MAP()



// WindowFinder message handlers


