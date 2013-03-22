#pragma once

#include "WizardPage.h"

// InstallingPage dialog

class InstallingPage : public WizardPage
{
	DECLARE_DYNAMIC(InstallingPage)

public:
	InstallingPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~InstallingPage();

// Dialog Data
	enum { IDD = IDD_INSTALLINGPAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};
