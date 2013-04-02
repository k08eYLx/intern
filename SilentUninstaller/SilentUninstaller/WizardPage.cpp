// InstallerPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentUninstaller.h"
#include "WizardPage.h"
#include "afxdialogex.h"

#include "SilentUninstallerDlg.h"

// InstallerPage dialog

SilentUninstallerDlg *WizardPage::pMainDialog = NULL;
VirtualDesktop *WizardPage::vDesktop = NULL;

IMPLEMENT_DYNAMIC(WizardPage, CPropertyPage)

WizardPage::WizardPage(UINT nIDTemplate)
	: CPropertyPage(nIDTemplate)
{
	
}

WizardPage::~WizardPage()
{
}

void WizardPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WizardPage, CPropertyPage)
END_MESSAGE_MAP()


// InstallerPage message handlers


BOOL WizardPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMainDialog == NULL || vDesktop == NULL) {
		pMainDialog = (SilentUninstallerDlg *)GetParent();
		vDesktop = pMainDialog->getVirtualDesktop();
	}

	return CPropertyPage::OnSetActive();
}
