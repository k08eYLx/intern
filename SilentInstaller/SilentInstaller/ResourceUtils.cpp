// ResourceUtils.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "ResourceUtils.h"

#include "FileUtils.h"


// ResourceUtils

ResourceUtils::ResourceUtils()
{
}

ResourceUtils::~ResourceUtils()
{
}


// ResourceUtils member functions


/**
 * path ����Դ�ļ��ͷŵ���·��
 * rId  �Ƕ�Ӧ��Դ��ID
 * type �Ƕ�Ӧ��Դ�����ͣ��ڲ�����Դʱ�Զ������Դ����
**/
bool ResourceUtils::extract(const TCHAR *path, const int rId, const TCHAR *type/*  = _T("EXE")*/)
{
	HRSRC hRes = ::FindResource(NULL, MAKEINTRESOURCE(rId), type);
    if (NULL == hRes) return false;
          
    DWORD dwSize = ::SizeofResource(NULL, hRes);
    if (0 == dwSize) return false;
          
    HGLOBAL hGlobal = ::LoadResource(NULL, hRes);
    if (NULL == hGlobal) return false;
          
    LPVOID lpResource = ::LockResource(hGlobal);
    if (NULL == lpResource) return false;

    CFile file;
	UINT openFlags = CFile::modeWrite;
	if (!PathFileExists(path)) openFlags |= CFile::modeCreate;
	BOOL bRtVal = file.Open(path, openFlags);
	file.Write(lpResource, dwSize);
    file.Close();
  
    return (bRtVal == TRUE);
}


/**
 * ����Դ�ļ��ͷŵ�����ʱĿ¼
 * rId  �Ƕ�Ӧ��Դ��ID
 * type �Ƕ�Ӧ��Դ�����ͣ��ڲ�����Դʱ�Զ������Դ����
**/
string ResourceUtils::extract2Tmp(const int rId, const TCHAR *type/*  = _T("EXE")*/)
{
	TCHAR szTmp[MAX_PATH] = { _T("") };
	FileUtils::getTempFileName(szTmp);

	if (extract(szTmp, rId))
		return string(szTmp, szTmp + strlen(szTmp));
	else return "";
}
