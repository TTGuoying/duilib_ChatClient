#include "ChatMainWnd.h"
#include "LoginWnd.h"
#include "AddFriendWnd.h"
#include "Task.h"
#include "Client.h"
#include "ThreadPool.h"

DUI_BEGIN_MESSAGE_MAP(ChatMainWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_END_MESSAGE_MAP()

ChatMainWnd::ChatMainWnd(Client *client, ThreadPool *threadPool)
{
	
	this->client = client;
	this->threadPool = threadPool;
	addFriendWnd = NULL;
	friendISession = NULL;
	initSuccess = false;
	bClose = false;
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
	title = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Title"));
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"closebtn"));
	btnMax = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"maxbtn"));
	btnRestore = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"restorebtn"));
	btnMin = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"minbtn"));
	
	btnSession = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Session"));
	btnContact = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Contact"));

	btnSearchFriend = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"AddFriend"));

	sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
	chatRoom = static_cast<CContainerUI *>(m_PaintManager.FindControl(L"ChatRoom"));
	btnSend = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SendBtn"));
	friendList = static_cast<CListUI *>(m_PaintManager.FindControl(L"ContactList"));
	friendDetailList = static_cast<CListUI *>(m_PaintManager.FindControl(L"ContactDetail"));
	edInput = static_cast<CRichEditUI *>(m_PaintManager.FindControl(L"InputBox")); 

	CButtonUI *myHeaderImg = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"MyheaderImg"));
	CString str;
	str.Format(L"HeaderImg%1d.png", client->user->headerImg);
	myHeaderImg->SetBkImage(str);
	str.Format(L"%s %s", client->user->nickName, client->user->account);
	myHeaderImg->SetToolTip(str);

	for (auto it = client->friends.begin(); it != client->friends.end(); it++)
	{
		UserAndFriend *item = *it;
		friendList->Add(CreateFrientItem(item));
	}
	initSuccess = true;
}

void ChatMainWnd::LoginSuccess()
{
	CButtonUI *myHeaderImg = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"MyheaderImg"));
	CString str;
	str.Format(L"HeaderImg%1d.png", client->user->headerImg);
	myHeaderImg->SetBkImage(str);
	str.Format(L"%s %s", client->user->nickName, client->user->account);
	myHeaderImg->SetToolTip(str);

	for (auto it = client->friends.begin(); it != client->friends.end(); it++)
	{
		UserAndFriend *item = *it;
		friendList->Add(CreateFrientItem(item));
	}
}

void ChatMainWnd::BackToLogin()
{

}

void ChatMainWnd::UpdateFriendList()
{
	if (initSuccess)
	{
		friendList->RemoveAll();
		friendList->Add(CreateFriendItem());
		for (auto it = client->friends.begin(); it != client->friends.end(); it++)
		{
			UserAndFriend *item = *it;
			friendList->Add(CreateFrientItem(item));
		}
	}
	
}

void ChatMainWnd::AddTopSessionItem(ULONG friendID)
{
	SessionItem *sessionItem = NULL;
	for (auto it = client->sessions.begin(); it != client->sessions.end(); it++)
	{
		if ((*it)->friendUser->userID == friendID)
		{
			sessionItem = *it;
			client->sessions.erase(it);
			client->sessions.push_front(sessionItem);
			friendISession = sessionItem;
			break;
		}
	}
	
	if (sessionItem == NULL)
	{
		UserAndFriend *myFriend = NULL;
		for (auto it = client->friends.begin(); it != client->friends.end(); it++)
		{
			if ((*it)->userID == friendID)
			{
				myFriend = (*it);
				break;
			}
		}
		if (myFriend != NULL)
		{
			SessionItem *item = new SessionItem;
			item->friendUser = myFriend;
			item->latestMsg = L"";
			item->msgTime = L"";
			item->mute = false;
			item->newMsg = false;
			item->msgListUI = CreateChatMsgList();
			client->sessions.push_front(item); 
			friendISession = item;
		}
	}
	ShowSesionItem();
	if (sessionItem == NULL)
	{
		ViewSessionChatRoom(friendID);
	}
	
	btnSession->Activate();
	btnSession->SetNormalImage(L"");
}

void ChatMainWnd::ShowSesionItem()
{
	sessionList->RemoveAll();
	for (auto it = client->sessions.rbegin(); it != client->sessions.rend(); it++)
	{
		sessionList->AddAt(CreateSessionItem(*it), 0);
	}
	edInput->SetFocus();
}

