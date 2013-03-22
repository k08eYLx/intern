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


bool BaiduYun::install(VirtualDesktop *vDesktop, string path)
{
	TRACE("%s ===> BaiduYun is installing...\n", __FUNCTION__);
	
	if (vDesktop != NULL) {
		hWnd = vDesktop->findWindow(WIN_TITLE);
		if (hWnd != NULL) {
			selectMode();
			changeSettings(vDesktop, path);
			finish();
			login("", "");
			config(path + "\\local");
			walkWizard();
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

void BaiduYun::changeSettings(VirtualDesktop *vDesktop, string path)
{
	Sleep(1000);
	
	// 更改安装路径
	if (!(path.empty())) {
		POINT pt;
		pt.x = PATH_EDIT_X;
		pt.y = PATH_EDIT_Y;
		HWND hWnd = vDesktop->fromPoint(pt);
		if (hWnd != NULL) {
			DWORD dwResult = 0;
			// 在输入框里设置上了新内容，但是没有Update，安装目录仍然没有改变。
			::SendMessageTimeout(hWnd, WM_SETTEXT, NULL, (LPARAM)path.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			// 使用Spy++查看窗口消息流得出，WM_CHAR消息可以触发对路径数据的更新。
			::SendMessage(hWnd, WM_CHAR, NULL, NULL);

			CString content;
			SendMessageTimeout(hWnd, WM_GETTEXT, 256, (LPARAM)content.GetBuffer(256), SMTO_ABORTIFHUNG, 100, &dwResult);
			content.ReleaseBuffer();
			TRACE("!!!After set path ===> %s\n", content);
		}
	}

	// 安装按钮
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::finish()
{
	Sleep(7000);

	// 完成按钮
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
	Installer::deleteShortcuts(APP_NAME);    // 移除程序在桌面和开始菜单创建的快捷方式
}


void BaiduYun::login(string usr, string pswd)
{
}


void BaiduYun::config(string path)
{
}


void BaiduYun::walkWizard()
{
}


BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


