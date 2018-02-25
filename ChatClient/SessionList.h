#pragma once
#include "stdafx.h"


class SessionList : public CListUI
{
public:
	SessionList(CPaintManagerUI &paintManager);
	~SessionList();

	bool Add(CControlUI *control);
	bool AddAt(CControlUI *control, int index);
	bool Remove(CControlUI *control, bool destroy = false);
	bool RemoveAt(int index, bool destroy = false);
	//CListContainerElementUI * AddItem(SessionItemInfo *item);
	//bool RemoveItem(SessionItemInfo *item);

private:
	CPaintManagerUI & paintManager;
	CDialogBuilder dlgBuilder;
};

