#include <stdio.h>
#include <string>
using namespace std;

#include "chatroom.h"
#include "massage.h"
#include "network.h"
#include "ui.h"

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#define SERVER_IP ""   //服务器ip地址
#define CHAT_PORT 666  //端口

SOCKET serverSocket;   //网络套接字
sockaddr_in sockAddr;  //网络地址

HANDLE hMutex;         //互斥锁

//配置聊天室网络地址

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

	//7.编辑信息功能

	//8.编辑删除功能

	return 0;
}