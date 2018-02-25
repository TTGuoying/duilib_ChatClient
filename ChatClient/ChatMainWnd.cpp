#include "stdafx.h"
#include "ChatMainWnd.h"

DUI_BEGIN_MESSAGE_MAP(ChatMainWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_END_MESSAGE_MAP()

ChatMainWnd::ChatMainWnd()
{
	//sessionList = new SessionList(m_PaintManager);
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
	if (_tcsicmp(pstrClass, _T("SessionList")) == 0)
	{
		return new SessionList(m_PaintManager);
	}

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
			sessionList = static_cast<SessionList *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = true;
		item->newMsg = true;
		sessionList->Add(sessionList->AddItem(item));

		bHandled = true;
	}
	else if (pressChar == 'a')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<SessionList *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = true;
		item->newMsg = false;
		sessionList->Add(sessionList->AddItem(item));
	}
	else if (pressChar == 's')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<SessionList *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = false;
		item->newMsg = true;
		sessionList->Add(sessionList->AddItem(item));
	}
	else if (pressChar == 'd')
	{
		if (sessionList == NULL)
		{
			sessionList = static_cast<SessionList *>(m_PaintManager.FindControl(L"SessionList"));
		}

		SessionItemInfo *item = new SessionItemInfo;
		item->headerImg = L"default.jpg";
		item->nickName = L"Friend";
		item->message = L"你好";
		item->msgTime = L"18:00";
		item->mute = false;
		item->newMsg = false;
		sessionList->Add(sessionList->AddItem(item));
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
		//static_cast<CTabLayoutUI*>()
	}
	else if (msg.pSender->GetName() == L"Contacts")
	{
		//static_cast<CTabLayoutUI*>()
	}
}

void ChatMainWnd::OnItemClick(TNotifyUI & msg)
{
}
