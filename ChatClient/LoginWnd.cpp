#include "LoginWnd.h"
#include "Task.h"
#include "ChatMainWnd.h"

LoginWnd::LoginWnd(Client *client, ThreadPool *threadPool)

{
	this->client = client;
	this->threadPool = threadPool;
	signInEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	signUpEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	stopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	headerImg = 1;
	sexulity = 1;
	client->AssociateWnd(this);
}


LoginWnd::~LoginWnd()
{
	SetEvent(stopEvent);
	CloseHandle(signInEvent);
	CloseHandle(signUpEvent);
	CloseHandle(stopEvent);
}

UILIB_RESOURCETYPE LoginWnd::GetResourceType() const
{
//#ifdef _DEBUG
//	return UILIB_FILE;
//#else
//	return UILIB_ZIP;
//#endif // _DEBUG
	return UILIB_FILE;
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

LRESULT LoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_NCLBUTTONDBLCLK:   //Ω˚÷πÀ´ª˜∑≈¥Û¥∞ø⁄
		return 0;
	case WM_USER_RECONNECT:		//÷ÿ–¬¡¨Ω”∑˛ŒÒ∆˜
		threadPool->QueueTaskItem(Task::Connect, (WPARAM)this, NULL);
		break;
	case WM_USER_RECVDATA:		// ’µΩ ˝æ›
		threadPool->QueueTaskItem(Task::ProcessRecvDate, wParam, (LPARAM)this);
		break;
	case WM_USER_SIGNIN_SUCESS:		//µ«¬º≥…π¶
		client->user = (UserAndFriend *)wParam;
		break;
	case WM_USER_SIGNIN_FAIL:		//µ«¬º ß∞‹
		SetEvent(signInEvent);
		SetSignInBtnEnable();
		ShowTip(L"µ«¬º ß∞‹£¨”√ªß√˚ªÚ√‹¬Î¥ÌŒÛ£°", TRUE);
		break;
	case WM_USER_SIGNUP_SUCESS:		//◊¢≤·≥…π¶
		SignUpSuccess();
		break;
	case WM_USER_SIGNUP_FAIL:		//◊¢≤· ß∞‹
		SetEvent(signUpEvent);
		SetSignUpBtnEnable();
		ShowTip(L"◊¢≤· ß∞‹£¨”√ªß√˚“—¥Ê‘⁄£°", TRUE);
		break;
	case WM_USER_GET_FRIENDS:		//ªÒ»°∫√”—
		if (lParam == 1)
			SignInBtnSuccess();
		else
			client->friends.push_back((UserAndFriend *)wParam);
		break;
		
	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void LoginWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"btnclose"));
	lbTitle = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Title"));
	lbTips = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Tips"));

	edAccount = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Account"));
	edPassword = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Password"));
	btnSetting = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Setting"));
	btnGoSignUp = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"GoSignUp"));
	btnSignIn = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SignIn"));
	edAccount->SetFocus();

	edIPAddress = static_cast<CEditUI *>(m_PaintManager.FindControl(L"IPAddress"));
	ip = L"127.0.0.1";
	//ip = L"118.24.1.202";
	edIPAddress->SetText(ip);
	edPort = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Port"));
	port = L"10240";
	edPort->SetText(port);
	btnBack = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Back"));
	btnOK = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"OK"));

	edSignUpAccount = static_cast<CEditUI *>(m_PaintManager.FindControl(L"SignUpAccount"));
	edSignUpPassword = static_cast<CEditUI *>(m_PaintManager.FindControl(L"SignUpPassword"));
	edSignUpPassword2 = static_cast<CEditUI *>(m_PaintManager.FindControl(L"SignUpPassword2"));
	edSignUpNickName = static_cast<CEditUI *>(m_PaintManager.FindControl(L"NickName"));
	edSignUpSignature = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Signature"));
	edSignUpAera = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Area"));
	edSignUpPhone = static_cast<CEditUI *>(m_PaintManager.FindControl(L"Phone"));
	btnSignUp = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"SignUp"));

	threadPool->QueueTaskItem(Task::Connect, (WPARAM)this, NULL);
}

void LoginWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.wParam == 0)
			sexulity = 1;
		else
			sexulity = 0;
	}
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender->GetName() == L"headerImg1")
		{
			headerImg = 1;
		}
		else if (msg.pSender->GetName() == L"headerImg2")
		{
			headerImg = 2;
		}
		else if (msg.pSender->GetName() == L"headerImg3")
		{
			headerImg = 3;
		}
		else if (msg.pSender->GetName() == L"headerImg4")
		{
			headerImg = 4;
		}
	}
	else if (msg.sType == L"click")
	{
		if (msg.pSender == btnOK)
		{
			this->ResizeClient(400, 286);
			static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(0);
			this->CenterWindow();
			lbTitle->SetText(L"µ« ¬º");
			btnSetting->SetVisible();
			btnGoSignUp->SetVisible();
			btnSignIn->SetVisible();

			btnSignUp->SetVisible(false);
			btnBack->SetVisible(false);
			btnOK->SetVisible(false);

			ip = edIPAddress->GetText();
			port = edPort->GetText();
		}
		else if (msg.pSender == btnBack)
		{
			this->ResizeClient(400, 286);
			static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(0);
			this->CenterWindow();
			lbTitle->SetText(L"µ« ¬º");
			btnSetting->SetVisible();
			btnGoSignUp->SetVisible();
			btnSignIn->SetVisible();

			btnSignUp->SetVisible(false);
			btnBack->SetVisible(false);
			btnOK->SetVisible(false);

			edIPAddress->SetText(ip);
			port = L"10240";
		}
		else if (msg.pSender == btnSignUp)
		{
			CDuiString account = edSignUpAccount->GetText();
			if (account.IsEmpty())
			{
				ShowTip(L"«Î ‰»Î’À∫≈£°", TRUE);
				return;
			}
			CDuiString password = edSignUpPassword->GetText();
			if (password.IsEmpty())
			{
				ShowTip(L"«Î ‰»Î√‹¬Î£°", TRUE);
				return;
			}
			CDuiString password2 = edSignUpPassword2->GetText();
			if (password2.IsEmpty())
			{
				ShowTip(L"«Î ‰»Î»∑»œ√‹¬Î£°", TRUE);
				return;
			}
			if (password != password2)
			{
				ShowTip(L"√‹¬Î≤ª“ª÷¬£°", TRUE);
				return;
			}
			CDuiString nickname = edSignUpNickName->GetText();
			if (nickname.IsEmpty())
			{
				ShowTip(L"«Î ‰»ÎÍ«≥∆£°", TRUE);
				return;
			}
			

			StringBuffer s;
			Writer<StringBuffer> writer(s);
			writer.StartObject();
			writer.Key("protocol");
			writer.Int(SignUp);
			writer.Key("headerImg");
			writer.Int(headerImg);
			writer.Key("account");
			writer.String(WToA(edSignUpAccount->GetText()));
			writer.Key("password");
			writer.String(WToA(edSignUpPassword->GetText()));
			writer.Key("nickName");
			writer.String(WToA(edSignUpNickName->GetText()));
			writer.Key("signature");
			writer.String(WToA(edSignUpSignature->GetText()));
			writer.Key("sexulity");
			writer.Int(sexulity);
			writer.Key("area");
			writer.String(WToA(edSignUpAera->GetText()));
			writer.Key("phone");
			writer.String(WToA(edSignUpPhone->GetText()));
			writer.EndObject();
			const char *json = s.GetString();

			if (!client->Send((BYTE*)json, strlen(json) + 1))
			{
				ShowTip(L"∑¢ÀÕ ß∞‹£¨«ÎºÏ≤ÈÕ¯¬Á¡¨Ω”£°", true);
				return;
			}

			SetSignUpBtnEnable(false);
			threadPool->QueueTaskItem(Task::SignUpWait, (WPARAM)this, NULL);
			return;

		}
		else if (msg.pSender == btnSetting)
		{
			this->ResizeClient(400, 286);
			static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(1);
			this->CenterWindow();
			lbTitle->SetText(L"…Ë ÷√");
			btnBack->SetVisible();
			btnOK->SetVisible();

			btnSetting->SetVisible(false);
			btnGoSignUp->SetVisible(false);
			btnSignIn->SetVisible(false);
			btnSignUp->SetVisible(false);
		}
		else if (msg.pSender == btnGoSignUp)
		{
			static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(2);
			this->ResizeClient(400, 600);
			this->CenterWindow();
			lbTitle->SetText(L"◊¢ ≤·");
			btnBack->SetVisible();
			btnSignUp->SetVisible();

			btnSetting->SetVisible(false);
			btnGoSignUp->SetVisible(false);
			btnSignIn->SetVisible(false);
			btnOK->SetVisible(false);
		}
		else if (msg.pSender == btnClose)
		{
			Close(0);
			return;
		}
		else if (msg.pSender == btnSignIn)
		{
			CDuiString account = edAccount->GetText();
			if (account.IsEmpty())
			{
				ShowTip(L"«Î ‰»Î’À∫≈£°", TRUE);
				return;
			}
			CDuiString password = edPassword->GetText();
			if (password.IsEmpty())
			{
				ShowTip(L"«Î ‰»Î√‹¬Î£°", TRUE);
				return;
			}

			StringBuffer s;
			Writer<StringBuffer> writer(s);
			writer.StartObject();
			writer.Key("protocol");
			writer.Int(SIGNIN);
			writer.Key("account");
			writer.String(WToA(edAccount->GetText()));
			writer.Key("password");
			writer.String(WToA(edPassword->GetText()));
			writer.EndObject();
			const char *json = s.GetString();

			if (!client->Send((BYTE*)json, strlen(json) + 1))
			{
				ShowTip(L"∑¢ÀÕ ß∞‹£¨«ÎºÏ≤ÈÕ¯¬Á¡¨Ω”£°", true);
				return;
			}
			SetSignInBtnEnable(false);
			threadPool->QueueTaskItem(Task::SignInWait, (WPARAM)this, NULL);
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

void LoginWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void LoginWnd::ShowTip(CDuiString tips, BOOL bBarning)
{
	if (bBarning)
	{
		lbTips->SetText(tips);
		lbTips->SetTextColor(0xFFFF0000);
	}
	else
	{
		lbTips->SetText(tips); 
		lbTips->SetTextColor(0xFF5FD557);
	}
}

void LoginWnd::Connect()
{
	client->Start(edIPAddress->GetText(), _ttoi(edPort->GetText()));
}

void LoginWnd::SetSignInBtnEnable(BOOL enable)
{
	btnSignIn->SetEnabled(enable);
}

void LoginWnd::SetSignUpBtnEnable(BOOL enable)
{
	btnSignUp->SetEnabled(enable);
}

void LoginWnd::SignInBtnSuccess()
{
	SetEvent(signInEvent);
	Close(2);
}

void LoginWnd::SignUpSuccess()
{
	SetEvent(signUpEvent);
	this->ResizeClient(400, 286);
	static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"))->SelectItem(0);
	this->CenterWindow();
	lbTitle->SetText(L"µ« ¬º");
	btnSetting->SetVisible();
	btnGoSignUp->SetVisible();
	btnSignIn->SetVisible();

	btnSignUp->SetVisible(false);
	btnBack->SetVisible(false);
	btnOK->SetVisible(false);
	ShowTip(L"◊¢≤·≥…π¶");
}
