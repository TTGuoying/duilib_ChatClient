#include "Client.h"
#include "Common.h"
#include "LoginWnd.h"
#include "ChatMainWnd.h"
#include "Task.h"

Client::Client()
{
	friends.clear();
	friendRequests.clear();
	sessions.clear();
	user = NULL;
	loginWnd = NULL;
	chatMainWnd = NULL;
	threadPool = new ThreadPool;
}


Client::~Client()
{
	RELEASE(user);
}

void Client::OnConnectionClosed()
{
	if (loginWnd != NULL)
	{
		threadPool->QueueTaskItem(Task::Connect, (WPARAM)loginWnd, NULL);
	}
	if (chatMainWnd != NULL && !chatMainWnd->bClose)
	{
		chatMainWnd->ConnectClose();
	}
}

void Client::OnConnectionError()
{
}

void Client::OnRecvCompleted(BYTE * data, int len)
{
	threadPool->QueueTaskItem(Task::ProcessRecvDate, (WPARAM)data, (LPARAM)this);
}

void Client::OnSendCompleted()
{
}

