#pragma once
#include "stdafx.h"
#include "Common.h"

class Client;
class ThreadPool;
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
	void LoginSuccess();
	void BackToLogin();
	virtual void Notify(TNotifyUI &msg);

	virtual LRESULT OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnMouseHover(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnChar(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual void OnFinalMessage(HWND hWnd);

	void UpdateFriendList();					// 更新好友列表
	void AddTopSessionItem(ULONG friendID);		// 添加会话列表项并置顶
	void ShowSesionItem();						// 会话列表项
	void ViewSessionChatRoom(ULONG friendID);	// 显示聊天框
	BOOL SendMsg();								// 发送消息
	void OnFriendRequest(FriendRequest *request);// 发送消息
	void RecvSingleMsg(ChatMsgItem * msg);		// 接收消息
	void OtherPlaceSignin();		// 其他地方登录
	void ConnectClose();

	Client * client;
	ThreadPool * threadPool;
	AddFriendWnd *addFriendWnd;	// 添加好友窗口
	BOOL initSuccess;
	BOOL bClose;

	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);

private:
	CListContainerElementUI * CreateSessionItem(SessionItem *item);
	CListContainerElementUI * CreateFriendItem();
	CListContainerElementUI * CreateFrientItem(UserAndFriend *user);
	CListContainerElementUI * CreateChatMsgItem(ChatMsgItem *item);
	CListContainerElementUI * CreateNewFriendItem(FriendRequest *request);
	CListContainerElementUI * CreateFriendDetail(UserAndFriend *user);
	CListUI * CreateChatMsgList();

	SessionItem *friendISession;
	CLabelUI *title;
	CButtonUI *btnClose;
	CButtonUI *btnMax;
	CButtonUI *btnRestore;
	CButtonUI *btnMin;

	CButtonUI *btnSession;		// 会话tab
	CButtonUI *btnContact;		// 好友tab
	CButtonUI *btnSearchFriend;	// 查找添加好友

	CListUI *sessionList;		// 会话列表
	CContainerUI *chatRoom;			// 聊天界面
	CRichEditUI *edInput;		// 聊天界面
	CButtonUI *btnSend;

	CListUI *friendList;		// 好友列表
	CListUI *friendDetailList;	// 好友详情
};

