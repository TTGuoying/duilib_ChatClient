#pragma once
#include "stdafx.h"

const char* WcharToUtf8(const wchar_t *pwStr);
const wchar_t* Utf8ToWchar(const char *pStr);
CDuiString GetInstancePath();
CDuiString GetCurrentTimeString();
char *WToA(LPCTSTR str);
LPWSTR AToW(const char* str);

struct SessionItemInfo
{
	CDuiString headerImg;
	CDuiString nickName;
	CDuiString msgTime;
	CDuiString message;
	bool newMsg;
	bool mute;
};

struct ChatMsgItem
{
	CDuiString headerImg;
	CDuiString nickName;
	CDuiString msgTime;
	CDuiString message;
};

// ”√ªß/∫√”—
struct UserAndFriend
{
	ULONG		userID;
	int			headerImg;
	LPCTSTR		nickName;
	int			sexulity;
	LPCTSTR		account;
	LPCTSTR		signature;
	LPCTSTR		area;
	LPCTSTR		phone;
};