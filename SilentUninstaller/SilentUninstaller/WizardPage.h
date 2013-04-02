#pragma once

#include "VirtualDesktop.h"

// WizardPage dialog
class SilentUninstallerDlg;

/*
 * To be the base class of all the wizard pages.
 */
class WizardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(WizardPage)

public:
	WizardPage(UINT nIDTemplate);
	virtual ~WizardPage();

protected:
	static SilentUninstallerDlg *pMainDialog;
	static VirtualDesktop *vDesktop;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
