#include "Task.h"
#include "LoginWnd.h"
#include "AddFriendWnd.h"

int Task::Connect(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	
	while (WaitForSingleObject(loginWnd->stopEvent, 0))
	{
		loginWnd->Connect();
		if (!loginWnd->client->IsConnect())
		{
			loginWnd->ShowTip(L"Õ¯¬Á¡¨Ω” ß∞‹£¨«ÎºÏ≤ÈÕ¯¬Á¡¨Ω”£°", TRUE);
			Sleep(1000);
			continue;
		}
		else
		{
			loginWnd->ShowTip(L"");
			break;
		}
	}
	return 1;
}

int Task::SignInWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signInEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i%3 == 1)
			{
				loginWnd->ShowTip(L"’˝‘⁄µ«¬º.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"’˝‘⁄µ«¬º..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"’˝‘⁄µ«¬º...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"µ«¬º≥¨ ±", TRUE);
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

int Task::SignUpWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signUpEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i % 3 == 1)
			{
				loginWnd->ShowTip(L"’˝‘⁄◊¢≤·.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"’˝‘⁄◊¢≤·..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"’˝‘⁄◊¢≤·...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"◊¢≤·≥¨ ±", TRUE);
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

int Task::ProcessRecvDate(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL || lParam == NULL)
		return 0;

	BYTE *data = (BYTE *)wParam;
	WindowImplBase *wnd = (WindowImplBase *)lParam;
	Document document;
	if (document.ParseInsitu((char *)data).HasParseError())
	{
		::HeapFree(::GetProcessHeap(), 0, data);
		return 0;
	}
	if (!(document.IsObject() && document.HasMember("protocol")))
	{
		::HeapFree(::GetProcessHeap(), 0, data);
		return 0;
	}

	int protocol = document["protocol"].GetInt();
	switch (protocol)
	{
	case SIGNIN_SECCUSS: // µ«¬º≥…π¶
	{
		UserAndFriend *user = new UserAndFriend;
		user->userID = document["userID"].GetInt();
		user->account = AToW(document["account"].GetString());
		user->headerImg = document["headerImg"].GetInt();
		user->nickName = AToW(document["nickName"].GetString());
		user->area = AToW(document["area"].GetString());
		user->phone = AToW(document["phone"].GetString());
		user->sexulity = document["sexulity"].GetInt();
		user->signature = AToW(document["signature"].GetString());
		SendMessage(wnd->GetHWND(), WM_USER_SIGNIN_SUCESS, (WPARAM)user, 0);
		break;
	}
	case SIGNIN_FAILED: // µ«¬º ß∞‹
		SendMessage(wnd->GetHWND(), WM_USER_SIGNIN_FAIL, 0, 0);
		break;
	case SIGNIN_ALREADY: // ±µƒµÿ∑Ωµ«¬º
		break;
	case SignUp_SECCUSS: // ◊¢≤·≥…π¶
		SendMessage(wnd->GetHWND(), WM_USER_SIGNUP_SUCESS, 0, 0);
		break; 
	case SignUp_FAILED: // ◊¢≤· ß∞‹
		SendMessage(wnd->GetHWND(), WM_USER_SIGNUP_FAIL, 0, 0);
			break;
	case GET_FRIENDS: // ªÒ»°∫√”—
	{
		const Value& friends = document["Friends"];
		if (!friends.IsArray())
		{
			break;
		}
		for (SizeType i = 0; i < friends.Size(); i++)
		{
			UserAndFriend * user = new UserAndFriend;
			user->userID = friends[i]["userID"].GetInt();
			user->account = AToW(friends[i]["account"].GetString());
			user->headerImg = friends[i]["headerImg"].GetInt();
			user->nickName = AToW(friends[i]["nickName"].GetString());
			user->area = AToW(friends[i]["area"].GetString());
			user->phone = AToW(friends[i]["phone"].GetString());
			user->sexulity = friends[i]["sexulity"].GetInt();
			user->signature = AToW(friends[i]["signature"].GetString());
			SendMessage(wnd->GetHWND(), WM_USER_GET_FRIENDS, (WPARAM)user, 0);
		}
		SendMessage(wnd->GetHWND(), WM_USER_GET_FRIENDS, 0, 1);
		break;
	}
	case SEARCH_FRIENDS: // µ«¬º
		if (document["result"].GetBool())
		{
			UserAndFriend * user = new UserAndFriend;
			user->userID = document["userID"].GetInt();
			user->account = AToW(document["account"].GetString());
			user->headerImg = document["headerImg"].GetInt();
			user->nickName = AToW(document["nickName"].GetString());
			user->area = AToW(document["area"].GetString());
			user->phone = AToW(document["phone"].GetString());
			user->sexulity = document["sexulity"].GetInt();
			user->signature = AToW(document["signature"].GetString());
			SendMessage(wnd->GetHWND(), WM_USER_SEARCH_FRIEND, (WPARAM)user, 0);
		}
		else
		{
			SendMessage(wnd->GetHWND(), WM_USER_SEARCH_FRIEND, 0, 0);
		}
		break;
	case 111: // µ«¬º

		break;
	case 1111: // µ«¬º

		break;
	case 11111: // µ«¬º

		break;
	case 111111: // µ«¬º

		break;
	case 111112: // µ«¬º

		break;
	default:
		break;
	}

	::HeapFree(::GetProcessHeap(), 0, data);
	return 1;
}

int Task::SearchWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	AddFriendWnd *wnd = (AddFriendWnd*)wParam;

	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(wnd->searchEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i % 3 == 1)
			{
				wnd->ShowTips(L"’˝‘⁄À—À˜.");
			}
			else if (i % 3 == 2)
			{
				wnd->ShowTips(L"’˝‘⁄À—À˜..");
			}
			else if (i % 3 == 0)
			{
				wnd->ShowTips(L"’˝‘⁄À—À˜...");
			}
			if (i > 20)
			{
				wnd->ShowTips(L"À—À˜≥¨ ±", TRUE);
				wnd->SetSearchBtnEnable();
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
