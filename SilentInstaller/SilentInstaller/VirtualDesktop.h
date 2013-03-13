#pragma once


// VirtualDesktop

class VirtualDesktop : public CWnd
{
	DECLARE_DYNAMIC(VirtualDesktop)

private:
	VirtualDesktop();

public:
	virtual ~VirtualDesktop();

private:
	static VirtualDesktop *vDesktop;
	
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	
	HDESK hDesktop;
	HDESK hOriginalThread;
	HDESK hOriginalInput;
	
	BOOL IsOriginalDesktopActive;	// 是否在原始桌面
	BOOL IsInitialDesktop;			// 是否初始化桌面
	
public:
	static VirtualDesktop *getVirtualDesktop();
	void create();
	void destroy();
	void switchDesktop();

protected:
	DECLARE_MESSAGE_MAP()
};


