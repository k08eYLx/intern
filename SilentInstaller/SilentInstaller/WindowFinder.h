#pragma once


// WindowFinder

struct WndInfo {
	void*   pVoid;
	HANDLE  hEvent;
	HDESK   hDesktop;
	string  title;
	POINT   pt;
	HWND    hWnd;
};

class WindowFinder : public CWnd
{
	DECLARE_DYNAMIC(WindowFinder)

public:
	WindowFinder();
	virtual ~WindowFinder();

public:
	void listWindows(CEdit *pEdit);
	void listWindows(HDESK hDesktop, CEdit *pEdit);
	void listChildWindows(HWND hWndParent, CEdit *pEdit);
	HWND find(CString name);
	HWND findChild(CString name);
	static bool appendText(CEdit *pEdit, HWND hwnd);

	friend BOOL CALLBACK EnumWindowsProc(HWND hwnd, DWORD lParam);
	friend BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


