// BaiduYun.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "BaiduYun.h"

#include "FileUtils.h"


// BaiduYun

IMPLEMENT_DYNAMIC(BaiduYun, CWnd)

BaiduYun::BaiduYun()
	: PROCESS_NAME("baiduyun.exe"), APP_NAME("�ٶ���"),
	  EXIST_WIN_TITLE("�ٶ��ư�װ"),     // û�пո�
	  INSTALL_WIN_TITLE("�ٶ��� ��װ"),  // �пո�
	  LOGIN_WIN_TITLE("��ӭʹ�ðٶ���"),
	  OY_BTN_X(265), OY_BTN_Y(175),
	  CN_BTN_X(325), CN_BTN_Y(175),
	  CINSTALL_BTN_X(250), CINSTALL_BTN_Y(290),
	  PATH_CTRL_CLSNAME("Edit"),
	  INSTALL_BTN_X(450), INSTALL_BTN_Y(350),
	  FINISH_BTN_X(450), FINISH_BTN_Y(350),
	  USR_EDIT_LENGTH(163), PSWD_EDIT_LENGTH(183),
	  REM_PSWD_CB_X(130), REM_PSWD_CB_Y(180),
	  LOGIN_BTN_X(165), LOGIN_BTN_Y(200),
	  CONFIG_WND_CLSNAME("BaseGui"), CONFIG_WND_WIDTH(452),
	  SYNC_PATH_EDIT_LENGTH(336),
	  NEXT_BTN_X(410), NEXT_BTN_Y(320),
	  SHOW_FLOAT_WND_X(367), SHOW_FLOAT_WND_Y(280),
	  CONFIG_OK_BTN_X(410), CONFIG_OK_BTN_Y(320),
	  WIZARD_NAME("��������"), SKIP_BTN_X(330), SKIP_BTN_Y(320)
{
}

BaiduYun::~BaiduYun()
{
}


bool BaiduYun::install(VirtualDesktop *vDesktop, string path)
{
	if (vDesktop == NULL) return false;
	
	TRACE("%s ===> BaiduYun is installing...\n", __FUNCTION__);
	this->vDesktop = vDesktop;

	if (isInstalled()) return false;

	//*
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		// hWnd �����˰�װ���������ڵľ��
		hWnd = vDesktop->findWindow(INSTALL_WIN_TITLE);
		if (hWnd != NULL) {
			selectMode();
			changeSettings(path);  // ���ܰ�װ��C���У�����Ĭ��λ�������·��
			complete();
			login("yang_lian_xiang@126.com", "abc123");
			config(path + "\\Local");
			walkWizard();
			finish();
			return true;
		}
		else Sleep(WAIT_TIME_SHORT);
	}//*/
	
	return false;
}


bool BaiduYun::isInstalled()
{
	for (int i = 0; i < RETRY_SEVERAL_TIMES; ++i) {
		// hWnd �����˰�װ���򵯳����Ѱ�װ��Ϣ���ھ��
		hWnd = vDesktop->findWindow(EXIST_WIN_TITLE);
		if (hWnd != NULL) {
			Installer::imitateLeftClick(CN_BTN_X, CN_BTN_Y);
			return true;
		}
		else Sleep(WAIT_TIME_SHORT);
	}
	return false;
}


string BaiduYun::retrieveInstalledDir()
{
	return Installer::retrieveInstalledDir("Software\\Baidu\\BaiduYun", "InstallDir");
}


void BaiduYun::selectMode()
{
	// �Զ��尲װ��ť
	Installer::imitateLeftClick(CINSTALL_BTN_X, CINSTALL_BTN_Y);
}


void BaiduYun::changeSettings(string path)
{
	// ���İ�װ·��
	if (!(path.empty())) {
		for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
			ProcedureData pd;
			ZeroMemory(&pd, sizeof(ProcedureData));
			pd.clsName = PATH_CTRL_CLSNAME;
			vDesktop->findChildWindows(hWnd, pd);
			if (pd.wInfos.size() > 0) {
				HWND hInputWnd = pd.wInfos.at(0).hWnd;
				if (hInputWnd != NULL) {
					Installer::setText(hInputWnd, path);
				}
				break;
			}
			else Sleep(WAIT_TIME_SHORT);
		}
	}

	// ��װ��ť
	Installer::imitateLeftClick(INSTALL_BTN_X, INSTALL_BTN_Y);
}


// ��װ���ֵ����
void BaiduYun::complete()
{
	DWORD timeToSleep = 0;
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		if (FileUtils::isDesktopShortcutExist(APP_NAME)) {
			timeToSleep = WAIT_TIME_LONG * (i + 1);
			break;
		}
		else Sleep(WAIT_TIME_LONG);
	}
	// �Ƴ�����������Ϳ�ʼ�˵������Ŀ�ݷ�ʽ
	FileUtils::deleteShortcuts(APP_NAME);
	TRACE("Shortcuts ===> %d\n", timeToSleep);    // �����ɿ�ݷ�ʽǰ�ȴ���ʱ��
	
	DWORD timeSleeped = 0;
	HWND hLoginWnd = NULL;
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		hLoginWnd = vDesktop->findWindow(LOGIN_WIN_TITLE);
		if (hLoginWnd != NULL) {
			timeSleeped = WAIT_TIME_LONG * (i + 1);
			break;
		}
		else {
			Sleep(timeToSleep);
			// ��ɰ�ť
			Installer::imitateLeftClick(FINISH_BTN_X, FINISH_BTN_Y);
		}
	}
	TRACE("Login ===> %d\n", timeSleeped);
	
	// hWnd ��Ϊ�����¼���ڵľ��
	hWnd = hLoginWnd;
}


