#pragma once


// RegistryUtils

class RegistryUtils : public CWnd
{
	DECLARE_DYNAMIC(RegistryUtils)

public:
	RegistryUtils();
	virtual ~RegistryUtils();

public:
	// Lm for Local Machine
	void openLm(CRegKey &regKey, string keyName);
	// Cu for Current User
	void openCu(CRegKey &regKey, string keyName);

	void writeString(CRegKey &regKey, CString key, CString value);
	void writeBoolean(CRegKey &regKey, CString key, bool value);
	void writeDword(CRegKey &regKey, CString key, DWORD value);
	
	bool readString(CRegKey &regKey, CString key, CString &value);
	bool readBoolean(CRegKey &regKey, CString key, bool &value);
	bool readDword(CRegKey &regKey, CString key, DWORD &value);

	bool readCuString(string keyName, CString key, CString &value);
	bool readCuBoolean(string keyName, CString key, bool &value);
	bool readCuDword(string keyName, CString key, DWORD &value);

protected:
	DECLARE_MESSAGE_MAP()
};


