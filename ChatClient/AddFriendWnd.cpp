#include "AddFriendWnd.h"
#include "Task.h"

AddFriendWnd::AddFriendWnd(WindowImplBase *wnd, Client *client, ThreadPool *threadPool)
{
	this->wnd = wnd;
	this->client = client;
	this->threadPool = threadPool;
	searchEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	friendID = 0;
}


AddFriendWnd::~AddFriendWnd()
{
	CloseHandle(searchEvent);
}

UILIB_RESOURCETYPE AddFriendWnd::GetResourceType() const
{
	//#ifdef _DEBUG
	//	return UILIB_FILE;
	//#else
	//	return UILIB_ZIP;
	//#endif // _DEBUG
	return UILIB_FILE;
}

CDuiString AddFriendWnd::GetSkinFolder()
{
	return L"Skin\\";
}

CDuiString AddFriendWnd::GetSkinFile()
{
	return L"AddFriendWnd.xml";
}

CDuiString AddFriendWnd::GetZIPFileName() const
{
	return L"Skin.zip";
}

LPCTSTR AddFriendWnd::GetWindowClassName() const
{
	return L"AddFriendWnd";
}

void AddFriendWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"btnclose"));
	btnSearch = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"BtnSearch"));
	btnAdd = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Add"));
	edSearch = static_cast<CEditUI *>(m_PaintManager.FindControl(L"EdSearch"));
	edSearch->SetFocus();
	searchResult = static_cast<CContainerUI *>(m_PaintManager.FindControl(L"SearchResult"));
	lbTips = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Tips"));
	searchResult->SetVisible(false);
}

void AddFriendWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender == btnClose)
		{
			ShowWindow(false);
			return;
		}
		if (msg.pSender == btnSearch)
		{
			CDuiString account = edSearch->GetText();
			if (account == client->user->account)
			{
				ShowTips(L"不能查找自己！", true);
			}
			else if (account == L"")
			{
				ShowTips(L"请输入查找的账号！", true);
			}
			else
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("protocol");
				writer.Int(SEARCH_FRIENDS);
				writer.Key("account");
				writer.String(WToA(account));
				writer.EndObject();
				const char *json = s.GetString();

				if (!client->Send((BYTE*)json, strlen(json) + 1))
				{
					ShowTips(L"发送失败，请检查网络连接！", true);
					return;
				}

				threadPool->QueueTaskItem(Task::SearchWait, (WPARAM)this, NULL);
			}
			return;
		}
		if (msg.pSender == btnAdd)
		{
			if (friendID != 0)
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("protocol");
				writer.Int(FRIENDS_REQUEST);
				writer.Key("userID");
				writer.Int(client->user->userID);
				writer.Key("friedID");
				writer.Int(friendID);
				writer.EndObject();
				const char *json = s.GetString();

				if (!client->Send((BYTE*)json, strlen(json) + 1))
				{
					ShowTips(L"发送失败，请检查网络连接！", true);
					return;
				}
				ShowTips(L"好友请求发送成功！");
				edSearch->SetText(L"");
				edSearch->SetFocus();
				btnAdd->SetEnabled(false);
			}
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

LRESULT AddFriendWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:   //禁止双击放大窗口
		return 0;
	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void AddFriendWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void AddFriendWnd::ShowTips(CDuiString tips, BOOL bWarning)
{
	if (bWarning)
	{
		lbTips->SetText(tips);
		lbTips->SetTextColor(0xFFFF0000);
	}
	else
	{
		lbTips->SetText(tips);
		lbTips->SetTextColor(0xFF53CB4E);
	}
}

void AddFriendWnd::SetSearchBtnEnable(BOOL enable)
{
	btnSearch->SetEnabled(enable);
}

void AddFriendWnd::SearchResult(UserAndFriend * user)
{
	SetEvent(searchEvent);
	if (user == NULL)
	{
		searchResult->SetVisible(false);
		ShowTips(L"查找失败，无此账号的用户！", true);
	}
	else
	{
		auto it = client->friends.begin();
		for (; it != client->friends.end(); it++)
		{
			if ((*it)->userID == user->userID)
			{
				btnAdd->SetText(L"已是好友");
				btnAdd->SetEnabled(false);
				break;
			}
		}
		if (it == client->friends.end())
		{
			btnAdd->SetText(L"加好友");
			btnAdd->SetEnabled();

		}
		CButtonUI *headerImg = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"HeaderImg"));
		if (headerImg)
		{
			CString str;
			str.Format(L"HeaderImg%1d.png", user->headerImg);
			headerImg->SetBkImage(str);
		}

		CLabelUI *nickName = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"NickName"));
		if (nickName)
		{
			nickName->SetText(user->nickName);
		}

		CLabelUI *signature = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Signture"));
		if (signature)
		{
			signature->SetText(user->signature);
		}
		searchResult->SetVisible();
		friendID = user->userID;
		ShowTips(L"查找成功！");
		delete user;
	}
}
