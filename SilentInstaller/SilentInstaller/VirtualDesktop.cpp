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
	if (IsInitialDesktop)	// 第一次运行时初始化程序
	{
		// 创建名称为“Virtual”的虚拟桌面（与CloseDesktop相对应）
		hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	

		// 初始化创建进程必须的结构体
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
		StartupInfo.cb        = sizeof(StartupInfo);
		StartupInfo.lpDesktop = "Virtual";

		// 记录原始桌面的句柄
		hOriginalThread = GetThreadDesktop(GetCurrentThreadId());   
		hOriginalInput  = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP); 

		// 无法执行安装程序，但可以运行其他可执行程序。WHY???
		// "D:\\eclipse-yanglx\\eclipse.exe"
		CString commandLine = "explorer";
		// 创建桌面外壳进程，在公司电脑上会出现dwProcessId不一致的情况，在自己电脑上没问题。
		if (!::CreateProcess(NULL, (CT2A)commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
		{
			TRACE("===> Create process failed!\n");
			exit(EXIT_FAILURE);
		}

		// 完成初始化，以后不会再次被调用，防止打开虚拟桌面出现资源管理器
		IsInitialDesktop = !IsInitialDesktop;
	}
}

void VirtualDesktop::destroy()
{
	// 关闭由本程序创建的explorer
	if (vDesktop != NULL)
	{
		// 获取虚拟桌面的explorer进程句柄
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.dwProcessId);
		// HANDLE和ID怎么都变化了？？？
		TRACE("ProcessId ===> %d %d %d\n", ProcessInfo.dwProcessId, hProcess, ProcessInfo.hProcess);
		if (hProcess)   
		{
			TerminateProcess(hProcess, EXIT_SUCCESS);	// 结束虚拟桌面的explorer
			CloseDesktop(hDesktop);
		}
		CloseHandle(hProcess);
		delete vDesktop;
	}
}

/*
 * For debug only.
 * switch只需负责切换桌面，没有必要再创建或关闭Virtual Desktop
 */
void VirtualDesktop::switchDesktop()
{
	if (IsOriginalDesktopActive)	   // 是否在原始桌面
	{
		//hDesktop = CreateDesktop("Virtual", NULL, NULL, 0, GENERIC_ALL , NULL);	// 创建虚拟桌面
		SetThreadDesktop(hDesktop);    // 设置桌面活动焦点是虚拟桌面 
		SwitchDesktop(hDesktop);       // 切换到虚拟桌面
	} 
	else
	{
		SetThreadDesktop(hOriginalThread); // 设置桌面活动焦点是原始桌面 
		SwitchDesktop(hOriginalInput);     // 切换回原始桌面
		//CloseDesktop(hDesktop);	       // 关闭虚拟桌面
	}
	IsOriginalDesktopActive = !IsOriginalDesktopActive;
}


BEGIN_MESSAGE_MAP(VirtualDesktop, CWnd)

END_MESSAGE_MAP()



// VirtualDesktop message handlers

