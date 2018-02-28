#pragma once
#include "stdafx.h"
#include "Common.h"
#include "Client.h"
#include "ThreadPool.h"

class LoginWnd : public WindowImplBase
{
public:
	LoginWnd(Client *client, ThreadPool *threadPool);
	~LoginWnd();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetZIPFileName() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	void ShowTip(CDuiString tips = L"", BOOL bBarning = FALSE);
	Client * client;
	void Connect();
	void SetSignInBtnEnable(BOOL enable = TRUE);
	void SetSignUpBtnEnable(BOOL enable = TRUE);
	void SignInBtnSuccess();
	void SignUpSuccess();

	HANDLE signInEvent;
	HANDLE signUpEvent;
private:
	ThreadPool *threadPool;

	CButtonUI * btnClose;
	CLabelUI * lbTitle;
	CLabelUI * lbTips;

	CEditUI *edAccount;
	CEditUI *edPassword;
	CButtonUI * btnSetting;
	CButtonUI * btnGoSignUp;
	CButtonUI * btnSignIn;

	CDuiString ip;
	CEditUI *edIPAddress;
	CDuiString port;
	CEditUI *edPort;
	CButtonUI * btnBack;
	CButtonUI * btnOK;

	int		headImg;
	CEditUI *edSignUpAccount;
	CEditUI *edSignUpPassword;
	CEditUI *edSignUpPassword2;
	CEditUI *edSignUpNickName;
	CEditUI *edSignUpSignature;
	int		sexulity;
	CEditUI *edSignUpAera;
	CEditUI *edSignUpPhone;

	CButtonUI * btnSignUp;
};

