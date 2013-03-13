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
	IsOriginalDesktopActive = TRUE;
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
	if (IsInitialDesktop)	// ��һ������ʱ��ʼ������
	{
		// ��������Ϊ��Virtual�����������棨��CloseDesktop���Ӧ��
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	

		// ��ʼ���������̱���Ľṹ��
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
		StartupInfo.cb        = sizeof(StartupInfo);
		StartupInfo.lpDesktop = "Virtual";

		// ��¼ԭʼ����ľ��
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput  = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP); 

		// �޷�ִ�а�װ���򣬵���������������ִ�г���WHY???
		// "D:\\eclipse-yanglx\\eclipse.exe"
		CString commandLine = "explorer";
		// ����������ǽ��̣��ڹ�˾�����ϻ����dwProcessId��һ�µ���������Լ�������û���⡣
		if (!::CreateProcess(NULL, (CT2A)commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
		{
			TRACE("===> Create process failed!\n");
			exit(EXIT_FAILURE);
		}

		// ��ɳ�ʼ�����Ժ󲻻��ٴα����ã���ֹ���������������Դ������
		IsInitialDesktop = !IsInitialDesktop;
	}
}

void VirtualDesktop::destroy()
{
	// �ر��ɱ����򴴽���explorer
	if (vDesktop != NULL)
	{
		// ��ȡ���������explorer���̾��
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.dwProcessId);
		// HANDLE��ID��ô���仯�ˣ�����
		TRACE("ProcessId ===> %d %d %d\n", ProcessInfo.dwProcessId, hProcess, ProcessInfo.hProcess);
		if (hProcess)   
		{
			TerminateProcess(hProcess, EXIT_SUCCESS);	// �������������explorer
			CloseDesktop(hDesktop);
		}
		CloseHandle(hProcess);
		delete vDesktop;
	}
}

/*
 * For debug only.
 * switchֻ�踺���л����棬û�б�Ҫ�ٴ�����ر�Virtual Desktop
 */
void VirtualDesktop::switchDesktop()
{
	if (IsOriginalDesktopActive)	   // �Ƿ���ԭʼ����
	{
		//hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// ������������
		SetThreadDesktop(hDesktop);    // ���������������������� 
		SwitchDesktop(hDesktop);       // �л�����������
	} 
	else
	{
		SetThreadDesktop(hOriginalThread); // ��������������ԭʼ���� 
		SwitchDesktop(hOriginalInput);     // �л���ԭʼ����
		//CloseDesktop(hDesktop);	       // �ر���������
	}
	IsOriginalDesktopActive = !IsOriginalDesktopActive;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

