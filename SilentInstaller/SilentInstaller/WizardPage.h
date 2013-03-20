#pragma once

#include "SilentInstallerDlg.h"

// WizardPage dialog

/*
 * To be the base class of all the wizard pages.
 */
class WizardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(WizardPage)

public:
	WizardPage();
	WizardPage(UINT nIDTemplate);
	virtual ~WizardPage();

protected:
	VirtualDesktop *vDesktop;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
