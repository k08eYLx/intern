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
	if (IsInitialDesktop)	// 第一次运行时初始化程序，加上判断防止灰色按钮克星
	{
		// 创建名称为“Virtual”的虚拟桌面
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	

		// 初始化创建进程必须的结构体
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
		StartupInfo.cb = sizeof(StartupInfo);
		StartupInfo.lpDesktop = "Virtual";

		// 记录原始桌面的句柄
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP); 

		// 创建桌面外壳进程
		CreateProcess(NULL, "explorer", NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);

		// 完成初始化，以后不会再次被调用，防止打开虚拟桌面出现资源管理器
		IsInitialDesktop = !IsInitialDesktop;

		//SetThreadDesktop(hDesktop);		// 设置桌面活动焦点是虚拟桌面
		//SwitchDesktop(hDesktop);		    // 切换到虚拟桌面
	}
}

void VirtualDesktop::destroy()
{
	// 关闭由本程序创建的explorer
	if (vDesktop != NULL)
	{
		// 获取虚拟桌面的explorer句柄
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.dwProcessId);
		if (hProcess)   
		{
			TerminateProcess(hProcess, 1);	// 结束虚拟桌面的explorer
		}
		CloseHandle(hProcess);
		delete vDesktop;
	}
	exit(0);
}

void VirtualDesktop::switchDesktop()
{
	if (!IsOriginalDesktopActive)	// 是否在原始桌面
	{
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// 创建虚拟桌面（与CloseDesktop相对应）
		SetThreadDesktop(hDesktop);
		SwitchDesktop(hDesktop);
	} 
	else
	{
		SetThreadDesktop(hOriginalThread);
		SwitchDesktop(hOriginalInput);
		CloseDesktop(hDesktop);		// 关闭虚拟桌面
	}
	IsOriginalDesktopActive = !IsOriginalDesktopActive;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

