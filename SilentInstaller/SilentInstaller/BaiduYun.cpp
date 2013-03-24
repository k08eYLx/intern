// BaiduYun.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "BaiduYun.h"


// BaiduYun

IMPLEMENT_DYNAMIC(BaiduYun, CWnd)

BaiduYun::BaiduYun()
	: PROCESS_NAME("baiduyun.exe"), APP_NAME("百度云"),
	  INSTALL_WIN_TITLE("百度云 安装"),
	  LOGIN_WIN_TITLE("欢迎使用百度云"),
	  CINSTALL_BTN_X(250), CINSTALL_BTN_Y(290),
	  PATH_CTRL_CLSNAME("Edit"),
	  INSTALL_BTN_X(450), INSTALL_BTN_Y(350),
	  FINISH_BTN_X(450), FINISH_BTN_Y(350),
	  USR_EDIT_LENGTH(163), PSWD_EDIT_LENGTH(183),
	  LOGIN_BTN_X(165), LOGIN_BTN_Y(200),
	  SYNC_PATH_EDIT_LENGTH(336),
	  NEXT_BTN_X(860), NEXT_BTN_Y(510)
{
}

BaiduYun::~BaiduYun()
{
}


bool BaiduYun::install(VirtualDesktop *vDesktop, string path)
{
	TRACE("%s ===> BaiduYun is installing...\n", __FUNCTION__);
	
	if (vDesktop != NULL) {
		this->vDesktop = vDesktop;
		hWnd = vDesktop->findWindow(INSTALL_WIN_TITLE);
		if (hWnd != NULL) {
			selectMode();
			changeSettings(path);
			complete();
			login("yang_lian_xiang@126.com", "abc123");
			config(path + "\\local");
			walkWizard();
			finish();
		}
	}

	login("yang_lian_xiang@126.com", "abc123");
	config(path + "\\local");
	
	return true;
}

void BaiduYun::selectMode()
{
	Sleep(300);

	// 自定义安装按钮
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}


void BaiduYun::changeSettings(string path)
{
	Sleep(1000);
	
	// 更改安装路径
	if (!(path.empty())) {
		ProcedureData pd;
		ZeroMemory(&pd, sizeof(ProcedureData));
		pd.clsName = PATH_CTRL_CLSNAME;
		vDesktop->findChildWindows(hWnd, pd);
		HWND hInputWnd = pd.wInfos.at(0).hWnd;

		if (hInputWnd != NULL) {
			DWORD dwResult = 0;
			// 在输入框里设置上了新内容，但是没有Update，安装目录仍然没有改变。
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)path.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			// 使用Spy++查看窗口消息流得出，WM_CHAR消息可以触发对路径数据的更新。
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);

			CString content;
			SendMessageTimeout(hInputWnd, WM_GETTEXT, 256, (LPARAM)content.GetBuffer(256), SMTO_ABORTIFHUNG, 100, &dwResult);
			content.ReleaseBuffer();
			TRACE("!!!After set path ===> %s\n", content);
		}
	}

	// 安装按钮
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::complete()
{
	Sleep(7000);

	// 完成按钮
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);

	Installer::deleteShortcuts(APP_NAME);    // 移除程序在桌面和开始菜单创建的快捷方式
}


void BaiduYun::login(string usr, string pswd)
{
	Sleep(300);

	hWnd = vDesktop->findWindow(LOGIN_WIN_TITLE);
	
	ProcedureData pd;
	//ZeroMemory(&pd, sizeof(ProcedureData));  // 清零的操作将导致不能对容器进行遍历
	vDesktop->findChildWindows(hWnd, pd);
	vector<WndInfo> &wInfos = pd.wInfos;
	vector<WndInfo>::iterator iter = wInfos.begin();
	for ( ; iter != wInfos.end(); ++iter) {
		HWND hInputWnd = iter->hWnd;
		if (USR_EDIT_LENGTH == iter->length) {        // 填充用户名
			DWORD dwResult = 0;
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)usr.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);
		}
		else if (PSWD_EDIT_LENGTH == iter->length) {  // 填充用密码
			DWORD dwResult = 0;
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)pswd.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);
		}
	}
	
	Installer::imitateLeftClick(LOGIN_BTN_X, LOGIN_BTN_Y);
}


void BaiduYun::config(string path)
{
	SHCreateDirectory(NULL, CA2W(path.c_str()));
	
	Sleep(1000);

	ProcedureData pd4Top;    // For top 窗口
	pd4Top.clsName = "BaseGui";
	vDesktop->findWindows(pd4Top);
	hWnd = pd4Top.wInfos.at(0).hWnd;

	Sleep(1000);

	ProcedureData pd4Child;
	vDesktop->findChildWindows(hWnd, pd4Child);
	vector<WndInfo> &wInfos = pd4Child.wInfos;
	vector<WndInfo>::iterator iter = wInfos.begin();
	for ( ; iter != wInfos.end(); ++iter) {
		HWND hInputWnd = iter->hWnd;
		if (SYNC_PATH_EDIT_LENGTH == iter->length) {    // 填充同步目录
			DWORD dwResult = 0;
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)path.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);
		}
	}
	
	//Installer::imitateLeftClick(NEXT_BTN_X, NEXT_BTN_Y);
}


void BaiduYun::walkWizard()
{
}

void BaiduYun::finish()
{
	Sleep(1000);
	
	//Installer::killProcess(PROCESS_NAME);    // 结束安装完成后自动运行的进程
}

BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


