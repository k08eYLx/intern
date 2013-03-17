#pragma once


// WindowFinder

class WindowFinder : public CWnd
{
	DECLARE_DYNAMIC(WindowFinder)

public:
	WindowFinder();
	virtual ~WindowFinder();

public:
	void listWindows(CEdit *pEdit);
	void listChildWindows(HWND hWndParent, CEdit *pEdit);
	HWND find(CString name);
	HWND findChild(CString name);
	static bool appendText(CEdit *pEdit, HWND hwnd);

protected:
	DECLARE_MESSAGE_MAP()
};


