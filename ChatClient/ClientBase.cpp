#include "ClientBase.h"
#include "Common.h"

#pragma comment(lib, "WS2_32.lib")

ClientBase::ClientBase()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	bConnect = FALSE;
}


ClientBase::~ClientBase()
{
	WSACleanup();
}

BOOL ClientBase::Start(LPCTSTR IPAddress, USHORT port)
{
	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == INVALID_SOCKET)
		return false;
	socketEvent = WSACreateEvent();
	stopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);

	inet_pton(AF_INET, ::WcharToUtf8(IPAddress), &serAddr.sin_addr);
	if (connect(clientSock, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{  //连接失败   
		closesocket(clientSock);
		WSACloseEvent(socketEvent);
		CloseHandle(stopEvent);
		return false;
	}
	if (0 != WSAEventSelect(clientSock, socketEvent, FD_READ | FD_CLOSE)) 
	{
		closesocket(clientSock);
		WSACloseEvent(socketEvent);
		CloseHandle(stopEvent);
		return false;
	}
	bConnect = TRUE;
	thread = CreateThread(0, 0, RecvThreadProc, (void *)this, 0, 0);
	return true;
}

BOOL ClientBase::Stop()
{
	SetEvent(stopEvent);
	WaitForSingleObject(thread, INFINITE);
	RELEASE_SOCKET(clientSock);
	WSACloseEvent(socketEvent);
	RELEASE_HANDLE(stopEvent);
	return true;
}

BOOL ClientBase::Send(const BYTE * buffer, int len)
{
	if (SOCKET_ERROR == send(clientSock, (char*)buffer, len, 0))
	{
		return false;
	}
	return true;
}

BOOL ClientBase::IsConnect()
{
	return bConnect;
}

DWORD ClientBase::RecvThreadProc(LPVOID lpParam)
{
	if (lpParam == NULL)
		return 0;

	ClientBase *client = (ClientBase *)lpParam;
	DWORD ret = 0;
	int index = 0;
	WSANETWORKEVENTS networkEvent;
	HANDLE events[2];
	events[0] = client->socketEvent;
	events[1] = client->stopEvent;

	while (true)
	{
		ret = WSAWaitForMultipleEvents(2, events, FALSE, INFINITE, FALSE);
		if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			continue;
		index = ret - WSA_WAIT_EVENT_0;
		if (index == 0)
		{
			WSAEnumNetworkEvents(client->clientSock, events[0], &networkEvent);
			if (networkEvent.lNetworkEvents & FD_READ)
			{
				if (networkEvent.iErrorCode[FD_READ_BIT != 0])
				{
				    //Error
				    continue;
				}
				char *buff = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 4096);
				ret = recv(client->clientSock, buff, 4096, 0);
				if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					client->OnConnectionClosed();
					client->bConnect = FALSE;
					break;		//错误
				}
				client->OnRecvCompleted((BYTE*)buff, ret);
			}
			if (networkEvent.lNetworkEvents & FD_CLOSE)
			{
				client->bConnect = FALSE;
				client->OnConnectionClosed();
				break;	//关闭
			}
		}
		else
		{
			client->bConnect = FALSE;
			client->OnConnectionClosed();
			break;	// 退出
		}
		
	}
	return 1;
}

BOOL ClientBase::IsSocketAlive(SOCKET sock)
{
	return 0;
}
