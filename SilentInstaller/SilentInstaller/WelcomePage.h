#pragma once

#include "WizardPage.h"

// WelcomePage dialog

// Most of the time, this page is just for test.

class WelcomePage : public WizardPage
{
	DECLARE_DYNAMIC(WelcomePage)

public:
	WelcomePage();
	virtual ~WelcomePage();

// Dialog Data
	enum { IDD = IDD_WELCOMEPAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CEdit edit;

private:
	HWND findWindow(POINT point);

public:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedListWindowsButton();
	afx_msg void OnBnClickedListChildWindowsButton();
	virtual LRESULT OnWizardNext();
};
