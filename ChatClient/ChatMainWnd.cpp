#include "ChatMainWnd.h"
#include "LoginWnd.h"
#include "AddFriendWnd.h"
#include "Task.h"

DUI_BEGIN_MESSAGE_MAP(ChatMainWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_END_MESSAGE_MAP()

ChatMainWnd::ChatMainWnd(Client *client, ThreadPool *threadPool)
{
	this->client = client;
	this->threadPool = threadPool;
	client->AssociateWnd(this);
	addFriendWnd = NULL;
	client->AssociateWnd(this);
}


ChatMainWnd::~ChatMainWnd()
{
}

UILIB_RESOURCETYPE ChatMainWnd::GetResourceType() const
{
//#ifdef _DEBUG
//	return UILIB_FILE;
//#else
//	return UILIB_ZIP;
//#endif // _DEBUG
	return UILIB_FILE;
}

CDuiString ChatMainWnd::GetSkinFolder()
{
	return L"Skin\\";
}

CDuiString ChatMainWnd::GetSkinFile()
{
	return L"ChatMain.xml";
}

CDuiString ChatMainWnd::GetZIPFileName() const
{
	return L"Skin.zip";
}

CControlUI * ChatMainWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LPCTSTR ChatMainWnd::GetWindowClassName() const
{
	return L"ChatMainWnd";
}

void ChatMainWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"closebtn"));
	btnMax = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"maxbtn"));
	btnRestore = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"restorebtn"));
	btnMin = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"minbtn"));
	btnAddFriend = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"AddFriend"));

	CButtonUI *myHeaderImg = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"MyheaderImg"));
	CString str;
	str.Format(L"HeaderImg%1d.png", client->user->headerImg);
	myHeaderImg->SetBkImage(str);
	str.Format(L"%s %s", client->user->nickName, client->user->account);
	myHeaderImg->SetToolTip(str);

	sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
	chatRoom = static_cast<CListUI *>(m_PaintManager.FindControl(L"ChatRoom"));
	FriendList = static_cast<CListUI *>(m_PaintManager.FindControl(L"ContactList"));
	

	SessionItemInfo *item = new SessionItemInfo;
	item->headerImg = L"default.jpg";
	item->nickName = L"Friend";
	item->message = L"你好";
	item->msgTime = L"18:00";
	item->mute = false;
	item->newMsg = false;
	sessionList->Add(CreateSessionItem(item));

	for (auto it = client->friends.begin(); it != client->friends.end(); it++)
	{
		UserAndFriend *item = *it;
		FriendList->Add(CreateFrientItem(item));
	}
}

LRESULT ChatMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_USER_RECVDATA:
		threadPool->QueueTaskItem(Task::ProcessRecvDate, wParam, (LPARAM)this);
		return 0;
	case WM_USER_SEARCH_FRIEND:
		addFriendWnd->SendMessageW(WM_USER_SEARCH_FRIEND, wParam, lParam);
		return 0;
	case 11111:
		
		return 0;
	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void ChatMainWnd::Notify(TNotifyUI & msg)
{

	if (msg.sType == DUI_MSGTYPE_ITEMDBCLICK)
	{
		return WindowImplBase::Notify(msg);
	}
	if (msg.sType == L"itemactivate")	//响应双击
	{
		return WindowImplBase::Notify(msg);
	}
	return WindowImplBase::Notify(msg);
}

void ChatMainWnd::OnClick(TNotifyUI & msg)
{
	if (msg.pSender == btnAddFriend)
	{
		if (addFriendWnd == NULL)
		{
			addFriendWnd = new AddFriendWnd(this, client, threadPool);
			if (addFriendWnd == NULL)
				return ;
			addFriendWnd->Create(NULL, _T("添加好友"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
		}

		addFriendWnd->CenterWindow();
		addFriendWnd->ShowWindow();

		return;
	}
	else if (msg.pSender == btnClose)
	{
		::PostQuitMessage(0);
		Close();
		return;
	}
	else if (msg.pSender == btnMin)
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return;
	}
	else if (msg.pSender == btnMax)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		return;
	}
	else if (msg.pSender == btnRestore)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return;
	}
}

LRESULT ChatMainWnd::OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE)
	{
		::PostQuitMessage(0);
		Close();
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRet = CWindowWnd::HandleMessage(msg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		if (bZoomed)
		{
			btnMax->SetVisible(true);
			btnRestore->SetVisible(false);
		}
		else
		{
			btnMax->SetVisible(false);
			btnRestore->SetVisible(true);
		}
	}
	return lRet;
}

