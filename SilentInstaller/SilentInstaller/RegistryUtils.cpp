// RegistryUtils.cpp : implementation file
//

#include "stdafx.h"
#include "SilentInstaller.h"
#include "RegistryUtils.h"


// RegistryUtils

IMPLEMENT_DYNAMIC(RegistryUtils, CWnd)

RegistryUtils::RegistryUtils()
{

}

RegistryUtils::~RegistryUtils()
{
}


// 写入文本  
void RegistryUtils::writeString(CRegKey &regKey, CString key, CString value)  
{  
    regKey.SetStringValue(key, value);  
}


// 写入bool  
void RegistryUtils::writeBoolean(CRegKey &regKey, CString key, bool value)  
{  
    writeString(regKey, key, value ? _T("true") : _T("false"));  
}


// 写入DWORD  
void RegistryUtils::writeDword(CRegKey &regKey, CString key, DWORD value)  
{  
    regKey.SetDWORDValue(key, value);  
}


// 读取文本  
bool RegistryUtils::readString(CRegKey &regKey, CString key, CString &value)  
{  
	TCHAR buf[MAX_PATH] = { 0 };
    DWORD len = MAX_PATH;  
    if (regKey.QueryStringValue(key, buf, &len) == ERROR_SUCCESS) {  
        value = buf;
        return true;
    }
    return false;  
}


// 读取bool  
bool RegistryUtils::readBoolean(CRegKey &regKey, CString key, bool &value)  
{  
    CString strValue = "";  
    // 先读取文本值
	if (readString(regKey, key, strValue)) {
        // 判断是否为true  
        value = (strValue.CompareNoCase("true") == 0);  
        return true;
    }
    return false;  
}


// 读取DWORD  
bool RegistryUtils::readDword(CRegKey &regKey, CString key, DWORD &value)  
{
    DWORD dwValue = 0;  
    if (regKey.QueryDWORDValue(key, dwValue) == ERROR_SUCCESS) {  
        value = dwValue;
        return true;  
    }
    return false;
}


void RegistryUtils::openLm(CRegKey &regKey, string keyName)
{
	regKey.Open(HKEY_LOCAL_MACHINE, _T(keyName.c_str()));
}


void RegistryUtils::openCu(CRegKey &regKey, string keyName)
{
	regKey.Open(HKEY_CURRENT_USER, _T(keyName.c_str()));
}


bool RegistryUtils::readCuString(string keyName, CString key, CString &value)
{
	CRegKey regKey;
	openCu(regKey, keyName);
	return readString(regKey, key, value);
}


bool RegistryUtils::readCuBoolean(string keyName, CString key, bool &value)
{
	CRegKey regKey;
	openCu(regKey, keyName);
	return readBoolean(regKey, key, value);
}


bool RegistryUtils::readCuDword(string keyName, CString key, DWORD &value)
{
	CRegKey regKey;
	openCu(regKey, keyName);
	return readDword(regKey, key, value);
}


BEGIN_MESSAGE_MAP(RegistryUtils, CWnd)
END_MESSAGE_MAP()



// RegistryUtils message handlers


