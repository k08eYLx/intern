#pragma once


// ConfirmPage dialog

class ConfirmPage : public CPropertyPage
{
	DECLARE_DYNAMIC(ConfirmPage)

public:
	ConfirmPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConfirmPage();

// Dialog Data
	enum { IDD = IDD_CONFIRMPAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};
