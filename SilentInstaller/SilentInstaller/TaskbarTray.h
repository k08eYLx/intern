#pragma once


// TaskbarTray

class TaskbarTray : public CWnd
{
	DECLARE_DYNAMIC(TaskbarTray)

public:
	TaskbarTray();
	virtual ~TaskbarTray();

public:
	HWND findTrayWnd();
	HWND findNotifyIconOverflowWindow();
	RECT getTrayRect();
	bool enumNotifyWindow(RECT &rect,HWND hWnd);

protected:
	DECLARE_MESSAGE_MAP()
};


