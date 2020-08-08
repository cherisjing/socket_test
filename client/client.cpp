#include "WinSock2.h"
#include "stdio.h"
#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable : 4996)

int main(int argc, CHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
		return 0;
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return 0;
	}

	SOCKET cSocket;
	cSocket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(5000);
	addrSer.sin_addr.S_un.S_addr = inet_addr("10.15.69.15");
	if (::connect(cSocket, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		printf("连接服务器失败");
		return 0;
	}
	char senBuf[1024];
	char recBuf[1024];
	while (1)
	{
		printf("客户端：");
		gets_s(senBuf);
		if (senBuf[0] == '#')
		{
			::send(cSocket, senBuf, 1024, 0);
			printf("正在请求断开...\n");
			Sleep(3000);
			return 0;
		}
		//printf("客户端：%s\n",senBuf);
		::send(cSocket, senBuf, 1024, 0);
		::recv(cSocket, recBuf, 1024, 0);
		if (recBuf[0] == '#')
		{
			printf("与服务器断开");
			printf("正在断开...");
			Sleep(3000);
			break;
		}
		printf("服务器：%s\n", recBuf);
		memset(recBuf, ' ', 1024);
		memset(senBuf, ' ', 1024);
	}
	::closesocket(cSocket);
	::WSACleanup();
	return 0;
}