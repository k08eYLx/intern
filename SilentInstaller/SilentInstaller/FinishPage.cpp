// FinishPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "FinishPage.h"
#include "afxdialogex.h"


// FinishPage dialog

IMPLEMENT_DYNAMIC(FinishPage, CPropertyPage)

FinishPage::FinishPage()
	: CPropertyPage(FinishPage::IDD)
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
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	// | ~PSWIZB_CANCEL changes next button to finish button.
	sheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

	return CPropertyPage::OnSetActive();
}
