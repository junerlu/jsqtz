#pragma once
#include<WinSock2.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using std::string;

//封装收发数据客户端套接字
class CTcpOfClient
{
private:
	string C_ip;   //客户端ip
	SOCKET C_sock; //客户通信套接字

public:
	CTcpOfClient() 
	{
		C_sock = INVALID_SOCKET;
	}
	~CTcpOfClient()
	{
		if (C_sock != INVALID_SOCKET) 
		{
			closesocket(C_sock);
		}
	}
	
//设置连接的用户信息
void SetSocket(string ip, SOCKET sock) 
{
	C_sock = sock; //客户端的ip地址
	C_ip = ip;     //与客户端通信的套接字
}

//获取该用户的ip地址
string GetIp() 
{
	return C_ip;
}


//向客户端发送信息
//msg 信息缓存区
//len 信息长度
int Send(const char* msg, int len) 
{
	return send(C_sock, msg, len, 0);//返回发送成功的字节数
}

//接收该客户端的信息
//buf 接收信息的缓存区
//size 该缓存区的大小
int Recv(char* buf, int size) 
{
	return recv(C_sock, buf, size, 0);//返回接收到的字节数
}

};

class CTcpListen 
{
private:
	SOCKET S_sock;
	CTcpListen() 
	{
		S_sock = INVALID_SOCKET;
	}
	~CTcpListen()
	{
		if (S_sock != INVALID_SOCKET) 
		{
			closesocket(S_sock);
		}
	}

public:
	
	// 创建socket
	bool Create() 
	{
		S_sock = socket(AF_INET, SOCK_STREAM, 0);
		return S_sock != INVALID_SOCKET; // 成功返回true，失败返回false
	}
	
	// 绑定本机网络
	// port 端口
	// ip ip地址
	bool Bind(int port, string ip = "0.0.0.0") 
	{
		SOCKADDR_IN addrSev;
		addrSev.sin_family = AF_INET;
		addrSev.sin_port = htons(port);
		addrSev.sin_addr.S_un.S_addr = inet_addr(ip.data());
		return -1 != bind(S_sock, (sockaddr*)&addrSev, sizeof(addrSev)); // 成功返回true， 失败返回flase
	}

	//监听
	bool Listen() 
	{
		return -1 != listen(S_sock, 5); // 成功返回true， 失败返回flase
	}

	// 接受客户端连接
	// cli 返回连接上的客户端
	bool Accept(CTcpOfClient& cli) 
	{
		SOCKADDR_IN addrCli;
		int len = sizeof(addrCli);
		SOCKET sockCli = accept(S_sock, (sockaddr*)&addrCli, &len);
		string ip = inet_ntoa(addrCli.sin_addr); //将数字ip值转化为字符串ip
		cli.SetSocket(ip, sockCli); //设置客户端的信息

		return sockCli != INVALID_SOCKET; // 成功返回true， 失败返回flase
	}

public:

	//初始化网络环境
	static bool InitNet() 
	{
		WSADATA wsadata;
		return 0 == WSAStartup(MAKEWORD(2, 2), &wsadata);//成功返回true，否则返回false
	}
	
	//清理网络环境
	static void ClearNet() 
	{
		WSACleanup();
	}



};





