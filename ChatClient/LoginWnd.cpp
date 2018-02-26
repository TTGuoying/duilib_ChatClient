#include "LoginWnd.h"
#include "RegisterWnd.h"

#include "ChatMainWnd.h"

LoginWnd::LoginWnd()
{

}


LoginWnd::~LoginWnd()
{
}

UILIB_RESOURCETYPE LoginWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif // _DEBUG
}

CDuiString LoginWnd::GetSkinFolder()
{
	return L"Skin\\";
}

CDuiString LoginWnd::GetSkinFile()
{
	return L"LoginWnd.xml";
}

CDuiString LoginWnd::GetZIPFileName() const
{
	return L"Skin.zip";
}

CControlUI * LoginWnd::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

LPCTSTR LoginWnd::GetWindowClassName() const
{
	return L"LoginWnd";
}

void LoginWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"btnclose"));
	btnSetting = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Setting"));
	btnSignUp = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SignUp"));
	btnSignIn = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SignIn"));
	account = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Account"));
	password = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Password"));
	account->SetFocus();

}

void LoginWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender == btnClose)
		{
			Close(0);
			return;
		}
		else if (msg.pSender == btnSignUp)
		{
			RegisterWnd *registerWnd = new RegisterWnd;
			if (registerWnd == NULL)
				return ;
			registerWnd->Create(m_hWnd, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
			registerWnd->CenterWindow();
			if (registerWnd->ShowModal() == 0)
				;
			
		}
		else if (msg.pSender == btnSignIn)
		{
			Close(1);
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

void LoginWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}
