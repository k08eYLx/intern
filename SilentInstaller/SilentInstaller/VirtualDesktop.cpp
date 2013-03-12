// VirtualDesktop.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "VirtualDesktop.h"


// VirtualDesktop

VirtualDesktop * VirtualDesktop::vDesktop = NULL;

IMPLEMENT_DYNAMIC(VirtualDesktop, CWnd)

VirtualDesktop::VirtualDesktop()
{
	IsOriginalDesktopActive = FALSE;
	IsInitialDesktop = TRUE;
}

VirtualDesktop::~VirtualDesktop()
{
}

VirtualDesktop *VirtualDesktop::getVirtualDesktop()
{
	if (vDesktop == NULL)
	{
		vDesktop = new VirtualDesktop();
	}
	return vDesktop;
}

void VirtualDesktop::create()
{
	if (IsInitialDesktop)	// ��һ������ʱ��ʼ�����򣬼����жϷ�ֹ��ɫ��ť����
	{
		// ��������Ϊ��Virtual������������
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	

		// ��ʼ���������̱���Ľṹ��
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
		StartupInfo.cb = sizeof(StartupInfo);
		StartupInfo.lpDesktop = "Virtual";

		// ��¼ԭʼ����ľ��
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP); 

		// ����������ǽ���
		CreateProcess(NULL, "explorer", NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);

		// ��ɳ�ʼ�����Ժ󲻻��ٴα����ã���ֹ���������������Դ������
		IsInitialDesktop = !IsInitialDesktop;

		//SetThreadDesktop(hDesktop);		// ����������������������
		//SwitchDesktop(hDesktop);		    // �л�����������
	}
}

void VirtualDesktop::destroy()
{
	// �ر��ɱ����򴴽���explorer
	if (vDesktop != NULL)
	{
		// ��ȡ���������explorer���
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.dwProcessId);
		if (hProcess)   
		{
			TerminateProcess(hProcess, 1);	// �������������explorer
		}
		CloseHandle(hProcess);
		delete vDesktop;
	}
	exit(0);
}

void VirtualDesktop::switchDesktop()
{
	if (!IsOriginalDesktopActive)	// �Ƿ���ԭʼ����
	{
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// �����������棨��CloseDesktop���Ӧ��
		SetThreadDesktop(hDesktop);
		SwitchDesktop(hDesktop);
	} 
	else
	{
		SetThreadDesktop(hOriginalThread);
		SwitchDesktop(hOriginalInput);
		CloseDesktop(hDesktop);		// �ر���������
	}
	IsOriginalDesktopActive = !IsOriginalDesktopActive;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

