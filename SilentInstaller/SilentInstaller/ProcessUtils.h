#pragma once


// ProcessUtils

class ProcessUtils : public CWnd
{
	DECLARE_DYNAMIC(ProcessUtils)

public:
	ProcessUtils();
	virtual ~ProcessUtils();

public:
	static bool kill(string name);
	static bool adjustPrivilege();

protected:
	DECLARE_MESSAGE_MAP()
};


