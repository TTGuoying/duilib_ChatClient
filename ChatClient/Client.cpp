#include "Client.h"
#include "Common.h"

Client::Client()
{
	wnd = NULL;
}


Client::~Client()
{
}

void Client::OnConnectionClosed()
{
	if (wnd != NULL)
	{
		SendMessage(wnd->GetHWND(), WM_USER_RECONNECT, 0, 0);
	}
}

void Client::OnConnectionError()
{
}

void Client::OnRecvCompleted(BYTE * data, int len)
{
	if (wnd != NULL)
	{
		SendMessage(wnd->GetHWND(), WM_USER_RECVDATA, (WPARAM)data, len);
	}
}

void Client::OnSendCompleted()
{
}

void Client::AssociateWnd(WindowImplBase * wnd)
{
	this->wnd = wnd;
}

