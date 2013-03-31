// WelcomePage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "WelcomePage.h"
#include "afxdialogex.h"

#include <strsafe.h>

#include "WindowFinder.h"
#include "SilentInstallerDlg.h"
#include "FileUtils.h"
#include "BaiduYun.h"
#include "TaskbarTray.h"

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
	WizardPage::OnSetActive();

	pMainDialog->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


void WelcomePage::OnBnClickedListWindowsButton()
{
	// TODO: Add your control notification handler code here

	//SetFileAttributes("D:\\ktv\\BaiduYun", FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);

	/*
	RegistryUtils ru;
	if (!(ru.deleteTree(HKEY_CURRENT_USER, "Software\\Baidu", "BaiduHi"))) {
		MessageBox("Failed");
	}//*/

	LONG ret = 0;
	HKEY hKey = NULL;

	ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE
		, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace"
		, 0, KEY_ALL_ACCESS, &hKey);
	ret = ::RegDeleteTree(hKey, "{20D04FE0-3AEA-1069-A2D8-08002B303091}");
	ret = ::RegCloseKey(hKey);

	TRACE("===> %d\n", ret);

	/*
	string from = ".";
#ifdef DEBUG
	from.append(".\\Debug");
#endif
	FileUtils::copyRelatively(from.append("\\programs\\*"), "D:\\ktv");//*/

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
