#include "Task.h"
#include "LoginWnd.h"
#include "ChatMainWnd.h"
#include "AddFriendWnd.h"
#include "Client.h"

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
			loginWnd->ShowTip(L"网络连接失败，请检查网络连接！", TRUE);
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
				loginWnd->ShowTip(L"正在登录.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"正在登录..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"正在登录...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"登录超时", TRUE);
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
				loginWnd->ShowTip(L"正在注册.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"正在注册..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"正在注册...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"注册超时", TRUE);
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
	Client *client = (Client *)lParam;
	Document document;
	try
	{
		if (document.ParseInsitu((char *)data).HasParseError())
		{
			::HeapFree(::GetProcessHeap(), 0, data);
			return 0;
		}
	}
	catch (...)
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
	case SIGNIN_SECCUSS: // 登录成功
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
		client->user = user;
		//SendMessage(client->wnd->GetHWND(), WM_USER_SIGNIN_SUCESS, 0, 0);
		client->loginWnd->SignInBtnSuccess();
		break;
	}
	case SIGNIN_FAILED: // 登录失败
		//SendMessage(client->wnd->GetHWND(), WM_USER_SIGNIN_FAIL, 0, 0);
		SetEvent(client->loginWnd->signInEvent);
		client->loginWnd->SetSignInBtnEnable();
		break;
	case SIGNIN_ALREADY: // 别的地方登录
		client->chatMainWnd->OtherPlaceSignin();
		break;
	case SignUp_SECCUSS: // 注册成功
		//SendMessage(client->wnd->GetHWND(), WM_USER_SIGNUP_SUCESS, 0, 0);
		client->loginWnd->SignUpSuccess();
		break; 
	case SignUp_FAILED: // 注册失败
		//SendMessage(client->wnd->GetHWND(), WM_USER_SIGNUP_FAIL, 0, 0);
		SetEvent(client->loginWnd->signUpEvent);
		client->loginWnd->SetSignUpBtnEnable();
		client->loginWnd->ShowTip(L"注册失败，用户名已存在！", TRUE);
		break;
	case GET_FRIENDS: // 获取好友
	{
		const Value& friends = document["Friends"];
		if (!friends.IsArray())
		{
			break;
		}
		client->friends.clear();
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
			client->friends.push_back((UserAndFriend *)user);
		}
		if (client->chatMainWnd != NULL)
		{
			client->chatMainWnd->UpdateFriendList();
		}
		//SendMessage(client->wnd->GetHWND(), WM_USER_GET_FRIENDS, 0, 0);
		break;
	}
	case SEARCH_FRIENDS: // 查找好友
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
			//SendMessage(client->wnd->GetHWND(), WM_USER_SEARCH_FRIEND, (WPARAM)user, 0);
			client->chatMainWnd->addFriendWnd->SearchResult(user);
		}
		else
		{
			client->chatMainWnd->addFriendWnd->SearchResult(NULL);
			//SendMessage(client->wnd->GetHWND(), WM_USER_SEARCH_FRIEND, 0, 0);
		}
		break;
	case FRIENDS_REQUEST: // 好友请求
	{
		FriendRequest * request = new FriendRequest;
		request->userID = document["userID"].GetInt();
		request->account = AToW(document["account"].GetString());
		request->headerImg = document["headerImg"].GetInt();
		request->nickName = AToW(document["nickName"].GetString());
		request->signature = AToW(document["signature"].GetString());
		request->signature = AToW(document["signature"].GetString());
		request->operation = -1;
		//SendMessage(client->wnd->GetHWND(), WM_USER_FRIEND_REQUEST, (WPARAM)request, 0);
		client->chatMainWnd->OnFriendRequest(request);
	}
		break;
	case AGREE_FRIENDS_REQUEST: // 同意好友请求
		
		break;
	case REFUSE_FRIENDS_REQUEST: // 拒绝好友请求

		break;
	case SINGLE_CHAT_MSG: // 好友消息
	{
		ChatMsgItem * msg = new ChatMsgItem;
		msg->from = document["from"].GetInt();
		msg->msg = AToW(document["msg"].GetString());
		msg->time = AToW(document["time"].GetString());
		msg->bMyself = false;
		//SendMessage(client->wnd->GetHWND(), WM_USER_SINGLE_CHAT_MESSAGE, (WPARAM)msg, 0);
		client->chatMainWnd->RecvSingleMsg(msg);
		break;
	}
	case 1111: // 登录

		break;
	case 11111: // 登录

		break;
	case 111111: // 登录

		break;
	case 111112: // 登录

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
				wnd->ShowTips(L"正在搜索.");
			}
			else if (i % 3 == 2)
			{
				wnd->ShowTips(L"正在搜索..");
			}
			else if (i % 3 == 0)
			{
				wnd->ShowTips(L"正在搜索...");
			}
			if (i > 20)
			{
				wnd->ShowTips(L"搜索超时", TRUE);
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

int Task::ChatListEndDown(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		return 0;
	}
	CListUI *list = (CListUI *)wParam;
	Sleep(50);
	list->EndDown();
	return 0;
}
