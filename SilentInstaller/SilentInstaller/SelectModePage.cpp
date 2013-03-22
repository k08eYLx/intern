// SelectModePage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "SelectModePage.h"

#include "SilentInstallerDlg.h"

// SelectModePage dialog

IMPLEMENT_DYNAMIC(SelectModePage, CPropertyPage)

SelectModePage::SelectModePage(CWnd* pParent /*=NULL*/)
	: WizardPage(SelectModePage::IDD)
{

}

SelectModePage::~SelectModePage()
{
}

void SelectModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BOOL SelectModePage::skipSettingPage()
{
	int index = pMainDialog->GetActiveIndex();
	return pMainDialog->SetActivePage(index + 2);
}


BEGIN_MESSAGE_MAP(SelectModePage, CPropertyPage)
	ON_BN_CLICKED(IDC_FASTINSTALL_BUTTON, &SelectModePage::OnBnClickedFastInstallButton)
	ON_BN_CLICKED(IDC_CUSTOMIZEINSTALL_BUTTON, &SelectModePage::OnBnClickedCustomizeInstallButton)
END_MESSAGE_MAP()


// SelectModePage message handlers


BOOL SelectModePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	pMainDialog->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


LRESULT SelectModePage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	return skipSettingPage();
}


void SelectModePage::OnBnClickedFastInstallButton()
{
	// TODO: Add your control notification handler code here
	skipSettingPage();
}


void SelectModePage::OnBnClickedCustomizeInstallButton()
{
	// TODO: Add your control notification handler code here
	int index = pMainDialog->GetActiveIndex();
	pMainDialog->SetActivePage(index + 1);
}
