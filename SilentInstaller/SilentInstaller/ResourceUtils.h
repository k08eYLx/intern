#pragma once

// ResourceUtils command target

class ResourceUtils : public CObject
{
public:
	ResourceUtils();
	virtual ~ResourceUtils();

public:
	static bool extract(const TCHAR *path, const int rId, const TCHAR *type = _T("EXE"));
	static string extract2Tmp(const int rId, const TCHAR *type = _T("EXE"));
};


