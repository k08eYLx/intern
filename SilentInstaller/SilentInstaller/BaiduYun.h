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

	const string EXIST_WIN_TITLE;       // �Ѱ�װʱ�����Ĵ���д����APP_NAME������ʵ�ʲ���
	const string INSTALL_WIN_TITLE;
	const string LOGIN_WIN_TITLE;

	// MessageBox �ϵ� "ȷ��" �� "��" ��ť
	const int OY_BTN_X;                 // 265
	const int OY_BTN_Y;                 // 175

	// MessageBox �ϵ� "ȡ��" �� "��" ��ť 
	const int CN_BTN_X;                 // 325
	const int CN_BTN_Y;                 // 175

	// Customize install button���Զ��尲װ��ť
	const int CINSTALL_BTN_X;
	const int CINSTALL_BTN_Y;

	// ��װ·���ı���
	const string PATH_CTRL_CLSNAME;

	// Install button����װ��ť
	const int INSTALL_BTN_X;
	const int INSTALL_BTN_Y;

	// Finish button����ɰ�ť
	const int FINISH_BTN_X;
	const int FINISH_BTN_Y;

	// ò��ֻ��ͨ�������������������ؼ���
	const int USR_EDIT_LENGTH;          // 163
	const int PSWD_EDIT_LENGTH;         // 183

	// ��ס����CheckBox
	const int REM_PSWD_CB_X;            // 130
	const int REM_PSWD_CB_Y;            // 180

	// Login button, ��½��ť
	const int LOGIN_BTN_X;
	const int LOGIN_BTN_Y;

	const string CONFIG_WND_CLSNAME;
	const int    CONFIG_WND_WIDTH;      // ���ñ���ͬ���ļ���·���Ĵ��ڿ��

	// ���ñ���ͬ���ļ���·���Ŀؼ�
	const int SYNC_PATH_EDIT_LENGTH;    // 336

	// Next button, ��һ����ť
	const int NEXT_BTN_X;
	const int NEXT_BTN_Y;

	// ��ʾ�������ڸ�ѡ��
	const int SHOW_FLOAT_WND_X;         // 370
	const int SHOW_FLOAT_WND_Y;         // 280

	// ����ȷ����ť
	const int CONFIG_OK_BTN_X;          // 410
	const int CONFIG_OK_BTN_Y;          // 320

	// ������������
	const string WIZARD_NAME;

	// �����򵼰�ť
	const int SKIP_BTN_X;               // 330
	const int SKIP_BTN_Y;               // 320

public:
	virtual bool install(VirtualDesktop *vDesktop, string path);
	
private:
	bool isInstalled();
	void selectMode();
	void changeSettings(string path);
	void complete();            // ��ɰ�װ
	void login(string usr, string pswd);
	void config(string path);
	HWND findConfigWnd();
	void configSyncDir(string path);
	void confirmUseExistDir();  // ���Ѵ���Ŀ¼��Ϊͬ��Ŀ¼
	void walkWizard();          // �˴�ֱ�������򵼣�Ҳ������ȫ���ô���
	void finish();              // ����������װ�����õȹ���

public:
	void kill();

protected:
	DECLARE_MESSAGE_MAP()
};


