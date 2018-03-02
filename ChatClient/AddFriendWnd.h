#pragma once
#include "stdafx.h"
#include "Common.h"
#include "Client.h"
#include "ThreadPool.h"

class AddFriendWnd : public WindowImplBase
{
public:
	AddFriendWnd(WindowImplBase *wnd, Client *client, ThreadPool *threadPool);
	~AddFriendWnd();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetZIPFileName() const;

	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);
	void ShowTips(CDuiString tips = L"", BOOL bBarning = FALSE);
	void SetSearchBtnEnable(BOOL enable = TRUE);

	HANDLE searchEvent;
	ULONG friendID;

	WindowImplBase *wnd;
	Client *client;
	ThreadPool *threadPool;

	CButtonUI * btnClose;
	CEditUI *edSearch;
	CButtonUI * btnSearch;
	CButtonUI * btnAdd;
	CLabelUI *lbTips;
	CContainerUI *searchResult;
};