LRESULT ChatMainWnd::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI *ui = m_PaintManager.FindControl(pt);
	if (ui == NULL)
		return 0;
	/*if (ui->GetName() == L"btn")
	{

	}*/
	return 0;
}

LRESULT ChatMainWnd::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	TCHAR pressChar = (TCHAR)wParam;
	if (pressChar == VK_RETURN && TRUE /*焦点在输入框*/)
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = true;
		item->newMsg = true;
		sessionList->Add(CreateSessionItem(item));

		bHandled = true;
	}
	else if (pressChar == 'a')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = true;
		item->newMsg = false;
		sessionList->Add(CreateSessionItem(item));
	}
	else if (pressChar == 's')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = false;
		item->newMsg = true;
		sessionList->Add(CreateSessionItem(item));
	}
	else if (pressChar == 'd')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = false;
		item->newMsg = false;
		sessionList->Add(CreateSessionItem(item));
	}
	else if (pressChar == 'q')
	{
		if (chatRoom == NULL)
		{
			chatRoom = static_cast<CListUI *>(m_PaintManager.FindControl(L"ChatRoom"));
		}

		ChatMsgItem *item = new ChatMsgItem;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"a123你好你好1234你好你好你好a123你好你好1234你好你好你好";
		item->msgTime = L"18:00";
		chatRoom->Add(CreateChatMsgItem(item));
	}
	else if (pressChar == 'w')
	{
		if (chatRoom == NULL)
		{
			chatRoom = static_cast<CListUI *>(m_PaintManager.FindControl(L"ChatRoom"));
		}

		ChatMsgItem *item = new ChatMsgItem;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"a123你好你好1234你好你好你好a123你好你好1234你好你好你好";
		item->msgTime = L"18:00";
		chatRoom->Add(CreateChatMsgItem(item, false));
	}
	else
	{
		bHandled = false;
	}
	return 0;
}

void ChatMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void ChatMainWnd::OnSelectChanged(TNotifyUI & msg)
{
	if (msg.pSender->GetName() == L"Chatting")
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(0);
	}
	else if (msg.pSender->GetName() == L"Contacts")
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(1);
	}
}

void ChatMainWnd::OnItemClick(TNotifyUI & msg)
{
	if (msg.pSender->GetName() == L"SessionItem")
	{
		int i = msg.pSender->GetTag();
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"ChatView"))->SetVisible();
	}
}

void ChatMainWnd::OnItemDoubleClick(TNotifyUI & msg)
{
	if (msg.pSender->GetName() == L"SessionItem")
	{
		int i = msg.pSender->GetTag();
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"ChatView"))->SetVisible();
	}
}

CListContainerElementUI * ChatMainWnd::CreateSessionItem(SessionItemInfo * item)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("SessionItem.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return false;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		headerImg->SetBkImage(item->headerImg);
		if (item->newMsg)
		{
			headerImg->SetAttributeList(_T("normalimage=\"file='UnReadDot.png' dest='28,-4,40,8'\""));
		}
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(item->nickName);
	}

	CLabelUI *message = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Message"));
	if (message)
	{
		message->SetText(item->message);
	}

	CLabelUI *msgTime = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"MsgTime"));
	if (msgTime)
	{
		msgTime->SetText(item->msgTime);
	}

	if (item->mute)
	{
		CButtonUI *muteImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"MuteImg"));
		if (muteImg)
		{
			muteImg->SetNormalImage(L"ChatList_Mute_Icon.png");
			muteImg->SetVisible();
		}
	}
	listElement->SetTag(1);
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateFrientItem(UserAndFriend * item)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("FriendItem.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return false;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CString str;
		str.Format(L"HeaderImg%1d.png", item->headerImg);
		headerImg->SetBkImage(str);
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(item->nickName);
	}

	listElement->SetTag(1);
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateChatMsgItem(ChatMsgItem * item, bool myself)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	if (myself)
		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("FriendChatIten.xml"), (UINT)0, NULL, &m_PaintManager));
	else
		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("MyselfChatItem.xml"), (UINT)0, NULL, &m_PaintManager));
	if (listElement == NULL)
		return false;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		headerImg->SetBkImage(item->headerImg);
	}

	CLabelUI *message = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Message"));
	if (message)
	{
		message->SetText(item->message);
	}

	
	listElement->SetTag(1);
	return listElement;
}
