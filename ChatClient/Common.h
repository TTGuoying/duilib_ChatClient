#pragma once
#include "stdafx.h"

const char* WcharToUtf8(const wchar_t *pwStr);
const wchar_t* Utf8ToWchar(const char *pStr);
CDuiString GetInstancePath();
CDuiString GetCurrentTimeString();

struct RecvDateParam
{
	BYTE *data;
	WindowImplBase *wnd;
};
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