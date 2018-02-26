#pragma once
#include "stdafx.h"


class RegisterWnd : public WindowImplBase
{
public:
	RegisterWnd();
	~RegisterWnd();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetZIPFileName() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI &msg);

	virtual void OnFinalMessage(HWND hWnd);

private:
	CButtonUI * btnClose;
	CButtonUI * btnSignUp;
	CButtonUI * btnBack;
	CEditUI *account;
	CEditUI *password;
};

