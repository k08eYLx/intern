#pragma once


// WindowFinder

struct WndInfo {
	HDESK   hDesktop;
	string  title;
	POINT   pt;
	HWND    hWnd;
	HWND    hParent;
	int     length;
};

struct ProcedureData {
	string clsName;
	vector<WndInfo> wInfos;
};


class WindowFinder : public CWnd
{
	DECLARE_DYNAMIC(WindowFinder)

public:
	WindowFinder();
	virtual ~WindowFinder();

public:
	friend BOOL CALLBACK EnumWindowsProc(HWND hwnd, DWORD lParam);
	void listWindows(CEdit *pEdit);
	void listWindows(HDESK hDesktop, CEdit *pEdit);
	
	friend BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
	void listChildWindows(HWND hWndParent, CEdit *pEdit);
	
	friend BOOL CALLBACK EnumWndInfoProc(HWND hWnd, LPARAM lParam);
	void findWindows(HDESK hDesktop, ProcedureData *pData);

	friend BOOL CALLBACK EnumChildInfoProc(HWND hWndChild, LPARAM lParam);
	void findChildWindows(HWND hWndParent, ProcedureData *pData);

	HWND find(CString name);
	static bool appendText(CEdit *pEdit, HWND hwnd);

protected:
	DECLARE_MESSAGE_MAP()
};


