#include "StdAfx.h"
#include "Uninstaller.h"

Uninstaller::Uninstaller(void)
	: vDesktop(NULL), hWnd(NULL),
	  WAIT_TIME_SHORT(100), WAIT_TIME_LONG(720),
	  RETRY_SEVERAL_TIMES(3), RETRY_MANY_TIMES(12)
{
}


Uninstaller::~Uninstaller(void)
{
}


void Uninstaller::imitateLeftClick(int xPos, int yPos)
{
	TRACE("%s ===> %d, %d\n", __FUNCTION__, xPos, yPos);
	LONG pos = MAKELONG(xPos, yPos);
	// Use PostMessage instead of SendMessage.
	::PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, pos);
	::PostMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, pos);
}


LRESULT Uninstaller::setText(HWND hInputWnd, string text)
{
	DWORD dwResult = 0;
	// ����������������������ݣ�����û��Update��
	::SendMessageTimeout(hInputWnd, WM_SETTEXT, NULL, (LPARAM)text.c_str()
		, SMTO_ABORTIFHUNG, WAIT_TIME_SHORT, &dwResult);
	// ʹ��Spy++�鿴������Ϣ���ó���WM_CHAR��Ϣ���Դ�����·�����ݵĸ��¡�
	::SendMessage(hInputWnd, WM_CHAR, NULL, NULL);
	return dwResult;
}


string Uninstaller::retrieveInstallDir(string keyName, string key)
{
	CString value = "";
	RegistryUtils ru;
	ru.readCuString(keyName, key.c_str(), value);
	return string(value);
}

