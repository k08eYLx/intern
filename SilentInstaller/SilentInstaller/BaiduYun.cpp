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

	// �Զ��尲װ��ť
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}

void BaiduYun::changeSettings(VirtualDesktop *vDesktop, string path)
{
	Sleep(1000);
	
	// ���İ�װ·��
	if (!(path.empty())) {
		POINT pt;
		pt.x = PATH_EDIT_X;
		pt.y = PATH_EDIT_Y;
		HWND hWnd = vDesktop->fromPoint(pt);
		if (hWnd != NULL) {
			DWORD dwResult = 0;
			// ����������������������ݣ�����û��Update����װĿ¼��Ȼû�иı䡣
			::SendMessageTimeout(hWnd, WM_SETTEXT, NULL, (LPARAM)path.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			// ʹ��Spy++�鿴������Ϣ���ó���WM_CHAR��Ϣ���Դ�����·�����ݵĸ��¡�
			::SendMessage(hWnd, WM_CHAR, NULL, NULL);

			CString content;
			SendMessageTimeout(hWnd, WM_GETTEXT, 256, (LPARAM)content.GetBuffer(256), SMTO_ABORTIFHUNG, 100, &dwResult);
			content.ReleaseBuffer();
			TRACE("!!!After set path ===> %s\n", content);
		}
	}

	// ��װ��ť
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::finish()
{
	Sleep(7000);

	// ��ɰ�ť
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
	Installer::deleteShortcuts(APP_NAME);    // �Ƴ�����������Ϳ�ʼ�˵������Ŀ�ݷ�ʽ
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


