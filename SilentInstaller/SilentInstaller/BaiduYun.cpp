// BaiduYun.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "BaiduYun.h"


// BaiduYun

IMPLEMENT_DYNAMIC(BaiduYun, CWnd)

BaiduYun::BaiduYun()
{
	 hWnd = NULL;
}

BaiduYun::~BaiduYun()
{
}


bool BaiduYun::install(VirtualDesktop *vDesktop)
{
	TRACE("%s ===> BaiduYun is installing...\n", __FUNCTION__);
	
	if (vDesktop != NULL) {
		hWnd = vDesktop->findWindow(WIN_TITLE);
		if (hWnd != NULL) {
			selectMode();
			changeSettings();
			finish();
		}
	}

	return true;
}

void BaiduYun::selectMode()
{
	Sleep(300);

	// 自定义安装按钮
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}

void BaiduYun::changeSettings()
{
	Sleep(300);
	
	// TO-DO: 更改安装路径

	// 安装按钮
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::finish()
{
	Sleep(7000);

	// 完成按钮
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
	//Installer::deleteShortcuts(APP_NAME);    // 移除程序在桌面和开始菜单创建的快捷方式
}


BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


