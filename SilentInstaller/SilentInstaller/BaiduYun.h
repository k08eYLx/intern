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

	// Login button, ��½��ť
	const int LOGIN_BTN_X;
	const int LOGIN_BTN_Y;

	// ���ñ���ͬ���ļ���·���Ŀؼ�
	const int SYNC_PATH_EDIT_LENGTH;    // 336

	// Next button, ��һ����ť
	const int NEXT_BTN_X;
	const int NEXT_BTN_Y;

public:
	virtual bool install(VirtualDesktop *vDesktop, string path);
	
private:
	void selectMode();
	void changeSettings(string path);
	void complete();    // ��ɰ�װ
	void login(string usr, string pswd);
	void config(string path);
	void walkWizard();
	void finish();     // ����������װ�����õȹ���

protected:
	DECLARE_MESSAGE_MAP()
};


