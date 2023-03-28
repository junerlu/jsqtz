#include <stdio.h>

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

//配置聊天室网络地址

bool init()
{
	//1.初始化网络服务
	
	//2.网络套接字

	//3.物理地址
}

void login()
{
	//登录界面
}

void uiInit()
{
	//初始化聊天界面
}

int main(void)
{
	//1.初始化网络
	init();
    
	//2.连接聊天室服务器
	int ret;

	//3.登录聊天室
	login();

	//4.初始化聊天界面
	uiInit();

	//5.创建一个新线程

	//6.创建互斥锁

	return 0;
}