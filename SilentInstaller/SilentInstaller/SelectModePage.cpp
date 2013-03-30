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


BOOL SelectModePage::fastInstall(BOOL isFastInstall)
{
	int index = pMainDialog->GetActiveIndex();
	index += isFastInstall + 1;
	return pMainDialog->SetActivePage(index);
}


BEGIN_MESSAGE_MAP(SelectModePage, CPropertyPage)
	ON_BN_CLICKED(IDC_FASTINSTALL_BUTTON, &SelectModePage::OnBnClickedFastInstallButton)
	ON_BN_CLICKED(IDC_CUSTOMIZEINSTALL_BUTTON, &SelectModePage::OnBnClickedCustomizeInstallButton)
END_MESSAGE_MAP()


// SelectModePage message handlers


BOOL SelectModePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	WizardPage::OnSetActive();

	pMainDialog->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


LRESULT SelectModePage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	 * 0 to automatically advance to the next page; ¨C1 to prevent the page from changing.
	 * To jump to a page other than the next one, return the identifier of the dialog to be displayed.
	 */
	return IDD_CONFIRMPAGE_DIALOG;
}


// Skip to setting page, jump to confirm page.
void SelectModePage::OnBnClickedFastInstallButton()
{
	// TODO: Add your control notification handler code here
	fastInstall(TRUE);
}


// Jump to setting page.
void SelectModePage::OnBnClickedCustomizeInstallButton()
{
	// TODO: Add your control notification handler code here
	fastInstall(FALSE);
}
