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
	// ��װĿ¼��ע�������ƺ�ֵ����
	const string IDIR_REG_KEY;
	const string IDIR_REG_VALUE;

	const string PROCESS_NAME;

	const string UNINSTALL_WIN_TITLE;
	const string UNINSTALL_WIN_TITLE_EX;

	// Uninstall button��ж�ذ�ť
	const int UNINSTALL_BTN_X;
	const int UNINSTALL_BTN_Y;

	// Finish button����ɰ�ť
	const int FINISH_BTN_X;
	const int FINISH_BTN_Y;

public:
	// �ٶ��Ƶ����Uninstaller���õ�����Local TempĿ¼������ʱ�ļ�Au_.exe����ʽ
	virtual bool uninstall(VirtualDesktop *vDesktop);

private:
	string retrieveUninstaller();
	bool kill();

protected:
	DECLARE_MESSAGE_MAP()
};


