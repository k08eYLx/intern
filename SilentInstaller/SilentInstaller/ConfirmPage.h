#pragma once

#include "WizardPage.h"

// ConfirmPage dialog

class ConfirmPage : public WizardPage
{
	DECLARE_DYNAMIC(ConfirmPage)

public:
	ConfirmPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConfirmPage();

// Dialog Data
	enum { IDD = IDD_CONFIRMPAGE_DIALOG };

private:
	HANDLE hEvent;
	bool isInstalled;
	string path;

public:
	void setInstallPath(string path);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};
