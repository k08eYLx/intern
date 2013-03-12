#pragma once


// WelcomePage dialog

class WelcomePage : public CPropertyPage
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

public:
	virtual BOOL OnSetActive();
};
