#pragma once

// InstallingPage dialog

class InstallingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(InstallingPage)

public:
	InstallingPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~InstallingPage();

// Dialog Data
	enum { IDD = IDD_INSTALLINGPAGE_DIALOG };

private:
	bool isInstalled;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
