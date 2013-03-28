#pragma once

#include "WelcomePage.h"
#include "SelectModePage.h"
#include "SettingPage.h"
#include "ConfirmPage.h"
#include "InstallingPage.h"
#include "FinishPage.h"

#include "VirtualDesktop.h"

// SilentInstallerDlg

/**
 * In fact, this is a property sheet, not a dialog.
**/
class SilentInstallerDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(SilentInstallerDlg)

public:
	SilentInstallerDlg();
	SilentInstallerDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	SilentInstallerDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~SilentInstallerDlg();

private:
	VirtualDesktop *vDesktop;	
	const CString ALT_X;
	int altX;    // Hot key id for ALT+X

protected:
	WelcomePage    wPage;
	SelectModePage mPage;
	SettingPage    sPage;
	ConfirmPage    cPage;
	InstallingPage iPage;
	FinishPage     fPage;

public:
	inline WelcomePage&    getWelcomePage()    { return wPage; }
	inline SelectModePage& getSelectModePage() { return mPage; }
	inline SettingPage&    getSettingPage()    { return sPage; }
	inline ConfirmPage&    getConfirmPage()    { return cPage; }
	inline InstallingPage& getInstallingPage() { return iPage; }
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
