#pragma once
#include "stdafx.h"

class ChatMainWnd : public WindowImplBase
{
public:
	ChatMainWnd();
	~ChatMainWnd();

	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	CDuiString GetZIPFileName() const;

	LPCTSTR GetWindowClassName() const;
	void InitWindow();
	void Notify(TNotifyUI &msg);

	LRESULT OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnMouseHover(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnChar(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void OnFinalMessage(HWND hWnd);

	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);

	CButtonUI *btnClose;
	CButtonUI *btnMax;
	CButtonUI *btnRestore;
	CButtonUI *btnMin;
};

