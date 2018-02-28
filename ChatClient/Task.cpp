#include "Task.h"
#include "LoginWnd.h"

int Task::Connect(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
	
	while (!loginWnd->client->IsConnect())
	{
		loginWnd->Connect();
		if (!loginWnd->client->IsConnect())
		{
			loginWnd->ShowTip(L"ÍøÂçÁ¬½ÓÊ§°Ü£¬Çë¼ì²éÍøÂçÁ¬½Ó£¡", TRUE);
			Sleep(1000);
			continue;
		}
		else
		{
			loginWnd->ShowTip(L"");
		}
	}
	return 1;
}

int Task::SignInWait(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signInEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i%3 == 1)
			{
				loginWnd->ShowTip(L"ÕıÔÚµÇÂ¼.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"ÕıÔÚµÇÂ¼..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"ÕıÔÚµÇÂ¼...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"µÇÂ¼³¬Ê±", TRUE);
				loginWnd->SetSignInBtnEnable();
				break;
			}
		}
		else
		{
			break;
		}

	}
	return 1;
}

int Task::SignUpWait(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signUpEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i % 3 == 1)
			{
				loginWnd->ShowTip(L"ÕıÔÚ×¢²á.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"ÕıÔÚ×¢²á..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"ÕıÔÚ×¢²á...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"×¢²á³¬Ê±", TRUE);
				loginWnd->SetSignUpBtnEnable();
				break;
			}
		}
		else
		{
			break;
		}

	}
	return 1;
}

int Task::ProcessRecvDate(PVOID p)
{
	if (p == NULL)
		return 0;

	RecvDateParam *param = (RecvDateParam*)p;
	Document document;
	if (document.ParseInsitu((char *)param->data).HasParseError())
	{
		::HeapFree(::GetProcessHeap(), 0, param->data);
		delete param;
		return 0;
	}
	if (!(document.IsObject() && document.HasMember("protocol")))
	{
		::HeapFree(::GetProcessHeap(), 0, param->data);
		delete param;
		return 0;
	}

	int protocol = document["protocol"].GetInt();
	switch (protocol)
	{
	case SIGNIN_SECCUSS: // µÇÂ¼³É¹¦
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNIN_SUCESS, 0, 0);
		break;
	case SIGNIN_FAILED: // µÇÂ¼Ê§°Ü
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNIN_FAIL, 0, 0);
		break;
	case SIGNIN_ALREADY: // ±ğµÄµØ·½µÇÂ¼
		break;
	case SignUp_SECCUSS: // ×¢²á³É¹¦
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNUP_SUCESS, 0, 0);
		break; 
	case SignUp_FAILED: // ×¢²áÊ§°Ü
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNUP_FAIL, 0, 0);
			break;
	case 100: // µÇÂ¼

		break;
	case 101: // µÇÂ¼

		break;
	default:
		break;
	}

	::HeapFree(::GetProcessHeap(), 0, param->data);
	delete param;
	return 1;
}
