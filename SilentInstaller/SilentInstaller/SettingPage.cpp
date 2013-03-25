// SettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "SettingPage.h"
#include "afxdialogex.h"

#include "SilentInstallerDlg.h"

// SettingPage dialog

IMPLEMENT_DYNAMIC(SettingPage, CPropertyPage)

SettingPage::SettingPage()
	: WizardPage(SettingPage::IDD)
	, m_strPath(_T(""))
{

}

SettingPage::~SettingPage()
{
}

void SettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH_EDIT, m_strPath);
}


BEGIN_MESSAGE_MAP(SettingPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &SettingPage::OnBnClickedBrowseButton)
END_MESSAGE_MAP()


// SettingPage message handlers


BOOL SettingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	pMainDialog->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


void SettingPage::OnBnClickedBrowseButton()
{
	// TODO: Add your control notification handler code here
	
	BROWSEINFO bi;  
    ZeroMemory(&bi, sizeof(BROWSEINFO));  
    bi.hwndOwner = m_hWnd;  
    bi.ulFlags   = BIF_RETURNONLYFSDIRS;  
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    
	if (pidl) {
		TCHAR szFolder[MAX_PATH] = { 0 };
        if (SHGetPathFromIDList(pidl, szFolder)) {
			m_strPath = szFolder;    // ��¼��ѡ����ļ���·��
		}

        IMalloc *pMalloc = NULL;  
        if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) {
            pMalloc->Free(pidl);
            pMalloc->Release(); 
        }
    }

	UpdateData(FALSE);    // ʹ�ñ���������ݸ��¿ؼ�����ʾ������
}


LRESULT SettingPage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);     // ʹ�ÿؼ�����ʾ�����ݸ��±����������
	TRACE("\n===> %s <===\n", m_strPath);
	
	::SHCreateDirectoryEx(NULL, m_strPath, NULL);    // fully qualified path

	pMainDialog->getConfirmPage().setInstallPath(m_strPath.GetBuffer());
	m_strPath.ReleaseBuffer();

	return CPropertyPage::OnWizardNext();
}
