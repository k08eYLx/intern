// ConfirmPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "ConfirmPage.h"
#include "afxdialogex.h"


// ConfirmPage dialog

IMPLEMENT_DYNAMIC(ConfirmPage, CPropertyPage)

ConfirmPage::ConfirmPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(ConfirmPage::IDD)
{

}

ConfirmPage::~ConfirmPage()
{
}

void ConfirmPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConfirmPage, CPropertyPage)
END_MESSAGE_MAP()


// ConfirmPage message handlers


BOOL ConfirmPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	sheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


LRESULT ConfirmPage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardNext();
}
