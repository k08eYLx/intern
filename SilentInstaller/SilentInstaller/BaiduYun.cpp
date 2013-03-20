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

	// �Զ��尲װ��ť
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}

void BaiduYun::changeSettings()
{
	Sleep(300);
	
	// TO-DO: ���İ�װ·��

	// ��װ��ť
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::finish()
{
	Sleep(7000);

	// ��ɰ�ť
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
	//Installer::deleteShortcuts(APP_NAME);    // �Ƴ�����������Ϳ�ʼ�˵������Ŀ�ݷ�ʽ
}


BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


