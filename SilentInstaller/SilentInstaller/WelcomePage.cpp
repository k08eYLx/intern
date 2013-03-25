// WelcomePage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "WelcomePage.h"
#include "afxdialogex.h"

#include "WindowFinder.h"
#include "SilentInstallerDlg.h"
#include "BaiduYun.h"

// WelcomePage dialog

IMPLEMENT_DYNAMIC(WelcomePage, CPropertyPage)

WelcomePage::WelcomePage()
	: WizardPage(WelcomePage::IDD)
{
}

WelcomePage::~WelcomePage()
{
}

void WelcomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, edit);
}


BEGIN_MESSAGE_MAP(WelcomePage, CPropertyPage)
	ON_BN_CLICKED(IDC_LIST_WINDOWS_BUTTON, &WelcomePage::OnBnClickedListWindowsButton)
	ON_BN_CLICKED(IDC_LIST_CHILD_WINDOWS_BUTTON, &WelcomePage::OnBnClickedListChildWindowsButton)
END_MESSAGE_MAP()


// WelcomePage message handlers


BOOL WelcomePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	pMainDialog = (SilentInstallerDlg *)GetParent();
	vDesktop = pMainDialog->getVirtualDesktop();

	pMainDialog->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


void WelcomePage::OnBnClickedListWindowsButton()
{
	// TODO: Add your control notification handler code here
	
	//*
	WindowFinder wndFinder;
	wndFinder.listWindows(&edit);//*/

	/*
	edit.SetWindowText("");
	vDesktop->listWindows(&edit);//*/

	/*
	WindowFinder wndFinder;
	HWND hWnd = vDesktop->findWindow("百度云 安装");
	if (hWnd != NULL) {
		// Son of a bitch, I made it!
		Sleep(300);
		// 自定义安装按钮
		::SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(250, 290));
		::SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(250, 290));

		Sleep(300);
		// 可以直接枚举到Edit控件
		wndFinder.listChildWindows(hWnd, &edit);
	}//*/

	/*
	HWND hInputWnd = NULL;
	Sleep(300);
	POINT pt;
	pt.x = 650;
	pt.y = 340;
	hInputWnd = vDesktop->fromPoint(pt);
	if (hInputWnd != NULL) {
		DWORD dwResult = 0;
		::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)"D:\\BaiduYun\\", SMTO_ABORTIFHUNG, 100, &dwResult);
		::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);

		CString title;
		SendMessageTimeout(hInputWnd, WM_GETTEXT, 256, (LPARAM)title.GetBuffer(256), SMTO_ABORTIFHUNG, 100, &dwResult);
		title.ReleaseBuffer();
		edit.SetWindowText(title);
	}
	//*/
}


void WelcomePage::OnBnClickedListChildWindowsButton()
{
	// TODO: Add your control notification handler code here
	edit.SetWindowText("");
	WindowFinder wndFinder;
	HWND hWnd = wndFinder.find("百度云 安装");   // 百度云 安装
	wndFinder.listChildWindows(hWnd, &edit);
	SilentInstallerDlg *pSiDlg = (SilentInstallerDlg *)GetParent();
	pSiDlg->getVirtualDesktop()->listWindows(&edit);
	hWnd = pSiDlg->getVirtualDesktop()->findWindow("百度云 安装");
	wndFinder.listChildWindows(hWnd, &edit);
}


LRESULT WelcomePage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardNext();
}
