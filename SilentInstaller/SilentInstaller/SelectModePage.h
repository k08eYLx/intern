#pragma once

#include "WizardPage.h"

// SelectModePage dialog

class SelectModePage : public WizardPage
{
	DECLARE_DYNAMIC(SelectModePage)

public:
	SelectModePage(CWnd* pParent = NULL);   // standard constructor
	virtual ~SelectModePage();

// Dialog Data
	enum { IDD = IDD_SELECTMODEPAGE_DIALOG };

private:
	BOOL skipSettingPage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	afx_msg void OnBnClickedFastInstallButton();
	afx_msg void OnBnClickedCustomizeInstallButton();
};
