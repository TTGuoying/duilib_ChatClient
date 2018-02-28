#pragma once
#include "stdafx.h"

// 释放指针的宏
#define RELEASE(x)			{if(x != NULL) {delete x; x = NULL;}}
// 释放句柄的宏
#define RELEASE_HANDLE(x)	{if(x != NULL && x != INVALID_HANDLE_VALUE) { CloseHandle(x); x = INVALID_HANDLE_VALUE; }}
// 释放Socket的宏
#define RELEASE_SOCKET(x)	{if(x != INVALID_SOCKET) { closesocket(x); x = INVALID_SOCKET; }}

class ClientBase
{
public:
	ClientBase();
	~ClientBase();

	// 启动通信
	BOOL Start(LPCTSTR IPAddress, USHORT port);	
	// 关闭通信
	BOOL Stop();	
	// 发送数据
	BOOL Send(const BYTE* buffer, int len);	
	// 是否已启动
	BOOL IsConnect();	

	// 事件通知函数(派生类重载此族函数)
	// 连接关闭
	virtual void OnConnectionClosed() = 0;
	// 连接上发生错误
	virtual void OnConnectionError() = 0;
	// 读操作完成
	virtual void OnRecvCompleted(BYTE* buffer, int len) = 0;
	// 写操作完成
	virtual void OnSendCompleted() = 0;

private:
	// 接收线程函数
	static DWORD WINAPI RecvThreadProc(LPVOID lpParam); 
	// socket是否存活
	BOOL IsSocketAlive(SOCKET sock);
	SOCKET clientSock;
	WSAEVENT socketEvent;
	HANDLE stopEvent;
	HANDLE thread;
	BOOL bConnect;
};

