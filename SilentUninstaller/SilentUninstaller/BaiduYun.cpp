// BaiduYun.cpp : implementation file
//

#include "stdafx.h"
#include "SilentUninstaller.h"
#include "BaiduYun.h"

#include "FileUtils.h"


// BaiduYun

IMPLEMENT_DYNAMIC(BaiduYun, CWnd)

BaiduYun::BaiduYun()
	: IDIR_REG_KEY("Software\\Baidu\\BaiduYun"), 
	  IDIR_REG_VALUE("InstallDir"),
	  PROCESS_NAME("Au_.exe"),
	  UNINSTALL_WIN_TITLE("百度云 卸载"),      // 有空格
	  UNINSTALL_WIN_TITLE_EX("百度云 卸载 "),  // 末尾还有一个空格
	  UNINSTALL_BTN_X(370), UNINSTALL_BTN_Y(350),
	  FINISH_BTN_X(450), FINISH_BTN_Y(350)
{
}

BaiduYun::~BaiduYun()
{
}


bool BaiduYun::uninstall(VirtualDesktop *vDesktop)
{
	if (vDesktop == NULL) return false;
	
	TRACE("%s ===> BaiduYun is uninstalling...\n", __FUNCTION__);
	this->vDesktop = vDesktop;

	string process = retrieveUninstaller();
	if (process.empty()) return true;
	if (!(vDesktop->runProcess(process))) return false;
	
	//*
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		// hWnd 保存了卸载程序主窗口的句柄
		hWnd = vDesktop->findWindow(UNINSTALL_WIN_TITLE);
		if (hWnd != NULL) {
			Uninstaller::imitateLeftClick(UNINSTALL_BTN_X, UNINSTALL_BTN_Y);
			
			for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
				TRACE("===> %d\n", i);
				hWnd = vDesktop->findWindow(UNINSTALL_WIN_TITLE_EX);
				if (hWnd == NULL) {
					TRACE("if ===> %d\n", i);
					Sleep(WAIT_TIME_LONG);
				}
				else {  // 如果再次查找到卸载窗口，则认为卸载已完成
					Uninstaller::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
					break;
				}
			}

			// 结束explorer进程，会此进程自动重启
			ProcessUtils::kill("explorer.exe");

			return kill();
		}
		else Sleep(WAIT_TIME_SHORT);
	}//*/
	
	return false;
}


string BaiduYun::retrieveUninstaller()
{
	char process[MAX_PATH] = { 0 };
	string installDir = "";

	installDir = Uninstaller::retrieveInstallDir(IDIR_REG_KEY, IDIR_REG_VALUE);
	if (!(installDir.empty())) {
		sprintf_s(process, "%s\\uninst.exe", installDir.c_str());
	}
	
	return process;
}


bool BaiduYun::kill()
{
	Sleep(WAIT_TIME_LONG);
	return ProcessUtils::kill(PROCESS_NAME);
}


BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


