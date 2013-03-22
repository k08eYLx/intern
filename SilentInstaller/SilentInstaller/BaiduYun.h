#pragma once

#include "Installer.h"

// BaiduYun

const string APP_NAME  = "�ٶ���";
const string WIN_TITLE = "�ٶ��� ��װ";

// Customize install button���Զ��尲װ��ť
const int CINSTALL_BTN_X = 250;
const int CINSTALL_BTN_Y = 290;

// ��װ·���ı���
const int PATH_EDIT_X = 650;
const int PATH_EDIT_Y = 340;

// Install button����װ��ť
const int INSTALL_BTN_X = 450;
const int INSTALL_BTN_Y = 350;

// Finish button����ɰ�ť
const int FINISH_BTN_X = 450;
const int FINISH_BTN_Y = 350;


class BaiduYun : public Installer, CWnd
{
	DECLARE_DYNAMIC(BaiduYun)

public:
	BaiduYun();
	virtual ~BaiduYun();

public:
	virtual bool install(VirtualDesktop *vDesktop, string path);
	
private:
	void selectMode();
	void changeSettings(VirtualDesktop *vDesktop, string path);
	void finish();
	void login(string usr, string pswd);
	void config(string path);
	void walkWizard();

protected:
	DECLARE_MESSAGE_MAP()
};


