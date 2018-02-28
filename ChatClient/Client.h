#pragma once
#include "stdafx.h"
#include "ClientBase.h"

class Client : public ClientBase
{
public:
	Client();
	~Client();

	// 连接关闭
	virtual void OnConnectionClosed();
	// 连接上发生错误
	virtual void OnConnectionError();
	// 读操作完成
	virtual void OnRecvCompleted(BYTE* data, int len);
	// 写操作完成
	virtual void OnSendCompleted();

	void AssociateWnd(WindowImplBase *wnd);
	WindowImplBase *wnd;
};
