// WelcomePage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "WelcomePage.h"
#include "afxdialogex.h"


// WelcomePage dialog

IMPLEMENT_DYNAMIC(WelcomePage, CPropertyPage)

WelcomePage::WelcomePage()
	: CPropertyPage(WelcomePage::IDD)
{

}

WelcomePage::~WelcomePage()
{
}

void WelcomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WelcomePage, CPropertyPage)
END_MESSAGE_MAP()


// WelcomePage message handlers


BOOL WelcomePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	sheet->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

