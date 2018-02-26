#pragma once
#include "stdafx.h"
#include "Common.h"

class LoginWnd : public WindowImplBase
{
public:
	LoginWnd();
	~LoginWnd();

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
	CButtonUI * btnSetting;
	CButtonUI * btnSignUp;
	CButtonUI * btnSignIn;
	CEditUI *account;
	CEditUI *password;
};

