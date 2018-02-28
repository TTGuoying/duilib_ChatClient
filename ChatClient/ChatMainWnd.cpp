#include "ChatMainWnd.h"
#include "LoginWnd.h"

DUI_BEGIN_MESSAGE_MAP(ChatMainWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_END_MESSAGE_MAP()

ChatMainWnd::ChatMainWnd()
{
	
}


ChatMainWnd::~ChatMainWnd()
{
}

UILIB_RESOURCETYPE ChatMainWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif // _DEBUG
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
	sessionList = static_cast<CListUI *>(m_PaintManager.FindControl(L"SessionList"));
	chatRoom = static_cast<CListUI *>(m_PaintManager.FindControl(L"ChatRoom"));

	SessionItemInfo *item = new SessionItemInfo;
	item->headerImg = L"default.jpg";
	item->nickName = L"Friend";
	item->message = L"你好";
	item->msgTime = L"18:00";
	item->mute = false;
	item->newMsg = false;
	sessionList->Add(CreateSessionItem(item));

}

void ChatMainWnd::Notify(TNotifyUI & msg)
{
	
	return WindowImplBase::Notify(msg);
}

LRESULT ChatMainWnd::OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE)
	{
		::PostQuitMessage(0);
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
	__super::OnFinalMessage(hWnd);
	delete this;
}

void ChatMainWnd::OnClick(TNotifyUI & msg)
{
	if (msg.pSender == btnClose)
	{
		::PostQuitMessage(0);
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
