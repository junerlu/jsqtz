#pragma once
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#ifndef _CHATROOM_H_
#define _CHATROOM_H_

DWORD WINAPI threadFuncRecy();

void printMsg(const char* msg);
#endif