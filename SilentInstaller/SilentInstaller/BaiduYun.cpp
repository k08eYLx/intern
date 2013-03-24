// BaiduYun.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "BaiduYun.h"


// BaiduYun

IMPLEMENT_DYNAMIC(BaiduYun, CWnd)

BaiduYun::BaiduYun()
	: PROCESS_NAME("baiduyun.exe"), APP_NAME("�ٶ���"),
	  INSTALL_WIN_TITLE("�ٶ��� ��װ"),
	  LOGIN_WIN_TITLE("��ӭʹ�ðٶ���"),
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

	// �Զ��尲װ��ť
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}


void BaiduYun::changeSettings(string path)
{
	Sleep(1000);
	
	// ���İ�װ·��
	if (!(path.empty())) {
		ProcedureData pd;
		ZeroMemory(&pd, sizeof(ProcedureData));
		pd.clsName = PATH_CTRL_CLSNAME;
		vDesktop->findChildWindows(hWnd, pd);
		HWND hInputWnd = pd.wInfos.at(0).hWnd;

		if (hInputWnd != NULL) {
			DWORD dwResult = 0;
			// ����������������������ݣ�����û��Update����װĿ¼��Ȼû�иı䡣
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)path.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			// ʹ��Spy++�鿴������Ϣ���ó���WM_CHAR��Ϣ���Դ�����·�����ݵĸ��¡�
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);

			CString content;
			SendMessageTimeout(hInputWnd, WM_GETTEXT, 256, (LPARAM)content.GetBuffer(256), SMTO_ABORTIFHUNG, 100, &dwResult);
			content.ReleaseBuffer();
			TRACE("!!!After set path ===> %s\n", content);
		}
	}

	// ��װ��ť
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}

void BaiduYun::complete()
{
	Sleep(7000);

	// ��ɰ�ť
	Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);

	Installer::deleteShortcuts(APP_NAME);    // �Ƴ�����������Ϳ�ʼ�˵������Ŀ�ݷ�ʽ
}


void BaiduYun::login(string usr, string pswd)
{
	Sleep(300);

	hWnd = vDesktop->findWindow(LOGIN_WIN_TITLE);
	
	ProcedureData pd;
	//ZeroMemory(&pd, sizeof(ProcedureData));  // ����Ĳ��������²��ܶ��������б���
	vDesktop->findChildWindows(hWnd, pd);
	vector<WndInfo> &wInfos = pd.wInfos;
	vector<WndInfo>::iterator iter = wInfos.begin();
	for ( ; iter != wInfos.end(); ++iter) {
		HWND hInputWnd = iter->hWnd;
		if (USR_EDIT_LENGTH == iter->length) {        // ����û���
			DWORD dwResult = 0;
			::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)usr.c_str(), SMTO_ABORTIFHUNG, 100, &dwResult);
			::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);
		}
		else if (PSWD_EDIT_LENGTH == iter->length) {  // ���������
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

	ProcedureData pd4Top;    // For top ����
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
		if (SYNC_PATH_EDIT_LENGTH == iter->length) {    // ���ͬ��Ŀ¼
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
	
	//Installer::killProcess(PROCESS_NAME);    // ������װ��ɺ��Զ����еĽ���
}

BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


