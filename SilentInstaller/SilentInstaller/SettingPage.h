#pragma once

#include "WizardPage.h"

// SettingPage dialog

class SettingPage : public WizardPage
{
	DECLARE_DYNAMIC(SettingPage)

public:
	SettingPage();
	virtual ~SettingPage();

// Dialog Data
	enum { IDD = IDD_SETTINGPAGE_DIALOG };

private:
	CString m_strPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedBrowseButton();
	virtual LRESULT OnWizardNext();
};