void ChatMainWnd::ViewSessionChatRoom(ULONG friendID)
{
	SessionItem *sessionItem = NULL;
	for (auto it = client->sessions.begin(); it != client->sessions.end(); it++)
	{
		if (friendID == (*it)->friendUser->userID)
		{
			sessionItem = *it;
			//threadPool->QueueTaskItem(Task::ChatListEndDown, (WPARAM)sessionItem->msgListUI, 0);
			break;
		}
	}
	if (sessionItem != NULL)
	{
		friendISession = sessionItem;
		title->SetText(sessionItem->friendUser->nickName);
		static_cast<CContainerUI*>(m_PaintManager.FindControl(L"ChatView"))->SetVisible();
		
		chatRoom->Add(sessionItem->msgListUI);
	}
}

BOOL ChatMainWnd::SendMsg()
{
	CDuiString msg = edInput->GetText();
	if (msg == L"")
	{
		return false;
	}
	else
	{
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("protocol");
		writer.Int(SINGLE_CHAT_MSG);
		writer.Key("from");
		writer.Int(client->user->userID);
		writer.Key("to");
		writer.Int(friendISession->friendUser->userID);
		writer.Key("msg");
		writer.String(WToA(msg));
		writer.Key("time");
		writer.String(WToA(GetCurrentTimeString()));
		writer.EndObject();
		const char *json = s.GetString();

		if (!client->Send((BYTE*)json, strlen(json) + 1))
		{
			return 0;
		}
		else
		{
			ChatMsgItem * msgItem = new ChatMsgItem;
			msgItem->bMyself = true;
			msgItem->headerImg = client->user->headerImg;
			msgItem->msg = msg;
			friendISession->msgRecords.push_back(msgItem);
			friendISession->msgListUI->Add(CreateChatMsgItem(msgItem));
			threadPool->QueueTaskItem(Task::ChatListEndDown, (WPARAM)friendISession->msgListUI, 0);
			edInput->SetText(L"");
			//ViewSessionChatRoom(friendISession->friendUser->userID);
			return true;
		}
	}
}

void ChatMainWnd::OnFriendRequest(FriendRequest * request)
{
	btnContact->SetAttributeList(L"normalimage=\"file='UnReadDot.png' dest='12,0,24,12'\"");
	CButtonUI *btnNewFriendHead = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"NewFriendHeadImg"));;
	if (btnNewFriendHead != 0)
	{
		btnNewFriendHead->SetAttributeList(_T("normalimage=\"file='UnReadDot.png' dest='28,-4,40,8'\""));
	}
	client->friendRequests.push_back(request);
}

void ChatMainWnd::RecvSingleMsg(ChatMsgItem * msg)
{
	if (msg == NULL)
		return;
	AddTopSessionItem(msg->from);
	for (auto it = client->sessions.begin(); it != client->sessions.end(); it++)
	{
		if ((*it)->friendUser->userID == msg->from)
		{
			(*it)->latestMsg = msg->msg;
			(*it)->newMsg = true;
			(*it)->msgTime = msg->time;
			msg->headerImg = (*it)->friendUser->headerImg;
			(*it)->msgRecords.push_back(msg);
			(*it)->msgListUI->Add(CreateChatMsgItem(msg));
			if (friendISession->friendUser->userID == msg->from)
				//threadPool->QueueTaskItem(Task::ChatListEndDown, (WPARAM)friendISession->msgListUI, 0);
				//ViewSessionChatRoom(msg->from);
			break;
		}
	}
	ShowSesionItem();
}


void ChatMainWnd::ConnectClose()
{
	bClose = true;
	MessageBox(m_hWnd, L"网络已断开！", L"网络已断开！", 0);
	::PostQuitMessage(5);
	Close(5);
}

void ChatMainWnd::OtherPlaceSignin()
{
	bClose = true;
	MessageBox(m_hWnd, L"账号在其他地方登录！", L"账号在其他地方登录！", 0);
	::PostQuitMessage(5);
	Close(5);
}

void ChatMainWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == L"itemactivate")	//响应双击
	{
		if (msg.pSender->GetName() == L"FriendItem") // 双击好友列表的item
		{
			AddTopSessionItem(msg.pSender->GetTag());
			ViewSessionChatRoom(msg.pSender->GetTag());
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

void ChatMainWnd::OnClick(TNotifyUI & msg) // 单击事件
{
	if (msg.pSender == btnSend)
	{
		SendMsg();
	}
	else if (msg.pSender == btnSearchFriend) // 查找好友窗口
	{
		if (addFriendWnd == NULL)
		{
			addFriendWnd = new AddFriendWnd(this, client, threadPool);
			if (addFriendWnd == NULL)
				return ;
			addFriendWnd->Create(m_hWnd, _T("添加好友"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
		}

		addFriendWnd->CenterWindow();
		addFriendWnd->ShowWindow();

		return;
	}
	else if (msg.pSender->GetName() == L"Agree") // 同意好友请求
	{
		ULONG friendID = msg.pSender->GetTag();
		if (friendID == 0)
			return;
		
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("protocol");
		writer.Int(AGREE_FRIENDS_REQUEST);
		writer.Key("userID");
		writer.Int(client->user->userID);
		writer.Key("friendID");
		writer.Int(friendID);
		writer.EndObject();
		const char *json = s.GetString();

		if (!client->Send((BYTE*)json, strlen(json) + 1))
		{
			return;
		}


		for (auto it = client->friendRequests.begin(); it != client->friendRequests.end(); it++)
		{
			if (friendID == (*it)->userID)
			{
				(*it)->operation = 1;
				break;
			}
		}


		friendDetailList->RemoveAll();
		for (auto it = client->friendRequests.begin(); it != client->friendRequests.end(); it++)
		{
			friendDetailList->Add(CreateNewFriendItem(*it));
		}

		return;
	}
	else if (msg.pSender->GetName() == L"Refuse") // 拒绝好友请求
	{
		ULONG friendID = msg.pSender->GetTag(); 
		if (friendID == 0)
			return;

		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("protocol");
		writer.Int(REFUSE_FRIENDS_REQUEST);
		writer.Key("userID");
		writer.Int(client->user->userID);
		writer.Key("friendID");
		writer.Int(friendID);
		writer.EndObject();
		const char *json = s.GetString();

		if (!client->Send((BYTE*)json, strlen(json) + 1))
		{
			return;
		}

		for (auto it = client->friendRequests.begin(); it != client->friendRequests.end(); it++)
		{
			if (friendID == (*it)->userID)
			{
				(*it)->operation = 0;
				break;
			}
		}

		friendDetailList->RemoveAll();
		for (auto it = client->friendRequests.begin(); it != client->friendRequests.end(); it++)
		{
			friendDetailList->Add(CreateNewFriendItem(*it));
		}
		return;
	}
	else if (msg.pSender->GetName() == L"SendMsg") // 好友详情发送信息按钮
	{
		AddTopSessionItem(msg.pSender->GetTag());
		return;
	}
	else if (msg.pSender == btnClose)
	{
		bClose = true;
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
		bClose = true;
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
	if (pressChar == VK_RETURN && edInput->IsFocused() /*焦点在输入框*/)
	{
		return SendMsg();
	}
	return WindowImplBase::OnChar(uMsg, wParam, lParam, bHandled);
}

void ChatMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void ChatMainWnd::OnSelectChanged(TNotifyUI & msg)
{
	if (msg.pSender == btnSession)
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(0);
		btnSession->SetNormalImage(L"");
	}
	else if (msg.pSender == btnContact)
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(1);
		btnContact->SetNormalImage(L"");
	}
}

void ChatMainWnd::OnItemClick(TNotifyUI & msg)
{
	if (msg.pSender->GetName() == L"SessionItem") // 点击会话列表的item
	{
		ViewSessionChatRoom(msg.pSender->GetTag());
	}
	else if (msg.pSender->GetName() == L"NewFriend") // 点击好友列表的新的朋友item
	{
		friendDetailList->RemoveAll();
		CButtonUI *btnNewFriendHead = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"NewFriendHeadImg"));;
		if (btnNewFriendHead != 0)
		{
			btnNewFriendHead->SetNormalImage(L"");
		}
		for (auto it = client->friendRequests.begin(); it != client->friendRequests.end(); it++)
		{
			friendDetailList->Add(CreateNewFriendItem(*it));
		}
		title->SetText(L"新朋友");
	}
	else if (msg.pSender->GetName() == L"FriendItem") // 点击好友列表的item
	{
		int i = msg.pSender->GetTag();
		UserAndFriend *myFriend = NULL;
		for (auto it = client->friends.begin(); it != client->friends.end(); it++)
		{
			if (i == (*it)->userID)
			{
				myFriend = (*it);
			}
		}
		if (myFriend != NULL)
		{
			title->SetText(myFriend->nickName);
			friendDetailList->RemoveAll();
			friendDetailList->Add(CreateFriendDetail(myFriend));
		}
	}
}


CListContainerElementUI * ChatMainWnd::CreateSessionItem(SessionItem * item)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("SessionItem.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return false;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CDuiString str;
		str.Format(L"HeaderImg%1d.png", item->friendUser->headerImg);
		headerImg->SetBkImage(str);
		if (item->newMsg)
		{
			headerImg->SetAttributeList(_T("normalimage=\"file='UnReadDot.png' dest='28,-4,40,8'\""));
		}
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(item->friendUser->nickName);
	}

	CLabelUI *message = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Message"));
	if (message)
	{
		message->SetText(item->latestMsg);
	}

	CLabelUI *msgTime = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"MsgTime"));
	if (msgTime)
	{
		if (item->msgTime != L"")
		{
			item->msgTime=item->msgTime.Right(8);
			item->msgTime = item->msgTime.Left(5);
		}
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
	listElement->SetTag(item->friendUser->userID);
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateFriendItem() // 新的朋友
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("NewFriend.xml"), (UINT)0, NULL, &m_PaintManager));
	listElement->SetTag(-1);
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateFrientItem(UserAndFriend * user) // 我的好友
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("FriendItem.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return NULL;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CString str;
		str.Format(L"HeaderImg%1d.png", user->headerImg);
		headerImg->SetBkImage(str);
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(user->nickName);
	}

	listElement->SetTag(user->userID);
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateChatMsgItem(ChatMsgItem * item)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	if (!item->bMyself)
		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("FriendChatIten.xml"), (UINT)0, NULL, &m_PaintManager));
	else
		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("MyselfChatItem.xml"), (UINT)0, NULL, &m_PaintManager));
	if (listElement == NULL)
		return NULL;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CString str;
		str.Format(L"HeaderImg%1d.png", item->headerImg);
		headerImg->SetBkImage(str);
	}

	CLabelUI *message = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Message"));
	if (message)
	{
		message->SetText(item->msg);
	}
	int len = item->msg.GetLength();
	CContainerUI *msgItem = static_cast<CContainerUI*>(m_PaintManager.FindSubControlByName(listElement, L"MsgItem"));
	if (msgItem)
	{
		msgItem->SetFixedHeight((len / 13 + 2) * 25);
	}

	CContainerUI *msgContainer = static_cast<CContainerUI*>(m_PaintManager.FindSubControlByName(listElement, L"MsgContainer"));
	if (msgContainer)
	{
		if (len > 13)
		{
			msgContainer->SetFixedWidth(200);
			msgContainer->SetFixedHeight((len / 13 + 2) * 25 - 10);
		}
		else
		{
			msgContainer->SetFixedWidth(15 * len + 25);
			msgContainer->SetFixedHeight((len / 13 + 2) * 25 -10);
		}
	}

	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateNewFriendItem(FriendRequest *request)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("NewFriendItem.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return NULL;

	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CString str;
		str.Format(L"HeaderImg%1d.png", request->headerImg);
		headerImg->SetBkImage(str);
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(request->nickName);
	}

	CLabelUI *signture = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Signture"));
	if (nickName)
	{
		signture->SetText(request->signature);
	}
	CButtonUI *btnAgree = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Agree"));
	CButtonUI *btnRefuse = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Refuse"));
	if (request->operation == -1)
	{
		if (btnAgree)
		{
			btnAgree->SetTag(request->userID);
		}
		if (btnRefuse)
		{
			btnRefuse->SetTag(request->userID);
		}
	}
	else if (request->operation == 0)
	{
		if (btnAgree)
		{
			btnAgree->SetEnabled(false);
		}
		if (btnRefuse)
		{
			btnRefuse->SetEnabled(false);
			btnRefuse->SetText(L"已拒绝");
		}
	}

	else if (request->operation == 1)
	{
		if (btnAgree)
		{
			btnAgree->SetEnabled(false);
			btnAgree->SetText(L"已同意");
		}
		if (btnRefuse)
		{
			btnRefuse->SetEnabled(false);
		}
	}
	
	return listElement;
}

CListContainerElementUI * ChatMainWnd::CreateFriendDetail(UserAndFriend * user)
{
	CListContainerElementUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("FriendDetail.xml"), (UINT)0, NULL, &m_PaintManager));

	if (listElement == NULL)
		return NULL;
	CButtonUI *headerImg = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"HeaderImg"));
	if (headerImg)
	{
		CString str;
		str.Format(L"HeaderImg%1d.png", user->headerImg);
		headerImg->SetBkImage(str);
	}

	CLabelUI *nickName = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"NickName"));
	if (nickName)
	{
		nickName->SetText(user->nickName);
	}

	CLabelUI *signture = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"Signture"));
	if (nickName)
	{
		signture->SetText(user->signature);
	}
	CButtonUI *btnSend = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(listElement, L"SendMsg"));
	if (btnSend)
	{
		btnSend->SetTag(user->userID);
	}
	return listElement;
}

CListUI * ChatMainWnd::CreateChatMsgList()
{
	CListUI *listElement = NULL;
	CDialogBuilder dlgBuilder;
	listElement = static_cast<CListUI*>(dlgBuilder.Create(_T("ChatMsgList.xml"), (UINT)0, NULL, &m_PaintManager));

	return listElement;
}
