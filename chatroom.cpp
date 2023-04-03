#include <stdio.h>
#include <string>
using namespace std;

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

HANDLE hMutex; //互斥锁

std::string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar t* wszGBK = new wchar t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, streUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

hMutex = CreateMutex(0, 0, L"console");		//创建锁（放到初始化函数里）

void printMsg(const char* msg)
{
	//1.上锁(申请互斥锁）
	//INFINITE,没有申请到资源就一直等待，直到等到为止！
	waitForSingleObject(hMutex, INFINITE);

	static POINT pos = { 0,0 };
	gotoxy(pos.x, pos.y);
	printf("%s\n", msg);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOut, &info);
	pos.x = info.dwCursorPosition.X;
	pos.y = info.dwCursorPosition.Y;




	//2.释放锁
	ReleaseMutex(hMutex);
}



DWORD WINAPI threadFuncRecv(LPVOID pram)
{	
	chr buff[4096];
	while(1)
	{
		int ret = recv(serverSocket, buff, sizeof(buff), 0);
		if (ret <= 0) 
		{
			printf("服务器关闭或故障！\n");
			break;
		}

		//打印接受到的信息
		printMsg(UTF8ToGBK(buff).c_str);
	}
	return NULL;
}
void main()
{
	HANDLE hThread = CreateThread(0, 0, threadFuncRecv, 0, 0, 0);
	closeHandle(hThread);
}