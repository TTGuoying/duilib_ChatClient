#pragma once

//通信协议值
enum EM_PROTOCOL
{
	INVALID_PROTOCOL,			// 无效值

	//登陆							 
	SIGNIN,                     // 登陆
	SIGNIN_SECCUSS,             // 登陆成功
	SIGNIN_FAILED,              // 登陆失败
	SIGNIN_ALREADY,             // 登陆失败
	SignUp,						// 注册
	SignUp_SECCUSS,             // 注册成功
	SignUp_FAILED,              // 注册失败

	// 好友
	GET_FRIENDS,				// 获取好友
	SEARCH_FRIENDS,				// 查找好友
	FRIENDS_REQUEST,			// 添加好友请求
};