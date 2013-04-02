#pragma once

#include "WelcomePage.h"
#include "ConfirmPage.h"
#include "FinishPage.h"

#include "VirtualDesktop.h"

// SilentInstallerDlg

/**
 * In fact, this is a property sheet, not a dialog.
**/
class SilentUninstallerDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(SilentUninstallerDlg)

public:
	SilentUninstallerDlg();
	SilentUninstallerDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	SilentUninstallerDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~SilentUninstallerDlg();

private:
	VirtualDesktop *vDesktop;	
	const CString ALT_X;
	unsigned short altX;    // Hot key id for ALT+X

protected:
	WelcomePage    wPage;
	ConfirmPage    cPage;
	FinishPage     fPage;

public:
	inline WelcomePage&    getWelcomePage()    { return wPage; }
	inline ConfirmPage&    getConfirmPage()    { return cPage; }
	inline FinishPage&     getFinishPage()     { return fPage; }


public:
	VirtualDesktop *getVirtualDesktop();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);		// 自定义热键消息函数
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
