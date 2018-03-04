#pragma once
#include "stdafx.h"
#include "Common.h"

class Task
{
public:
	static int Connect(WPARAM wParam, LPARAM lParam = NULL);
	static int SignInWait(WPARAM wParam, LPARAM lParam = NULL);
	static int SignUpWait(WPARAM wParam, LPARAM lParam = NULL);
	static int ProcessRecvDate(WPARAM wParam, LPARAM);

	static int SearchWait(WPARAM wParam, LPARAM lParam = NULL);

	static int ChatListEndDown(WPARAM wParam, LPARAM lParam = NULL);


	static void Callback(int result);


};

