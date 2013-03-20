// InstallerPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "WizardPage.h"
#include "afxdialogex.h"


// InstallerPage dialog

IMPLEMENT_DYNAMIC(WizardPage, CPropertyPage)

WizardPage::WizardPage(UINT nIDTemplate)
	: CPropertyPage(nIDTemplate)
{
	vDesktop = ((SilentInstallerDlg *)GetParent())->getVirtualDesktop();
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
