#include <stdio.h>
#include <string>
#include<conio.h>
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

	
	
	//阿自 主程序里面以下这个bool isHZ函数：

	bool isHz(char str[], int index) {
		int i = 0;
		while (i < index) {
			if (str[i] > 0) {
				i++;
			}
			else {
				i += 2;
			}
		}
	}
	
	if (i == index) {
		return false;
	}
	else {
		return true;
	}
	
	//阿自 在Main里面的massage部分-编辑消息
	while (1) {
		char buff[1024]//保存用户输入的字符串
			memset(buff, 0, sizeof(buff));

		editPrint(0, '>');

		int len = 0;
		while (1) {
			if (_kbhit()) {
				char c = getch();
				if (c == '/r') { //按下了回车按键
					break;
				}
				else if (c == 8) {
					if (len == 0) {
						continue;
					}
					if (isHz(buff, len - 1)) {
						editPrint(len + 1, "\b\b  \b\b");
						buff[len - 1] = 0;
						buff[len - 2] = 0;
						len -= 2;
					}
					else {
						editPrint(len + 1, "\b \b");
						buff[len - 1] = 0;
						len -= 1;
					}
					continue;
				}
				WaitForSingleObject(hMutex, INFINITE);

				do {
					printf("%c", c);
					buff[len++] = c;


				} while (_kbhit() && (c = getch()));
				ReleaseMutex(hMutex);

			}
		}
		if (len == 0) {
			continue;

		}
		//清除编辑区的信息
		char buff2[1024];
		sprintf_s(buff2, sizeof(buff2), "%s\n", line2);
		editPrint(0, buff2);
		//把用户自己说的话输出
		sprintf_s(buff2, sizeof(buff2), "【LocalHost@%S】%s"，nickName, buff);
		printMag(buff2);
		//发送编辑好的字符串
		send(serverSocket, buff, strlen(buff) + 1, 0);
	}

	
	//5.创建一个新线程

	//6.创建互斥锁

	//7.编辑信息功能

	//8.编辑删除功能

	return 0;
}
