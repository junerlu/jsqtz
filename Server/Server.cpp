#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <map>
#include "Server.h"

using std::map;
using std::cout;
using std::endl;
using std::pair;

map<CTcpOfClient*, string> m_clients; //存储客户端套接字和名称的映射关系

DWORD  WINAPI RecvMSG(LPVOID param); //处理单个客户端的线程

int main() {
	//初始化网络环境
	if (!CTcpListen::InitNet()) {
		cout << "初始化网路环境失败！";
		return -1;
	}

	CTcpListen sockServer; //接收客户端连接的服务器套接字

	//创建套接字
	if (!sockServer.Create()) {
		cout << "创建服务器监听socket失败！";
		return -1;
	}

	//绑定网络信息
	if (!sockServer.Bind(9999)) {
		cout << "绑定网络信息失败！";
		return -1;
	}

	//监听
	if (!sockServer.Listen()) {
		cout << "监听失败！";
		return -1;
	}


	cout << "服务器启动成功！" << endl;

	while (true) {
		CTcpOfClient* cli = new CTcpOfClient; //由于处于while循环内部，需要用new在堆中申请空间，否则下一个循环该变量就会被销毁（生命周期）

		//等待客户端的连接
		if (!sockServer.Accept(*cli)) {
			cout << cli->GetIp() << ":连接失败！" << endl;
			continue;
		}

		//成功连接后，还需要客户端发送自己的名称
		char msg[20]{};
		int len = cli->Recv(msg, 20); //首先接收这位客户端的名称
		if (len <= 0) {
			cout << cli->GetIp() << ":接收数据失败！已经断开" << endl;
			delete cli;
			continue;
		}

		m_clients.insert(pair<CTcpOfClient*, string>(cli, msg)); //将该

		cout << msg << ":进入聊天室！" << endl;

		//为新客户端单独开启一个线程用于接收处理该客户端的信息
		CreateThread(0, 0, RecvMSG, cli, 0, 0);
	}

	//清理网络环境
	CTcpListen::ClearNet();

}


DWORD  WINAPI RecvMSG(LPVOID param)
{
	CTcpOfClient* cli = (CTcpOfClient*)param; //得到传入的

	//通知以前的所有客户端，有新人加入
	for (auto& i : m_clients) {
		if (i.first == cli) continue; //新人本人，就不用发送了
		string tm = "1:"; //命令序号，1代表新人加入聊天室
		tm += (m_clients[cli] + ":加入聊天室");
		i.first->Send(tm.data(), tm.size()); //给在群聊中的每一个人都发送新成员加入的消息
	}


	//向新客户端发送以前的用户
	string tn = "4:";  //命令序号，4代表向新人发送当前在线人员
	for (auto& i : m_clients) { //遍历所有用户，拼接用户名称
		if (i.first == cli) continue;
		tn += (i.second + ":");
	}

	cli->Send(tn.data(), tn.size()); //向新用户发送所有在线用户


	char msg[0xFF]{}; //接收消息的缓存区
	while (1) {
		int len = cli->Recv(msg, 0xFF);
		//正常接收，转发消息
		if (len > 0) {
			for (auto& i : m_clients) {
				if (i.first == cli) continue;
				string tm = "3:" + m_clients[cli] + ':'; //命令序号，3代表正常接收消息，像其它客户端转发信息
				tm += msg;
				i.first->Send(tm.data(), tm.size());
			}
			continue;
		}

		//否则就是，客户端断连，通知其它在线用户
		for (auto& i : m_clients) {
			if (i.first == cli) continue;
			string exitMsg = "2:"; //命令序号，2代表有用户断开连接
			exitMsg += (m_clients[cli] + ":退出聊天室");
			i.first->Send(exitMsg.data(), exitMsg.size());
		}

		cout << m_clients[cli] << ":退出聊天室" << endl; //服务器显示推出的用户名称
		m_clients.erase(cli); //在映射图中删除该用户

		delete cli; //清除内存占用
		break; //退出处理该用户消息的循环，退出该线程
	}

	return 0;
}

