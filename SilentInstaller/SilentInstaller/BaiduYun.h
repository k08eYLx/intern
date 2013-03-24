#pragma once

#include "Installer.h"

// BaiduYun

class BaiduYun : public Installer, CWnd
{
	DECLARE_DYNAMIC(BaiduYun)

public:
	BaiduYun();
	virtual ~BaiduYun();

// constants
private:
	const string PROCESS_NAME;

	const string APP_NAME;
	const string INSTALL_WIN_TITLE;
	const string LOGIN_WIN_TITLE;

	// Customize install button，自定义安装按钮
	const int CINSTALL_BTN_X;
	const int CINSTALL_BTN_Y;

	// 安装路径文本框
	const string PATH_CTRL_CLSNAME;

	// Install button，安装按钮
	const int INSTALL_BTN_X;
	const int INSTALL_BTN_Y;

	// Finish button，完成按钮
	const int FINISH_BTN_X;
	const int FINISH_BTN_Y;

	// 貌似只能通过长度来区分这两个控件了
	const int USR_EDIT_LENGTH;          // 163
	const int PSWD_EDIT_LENGTH;         // 183

	// Login button, 登陆按钮
	const int LOGIN_BTN_X;
	const int LOGIN_BTN_Y;

	// 设置本地同步文件夹路径的控件
	const int SYNC_PATH_EDIT_LENGTH;    // 336

	// Next button, 下一步按钮
	const int NEXT_BTN_X;
	const int NEXT_BTN_Y;

public:
	virtual bool install(VirtualDesktop *vDesktop, string path);
	
private:
	void selectMode();
	void changeSettings(string path);
	void complete();    // 完成安装
	void login(string usr, string pswd);
	void config(string path);
	void walkWizard();
	void finish();     // 结束整个安装及设置等过程

protected:
	DECLARE_MESSAGE_MAP()
};


