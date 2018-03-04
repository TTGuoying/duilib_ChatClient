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

char *WToA(LPCTSTR str)
{
	LPSTR pszOut = NULL;
	if (str != NULL)
	{
		int len = wcslen(str);

		len = WideCharToMultiByte(CP_ACP, 0, str, len, NULL, 0, 0, 0) + 2;
		pszOut = new char[len];

		if (pszOut)
		{
			memset(pszOut, 0x00, len);
			WideCharToMultiByte(CP_ACP, 0, str, len, pszOut, len, 0, 0);
		}
	}
	return pszOut;
}

LPWSTR AToW(const char* str)
{
	LPWSTR pszOut = NULL;
	if (str != NULL)
	{
		int nOutputStrLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0) + 1;
		pszOut = (LPWSTR)new char[nOutputStrLen];

		if (pszOut)
		{
			ZeroMemory((void *)pszOut, nOutputStrLen);
			MultiByteToWideChar(CP_ACP, 0, str, -1, pszOut, nOutputStrLen);;
		}
	}
	return pszOut;
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
	str.Format(L"%4d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return str;
}
