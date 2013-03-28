#pragma once

#include "VirtualDesktop.h"

// abstract as base class
class Installer
{
public:
	Installer(void);
	virtual ~Installer(void);

public:
	virtual bool install(VirtualDesktop *vDesktop, string path) = 0;
	
protected:
	const int WAIT_TIME_SHORT;          // Sleep的时长（ms）较短
	const int WAIT_TIME_LONG;           // Sleep的时长（ms）较长
	const int RETRY_SEVERAL_TIMES;      // 查找窗口重试的次数
	const int RETRY_MANY_TIMES;         // 查找窗口重试的次数

protected:
	VirtualDesktop *vDesktop;
	HWND hWnd;    // 窗口句柄会变化

protected:
	void imitateLeftClick(int xPos, int yPos);
	LRESULT setText(HWND hInputWnd, string text);

public:
	static void killProcess(string name);
};
