// InstallingPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "InstallingPage.h"

#include "SilentInstallerDlg.h"
#include "BaiduYun.h"

// InstallingPage dialog

IMPLEMENT_DYNAMIC(InstallingPage, CPropertyPage)

InstallingPage::InstallingPage(CWnd* pParent /*=NULL*/)
: CPropertyPage(InstallingPage::IDD)
{
	isInstalled = false;
}

InstallingPage::~InstallingPage()
{
}

void InstallingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InstallingPage, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// InstallingPage message handlers


BOOL InstallingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	sheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


void InstallingPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	
	if (!isInstalled) {
		SilentInstallerDlg *pSiDlg = (SilentInstallerDlg *)GetParent();
		VirtualDesktop *vDesktop = pSiDlg->getVirtualDesktop();
	
		BaiduYun byInstaller;
		byInstaller.install(vDesktop);
		isInstalled = !isInstalled;
	}
}
