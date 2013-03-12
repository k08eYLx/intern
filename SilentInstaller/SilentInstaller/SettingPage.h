#pragma once


// SettingPage dialog

class SettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(SettingPage)

public:
	SettingPage();
	virtual ~SettingPage();

// Dialog Data
	enum { IDD = IDD_SETTINGPAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
