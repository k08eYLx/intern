#pragma once

#include "Installer.h"

// BaiduYun

const string APP_NAME  = "百度云";
const string WIN_TITLE = "百度云 安装";

// Customize install button，自定义安装按钮
const int CINSTALL_BTN_X = 250;
const int CINSTALL_BTN_Y = 290;

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
	virtual bool install(VirtualDesktop *vDesktop);
	
private:
	void selectMode();
	void changeSettings();
	void finish();

protected:
	DECLARE_MESSAGE_MAP()
};


