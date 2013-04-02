// ConfirmPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentUninstaller.h"
#include "ConfirmPage.h"

#include "SilentUninstallerDlg.h"
#include "BaiduYun.h"
#include "FileUtils.h"

// ConfirmPage dialog

IMPLEMENT_DYNAMIC(ConfirmPage, CPropertyPage)

ConfirmPage::ConfirmPage()
	: WizardPage(ConfirmPage::IDD)
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
#ifndef DEBUG
	WizardPage::OnSetActive();
	pMainDialog->SetWizardButtons(PSWIZB_NEXT);
#else
	pMainDialog->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
#endif

	return CPropertyPage::OnSetActive();
}


LRESULT ConfirmPage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	//*
	// ��Ĭ״̬�¶Կ�����������ж��
	BaiduYun byu;
	if (!(byu.uninstall(vDesktop))) {
		MessageBox("�����ǣ�\n1���������洴��ʧ�ܣ�\n2����β���ж�ش����޹���", "Failed");
	}//*/

	return CPropertyPage::OnWizardNext();
}

