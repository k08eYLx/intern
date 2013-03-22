#pragma once

#include "Installer.h"

// BaiduYun

const string APP_NAME  = "百度云";
const string WIN_TITLE = "百度云 安装";

// Customize install button，自定义安装按钮
const int CINSTALL_BTN_X = 250;
const int CINSTALL_BTN_Y = 290;

// 安装路径文本框
const int PATH_EDIT_X = 650;
const int PATH_EDIT_Y = 340;

// Install button，安装按钮
const int INSTALL_BTN_X = 450;
const int INSTALL_BTN_Y = 350;

// Finish button，完成按钮
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


