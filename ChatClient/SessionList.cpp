#include "SessionList.h"


SessionList::SessionList(CPaintManagerUI & paintManager) :
	paintManager(paintManager)
{
}

SessionList::~SessionList()
{
}

bool SessionList::Add(CControlUI * control)
{
	if (control == NULL)
		return false;

	if (_tcsicmp(control->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) != 0)
		return false;

	return CListUI::Add(control);
}

bool SessionList::AddAt(CControlUI * control, int index)
{
	if (control == NULL)
		return false;

	if (_tcsicmp(control->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) != 0)
		return false;

	return CListUI::AddAt(control, index);
}

bool SessionList::Remove(CControlUI * control, bool destroy)
{
	if (control == NULL)
		return false;

	if (_tcsicmp(control->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) != 0)
		return false;

	return CListUI::Remove(control, destroy);
}

bool SessionList::RemoveAt(int index, bool destroy)
{
	CControlUI *control = GetItemAt(index);
	if (control == NULL)
		return false;

	if (_tcsicmp(control->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) != 0)
		return false;

	return CListUI::RemoveAt(index, destroy);
}

//CListContainerElementUI *SessionList::AddItem(SessionItemInfo *item)
//{
//	CListContainerElementUI *listElement = NULL;
//	if (!dlgBuilder.GetMarkup()->IsValid())
//		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("SessionItem.xml"), (UINT)0, NULL, &paintManager));
//	else
//		listElement = static_cast<CListContainerElementUI*>(dlgBuilder.Create((UINT)0, &paintManager));
//
//	if (listElement == NULL)
//		return false;
//
//	CButtonUI *headerImg = static_cast<CButtonUI*>(paintManager.FindSubControlByName(listElement, L"HeaderImg"));
//	if (headerImg)
//	{
//		headerImg->SetBkImage(item->headerImg); 
//		if (item->newMsg)
//		{
//			headerImg->SetAttributeList(_T("normalimage=\"file='UnReadDot.png' dest='28,-4,40,8'\""));
//		}
//	}
//
//	CLabelUI *nickName = static_cast<CButtonUI*>(paintManager.FindSubControlByName(listElement, L"NickName"));
//	if (nickName)
//	{
//		nickName->SetText(item->nickName);
//	}
//
//	CLabelUI *message = static_cast<CButtonUI*>(paintManager.FindSubControlByName(listElement, L"Message"));
//	if (message)
//	{
//		message->SetText(item->message);
//	}
//
//	CLabelUI *msgTime = static_cast<CButtonUI*>(paintManager.FindSubControlByName(listElement, L"MsgTime"));
//	if (msgTime)
//	{
//		msgTime->SetText(item->msgTime);
//	}
//
//	if (item->mute)
//	{
//		CButtonUI *muteImg = static_cast<CButtonUI*>(paintManager.FindSubControlByName(listElement, L"MuteImg"));
//		if (muteImg)
//		{
//			muteImg->SetNormalImage(L"ChatList_Mute_Icon.png");
//			muteImg->SetVisible();
//		}
//	}
//	listElement->SetTag(1);
//	return listElement;
//}
//
//bool SessionList::RemoveItem(SessionItemInfo * item)
//{
//	return false;
//}
