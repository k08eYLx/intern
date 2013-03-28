// InstallingPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "InstallingPage.h"

#include "SilentInstallerDlg.h"

// InstallingPage dialog

IMPLEMENT_DYNAMIC(InstallingPage, CPropertyPage)

InstallingPage::InstallingPage(CWnd* pParent /*=NULL*/)
	: WizardPage(InstallingPage::IDD)
{
}

InstallingPage::~InstallingPage()
{
}

void InstallingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InstallingPage, CPropertyPage)
END_MESSAGE_MAP()


// InstallingPage message handlers


BOOL InstallingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	pMainDialog->SetWizardButtons(PSWIZB_FINISH);

	return CPropertyPage::OnSetActive();
}


LRESULT InstallingPage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardNext();
}

