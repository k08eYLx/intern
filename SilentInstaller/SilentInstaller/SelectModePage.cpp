// SelectModePage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "SelectModePage.h"
#include "afxdialogex.h"


// SelectModePage dialog

IMPLEMENT_DYNAMIC(SelectModePage, CPropertyPage)

SelectModePage::SelectModePage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(SelectModePage::IDD)
{

}

SelectModePage::~SelectModePage()
{
}

void SelectModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SelectModePage, CPropertyPage)
	ON_BN_CLICKED(IDC_FASTINSTALL_BUTTON, &SelectModePage::OnBnClickedFastInstallButton)
	ON_BN_CLICKED(IDC_CUSTOMIZEINSTALL_BUTTON, &SelectModePage::OnBnClickedCustomizeInstallButton)
END_MESSAGE_MAP()


// SelectModePage message handlers


BOOL SelectModePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet *sheet = (CPropertySheet *)GetParent();
	sheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


LRESULT SelectModePage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardNext();
}


void SelectModePage::OnBnClickedFastInstallButton()
{
	// TODO: Add your control notification handler code here
	CPropertySheet *pSheet = (CPropertySheet *)GetParent();
	int index = pSheet->GetActiveIndex();
	pSheet->SetActivePage(index + 2);
}


void SelectModePage::OnBnClickedCustomizeInstallButton()
{
	// TODO: Add your control notification handler code here
	CPropertySheet *pSheet = (CPropertySheet *)GetParent();
	int index = pSheet->GetActiveIndex();
	pSheet->SetActivePage(index + 1);
}
