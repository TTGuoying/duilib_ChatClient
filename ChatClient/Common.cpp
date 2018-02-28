#include "stdafx.h"
#include "Common.h"


const char* WcharToUtf8(const wchar_t *pwStr)
{
	if (pwStr == NULL)
	{
		return NULL;
	}

	int len = WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
	{
		return NULL;
	}
	char *pStr = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, pStr, len, NULL, NULL);
	return pStr;
}

const wchar_t* Utf8ToWchar(const char *pStr)
{
	if (pStr == NULL)
	{
		return NULL;
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	if (len <= 0)
	{
		return NULL;
	}
	wchar_t *pwStr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pwStr, len);
	return pwStr;
}

CDuiString GetInstancePath()
{
	return CPaintManagerUI::GetInstancePath();
}

CDuiString GetCurrentTimeString()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CDuiString str;
	str.Format(L"%4d/%02d/%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return str;
}