void BaiduYun::login(string usr, string pswd)
{
	for (int i = 0; i < RETRY_SEVERAL_TIMES; ++i) {
		ProcedureData pd;
		//ZeroMemory(&pd, sizeof(ProcedureData));  // ����Ĳ��������²��ܶ��������б���
		vDesktop->findChildWindows(hWnd, pd);
		vector<WndInfo> &wInfos = pd.wInfos;
		if (wInfos.size() > 0) {		
			vector<WndInfo>::iterator iter = wInfos.begin();
			for ( ; iter != wInfos.end(); ++iter) {
				HWND hInputWnd = iter->hWnd;
				if (USR_EDIT_LENGTH == iter->length) {        // ����û���
					Installer::setText(hInputWnd, usr);
				}
				else if (PSWD_EDIT_LENGTH == iter->length) {  // ���������
					Installer::setText(hInputWnd, pswd);
				}
			}
			break;
		}
		else Sleep(WAIT_TIME_SHORT);
	}
	
	/*
	 * Uncheck the remember pswd checkbox.
	 * ò��ȥ����ס�����check�ᵼ���Ժ��޷�����������WM_SETTEXT
	 */
	//Installer::imitateLeftClick(REM_PSWD_CB_X, REM_PSWD_CB_Y);
	
	Installer::imitateLeftClick(LOGIN_BTN_X, LOGIN_BTN_Y);
}


void BaiduYun::config(string path)
{
	SHCreateDirectoryEx(NULL, path.c_str(), NULL);
	
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		// hWnd ��Ϊ�������ô��ڵľ��
		hWnd = findConfigWnd();
		if (hWnd != NULL) break;
		else Sleep(WAIT_TIME_LONG);
	}

	configSyncDir(path);
	Installer::imitateLeftClick(NEXT_BTN_X, NEXT_BTN_Y);
	confirmUseExistDir();
	
	Sleep(WAIT_TIME_LONG);

	/* ûЧ����WHY???
	Installer::imitateLeftClick(SHOW_FLOAT_WND_X, SHOW_FLOAT_WND_Y);//*/

	Installer::imitateLeftClick(CONFIG_OK_BTN_X, CONFIG_OK_BTN_Y);
}


HWND BaiduYun::findConfigWnd()
{
	HWND hConfigWnd = NULL;

	ProcedureData pd4Top;    // For top ����
	pd4Top.clsName = CONFIG_WND_CLSNAME;
	vDesktop->findWindows(pd4Top);
	vector<WndInfo> &wInfos = pd4Top.wInfos;
	vector<WndInfo>::iterator iter = wInfos.begin();
	for ( ; iter != wInfos.end(); ++iter) {
		if (CONFIG_WND_WIDTH == iter->length) {
			hConfigWnd = iter->hWnd;
			break;
		}
	}

	return hConfigWnd;
}


void BaiduYun::configSyncDir(string path)
{
	ProcedureData pd4Child;  // For child �ؼ�
	
	for (int i = 0; i < RETRY_MANY_TIMES; ++i) {
		vDesktop->findChildWindows(hWnd, pd4Child);
		if (pd4Child.wInfos.size() > 0) break;
		else Sleep(WAIT_TIME_SHORT);
	}

	vector<WndInfo> &wInfos = pd4Child.wInfos;
	vector<WndInfo>::iterator iter = wInfos.begin();
	for ( ; iter != wInfos.end(); ++iter) {
		HWND hInputWnd = iter->hWnd;
		if (SYNC_PATH_EDIT_LENGTH == iter->length) {    // ���ͬ��Ŀ¼
			Installer::setText(hInputWnd, path);
		}
	}
}


void BaiduYun::confirmUseExistDir()
{
	Sleep(WAIT_TIME_LONG);
	HWND hTmpWnd = hWnd;
	hWnd = vDesktop->findWindow(APP_NAME);
	if (hWnd != NULL) {
		Installer::imitateLeftClick(OY_BTN_X, OY_BTN_Y);
	}
	hWnd = hTmpWnd;
}


void BaiduYun::walkWizard()
{
	Sleep(WAIT_TIME_LONG);
	hWnd = vDesktop->findWindow(WIZARD_NAME);
	if (hWnd != NULL) {
		Installer::imitateLeftClick(SKIP_BTN_X, SKIP_BTN_Y);
		Sleep(WAIT_TIME_SHORT);
		hWnd = vDesktop->findWindow(APP_NAME);
		Installer::imitateLeftClick(OY_BTN_X, OY_BTN_Y);
	}
}


void BaiduYun::kill()
{
	Sleep(WAIT_TIME_LONG);
	ProcessUtils::kill(PROCESS_NAME);
}


void BaiduYun::finish()
{
	// Hide the floating window, just be effectable to this single time.
	Sleep(WAIT_TIME_LONG);
	hWnd = vDesktop->findWindow(APP_NAME);
	if (hWnd != NULL) {
		/*// Show menu
		::PostMessage(hWnd, WM_RBUTTONDOWN, NULL, NULL);
		::PostMessage(hWnd, WM_RBUTTONUP, NULL, NULL);//*/	
		::ShowWindow(hWnd, SW_HIDE);
	}

	/*
	 * ������װ��ɺ��Զ����еĽ��̣�
	 * �����ڴ˴�ִ��ò�ƻᵼ���ٴ�����ʱ��
	 * ���µ��������򵼡�
	 *
	 * ���Ը�Ϊ��Finish Page��finish��ť��ִ��kill�����ˡ�
	 * kill();
	 */
}


BEGIN_MESSAGE_MAP(BaiduYun, CWnd)
END_MESSAGE_MAP()



// BaiduYun message handlers


