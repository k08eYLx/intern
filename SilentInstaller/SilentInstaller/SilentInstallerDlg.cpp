// SilentInstallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "SilentInstallerDlg.h"

// SilentInstallerDlg

IMPLEMENT_DYNAMIC(SilentInstallerDlg, CPropertySheet)

SilentInstallerDlg::SilentInstallerDlg()
	:CPropertySheet("", NULL, 0)
{
	AddPage(&wPage);
	AddPage(&sPage);
	AddPage(&fPage);

	SetWizardMode();

	vDesktop = VirtualDesktop::getVirtualDesktop();
	vDesktop->create();
}

SilentInstallerDlg::SilentInstallerDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

SilentInstallerDlg::SilentInstallerDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	
}

SilentInstallerDlg::~SilentInstallerDlg()
{
}


BOOL SilentInstallerDlg::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	/*
	 * Identifier of the hot key. No other hot key in the calling thread should have the same identifier. 
	 * An application must specify a value in the range 0x0000 through 0xBFFF. 
	 * A shared dynamic-link library (DLL) must specify a value in the range 0xC000 through 0xFFFF.
	 */
	::RegisterHotKey(m_hWnd, ALT_X, MOD_ALT, 'X'); 	// 设置热键为 ALT+X

	return bResult;
}


BEGIN_MESSAGE_MAP(SilentInstallerDlg, CPropertySheet)
	ON_COMMAND(IDHELP, &SilentInstallerDlg::OnHelp)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)	   // 链接热键消息
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// InstallDlg message handlers


void SilentInstallerDlg::OnHelp()
{
	// TODO: Add your command handler code here
	return;
}


LRESULT SilentInstallerDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if (wParam == ALT_X)		// 消息为 Alt+X 时
	{
		vDesktop->switchDesktop();
	}
	return TRUE;
}


void SilentInstallerDlg::OnDestroy()
{
	CPropertySheet::OnDestroy();

	// TODO: Add your message handler code here
	
	// 注销热键
	UnregisterHotKey(m_hWnd, ALT_X);
	vDesktop->destroy();
}

