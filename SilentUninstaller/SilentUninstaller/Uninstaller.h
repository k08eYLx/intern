#pragma once

#include "VirtualDesktop.h"
#include "RegistryUtils.h"
#include "ProcessUtils.h"

// abstract as base class
class Uninstaller
{
public:
	Uninstaller(void);
	virtual ~Uninstaller(void);

public:
	virtual bool uninstall(VirtualDesktop *vDesktop) = 0;
	
protected:
	const int WAIT_TIME_SHORT;          // Sleep��ʱ����ms���϶�
	const int WAIT_TIME_LONG;           // Sleep��ʱ����ms���ϳ�
	const int RETRY_SEVERAL_TIMES;      // ���Ҵ������ԵĴ���
	const int RETRY_MANY_TIMES;         // ���Ҵ������ԵĴ���

protected:
	VirtualDesktop *vDesktop;
	HWND hWnd;    // ���ھ����仯

protected:
	void imitateLeftClick(int xPos, int yPos);
	LRESULT setText(HWND hInputWnd, string text);
	string retrieveInstallDir(string keyName, string key);
};
