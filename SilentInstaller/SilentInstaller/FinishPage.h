#pragma once


// FinishPage dialog

class FinishPage : public CPropertyPage
{
	DECLARE_DYNAMIC(FinishPage)

public:
	FinishPage();
	virtual ~FinishPage();

// Dialog Data
	enum { IDD = IDD_FINISHPAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
