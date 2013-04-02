// SilentInstallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SilentUninstaller.h"
#include "SilentUninstallerDlg.h"

// SilentInstallerDlg

IMPLEMENT_DYNAMIC(SilentUninstallerDlg, CPropertySheet)

SilentUninstallerDlg::SilentUninstallerDlg()
	:CPropertySheet("", NULL, 0), ALT_X("ALT+X")
{
	// ȥ��������ť
	m_psh.dwFlags &= ~PSH_HASHELP;          // ǰ׺��PSH_
#ifdef DEBUG
	wPage.m_psp.dwFlags &= ~PSP_HASHELP;    // ǰ׺��PSP_
#endif
	cPage.m_psp.dwFlags &= ~PSP_HASHELP;
	fPage.m_psp.dwFlags &= ~PSP_HASHELP;

	/*
	 * AddPage adds the CPropertyPage object to the CPropertySheet object's list of pages 
	 * but does not actually create the window for the page. 
	 * The framework postpones creation of the window for the page until the user selects that page.
	 */
#ifdef DEBUG
	AddPage(&wPage);
#endif
	AddPage(&cPage);
	AddPage(&fPage);

	// ��Property sheet����Ϊ��ģʽ
	SetWizardMode();

	vDesktop = VirtualDesktop::getVirtualDesktop();
}

SilentUninstallerDlg::SilentUninstallerDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

SilentUninstallerDlg::SilentUninstallerDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	
}

SilentUninstallerDlg::~SilentUninstallerDlg()
{
}


BOOL SilentUninstallerDlg::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	
	/*
	 * GlobalAddAtom�����õ���ֵ��0xC000��0xFFFF֮�䣬���Լ���0xC000������RegisterHotKey�ĵ���Ҫ��
	 */
	altX = ::GlobalAddAtom(ALT_X) - 0xC000;
	/*
	 * Identifier of the hot key. No other hot key in the calling thread should have the same identifier. 
	 * An application must specify a value in the range 0x0000 through 0xBFFF. 
	 * A shared dynamic-link library (DLL) must specify a value in the range 0xC000 through 0xFFFF.
	 * Ϊ�˱�����������̬���ӿⶨ����ȼ���ͻ��һ��DLL����ʹ��GlobalAddAtom��������ȼ��ı�ʶ����
	 */
	::RegisterHotKey(m_hWnd, altX, MOD_ALT, 'X'); 	// �����ȼ�Ϊ ALT+X

	// ������������
	vDesktop->create();

	return bResult;
}


VirtualDesktop *SilentUninstallerDlg::getVirtualDesktop()
{
	return vDesktop;
}


BEGIN_MESSAGE_MAP(SilentUninstallerDlg, CPropertySheet)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)	   // �����ȼ���Ϣ
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// InstallDlg message handlers


LRESULT SilentUninstallerDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if (wParam == altX)	{    // ��ϢΪ Alt+X ʱ
		vDesktop->switchDesktop();
	}
	return TRUE;
}


/*
 * ����رհ�ť�ᷢ��WM_DESTROY��Ϣ
 */
void SilentUninstallerDlg::OnDestroy()
{
	CPropertySheet::OnDestroy();

	// TODO: Add your message handler code here
	
	// ע���ȼ�
	::UnregisterHotKey(m_hWnd, altX);

	// �Ƴ�ATOM
	::GlobalDeleteAtom(altX + 0xC000);

	// ������������
	vDesktop->destroy();
}
