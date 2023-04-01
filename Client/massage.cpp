#include <stdio.h>
#include <string>
using namespace std;

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
//还应该在void printMsg(const char* msg)最后一行加上：gotoxy(1, 34);
//释放锁
ReleaseMutex(hMutex);

void editPrint(int col, char ch)
{
	WaitForSingleObject(hMutex, INFINITE);
	gotoxy(col, 34);
	print("%c", ch);

	ReleaseMutex(hMutex);
	
}
void editPrint(int col, const char* str)
{
	WaitForSingleObject(hMutex, INFINITE);
	gotoxy(col, 34);
	print("%s", str);

	ReleaseMutex(hMutex);

}

void editdelet()
{
	//编辑删除
}
