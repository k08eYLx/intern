// SettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "SettingPage.h"
#include "afxdialogex.h"


// SettingPage dialog

IMPLEMENT_DYNAMIC(SettingPage, CPropertyPage)

SettingPage::SettingPage()
	: CPropertyPage(SettingPage::IDD)
{

}

SettingPage::~SettingPage()
{
}

void SettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingPage, CPropertyPage)
END_MESSAGE_MAP()


// SettingPage message handlers


BOOL SettingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	sheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}
