#pragma once
#include "stdafx.h"
#include "Common.h"
#include "Client.h"
#include "ThreadPool.h"

class AddFriendWnd;
class ChatMainWnd : public WindowImplBase
{
public:
	ChatMainWnd(Client *client, ThreadPool *threadPool);
	~ChatMainWnd();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetZIPFileName() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI &msg);

	virtual LRESULT OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnMouseHover(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnChar(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);
	void OnItemDoubleClick(TNotifyUI& msg);


	Client * client;
	ThreadPool * threadPool;
private:
	CListContainerElementUI * CreateSessionItem(SessionItemInfo *item);
	CListContainerElementUI * CreateFrientItem(UserAndFriend *item);
	CListContainerElementUI * CreateChatMsgItem(ChatMsgItem *item, bool myself = true);

	CButtonUI *btnClose;
	CButtonUI *btnMax;
	CButtonUI *btnRestore;
	CButtonUI *btnMin;
	CButtonUI *btnAddFriend;
	CListUI *sessionList;
	CListUI *chatRoom;
	CListUI *FriendList;
	AddFriendWnd *addFriendWnd;
};

