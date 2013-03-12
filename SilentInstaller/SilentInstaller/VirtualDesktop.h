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
	
	BOOL IsOriginalDesktopActive;	// �Ƿ���ԭʼ����
	BOOL IsInitialDesktop;			// �Ƿ��ʼ�����棬��ֹ��ɫ��ť������ɲ��ɹ��Ƶĺ��
	
public:
	static VirtualDesktop *getVirtualDesktop();
	void create();
	void destroy();
	void switchDesktop();

protected:
	DECLARE_MESSAGE_MAP()
};


