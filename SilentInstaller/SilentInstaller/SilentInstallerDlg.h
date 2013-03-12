#pragma once

#include "WelcomePage.h"
#include "SettingPage.h"
#include "FinishPage.h"

#include "VirtualDesktop.h"

// SilentInstallerDlg

// Hot key id for ALT+X
const int ALT_X = 1314;    // 任意，但要唯一，否则将发生热键冲突

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

protected:
	WelcomePage wPage;
	SettingPage sPage;
	FinishPage fPage;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnHelp();
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);		// 自定义热键消息函数
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};


