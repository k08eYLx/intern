// FinishPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "FinishPage.h"
#include "afxdialogex.h"

#include "SilentInstallerDlg.h"

// FinishPage dialog

IMPLEMENT_DYNAMIC(FinishPage, CPropertyPage)

FinishPage::FinishPage()
	: WizardPage(FinishPage::IDD)
{
}

FinishPage::~FinishPage()
{
}

void FinishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FinishPage, CPropertyPage)
END_MESSAGE_MAP()


// FinishPage message handlers


BOOL FinishPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	pMainDialog->SetWizardButtons(PSWIZB_FINISH);

	return CPropertyPage::OnSetActive();
}
