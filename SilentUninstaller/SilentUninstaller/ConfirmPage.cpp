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
	// 静默状态下对快盘类程序进行卸载
	BaiduYun byu;
	if (!(byu.uninstall(vDesktop))) {
		MessageBox("可能是：\n1、虚拟桌面创建失败；\n2、多次查找卸载窗口无果。", "Failed");
	}//*/

	return CPropertyPage::OnWizardNext();
}

