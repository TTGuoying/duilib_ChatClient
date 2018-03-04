#pragma once
#include "stdafx.h"
#include "ClientBase.h"
#include "Common.h"
#include <list>
#include "ThreadPool.h"

using std::list;

class LoginWnd;
class ChatMainWnd;
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

	LoginWnd *loginWnd;
	ChatMainWnd *chatMainWnd;
	UserAndFriend *user;

	ThreadPool *threadPool;

	list<SessionItem *> sessions;
	list<UserAndFriend *> friends;
	list<FriendRequest *> friendRequests;
};
