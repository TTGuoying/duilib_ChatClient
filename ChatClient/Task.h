#pragma once
#include "stdafx.h"
#include "Common.h"

class Task
{
public:
	static int Connect(PVOID p);
	static int SignInWait(PVOID p);
	static int SignUpWait(PVOID p);
	static int ProcessRecvDate(PVOID p);

	static void Callback(int result);


};

