#pragma once
#include "stdafx.h"

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