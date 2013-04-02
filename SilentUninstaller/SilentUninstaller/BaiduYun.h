#pragma once

#include "Uninstaller.h"

// BaiduYun

class BaiduYun : public Uninstaller, CWnd
{
	DECLARE_DYNAMIC(BaiduYun)

public:
	BaiduYun();
	virtual ~BaiduYun();

// constants
private:
	// 安装目录的注册表键名称和值名称
	const string IDIR_REG_KEY;
	const string IDIR_REG_VALUE;

	const string PROCESS_NAME;

	const string UNINSTALL_WIN_TITLE;
	const string UNINSTALL_WIN_TITLE_EX;

	// Uninstall button，卸载按钮
	const int UNINSTALL_BTN_X;
	const int UNINSTALL_BTN_Y;

	// Finish button，完成按钮
	const int FINISH_BTN_X;
	const int FINISH_BTN_Y;

public:
	// 百度云的这个Uninstaller采用的是在Local Temp目录创建临时文件Au_.exe的形式
	virtual bool uninstall(VirtualDesktop *vDesktop);

private:
	string retrieveUninstaller();
	bool kill();

protected:
	DECLARE_MESSAGE_MAP()
};


