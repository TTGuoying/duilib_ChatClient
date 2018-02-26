#include "RegisterWnd.h"

#include "ChatMainWnd.h"

RegisterWnd::RegisterWnd()
{

}


RegisterWnd::~RegisterWnd()
{
}

UILIB_RESOURCETYPE RegisterWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif // _DEBUG
}

CDuiString RegisterWnd::GetSkinFolder()
{
	return L"Skin\\";
}

CDuiString RegisterWnd::GetSkinFile()
{
	return L"RegistWnd.xml";
}

CDuiString RegisterWnd::GetZIPFileName() const
{
	return L"Skin.zip";
}

CControlUI * RegisterWnd::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

LPCTSTR RegisterWnd::GetWindowClassName() const
{
	return L"RegistWnd";
}

void RegisterWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"btnclose"));
	btnSignUp = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SignUp"));
	btnBack = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Back"));
	account = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Account"));
	password = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Password"));
	account->SetFocus();

}

void RegisterWnd::Notify(TNotifyUI & msg)
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
			Close(1);
			return;
		}
		else if (msg.pSender == btnBack)
		{
			Close(0);
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

void RegisterWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

