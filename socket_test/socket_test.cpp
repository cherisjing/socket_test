#include "WinSock2.h"  
#include "stdio.h"  
#pragma comment(lib,"Ws2_32.lib")//导入库,lib表示库，" "表示库的名称  

int main(int argc, CHAR* argv[])
{
    //加载WinSock库  
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

    SOCKET lSocket, aSocket;
    SOCKADDR_IN AddrSer;
    AddrSer.sin_family = AF_INET;
    AddrSer.sin_port = htons(5000);
    AddrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示接受任何机器的请求  
    lSocket = socket(AF_INET, SOCK_STREAM, 0);
    //绑定服务器地址  
    if (::bind(lSocket, (SOCKADDR*)&AddrSer, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        printf("绑定套接字失败");
        return 0;
    }
    ::listen(lSocket, 1);//开始监听客户端的连接请求  
    SOCKADDR_IN AddrCli;//客户端的地址信息  
    int len = sizeof(SOCKADDR);
    aSocket = ::accept(lSocket, (SOCKADDR*)&AddrCli, &len);//连接成功时，产生新的socket用来与客户端通信  
    char recBuf[1024];
    char senBuf[1024];
    while (1)
    {
        //TCP下为recv与send，UDP下为recefrom与sendto  
        ::recv(aSocket, recBuf, 1024, 0);
        if (recBuf[0] == '#')
        {
            printf("客户端请求断开\n");
            printf("正在断开...");
            Sleep(3000);
            break;
        }
        printf("客户端：%s\n", recBuf);
        printf("服务器:");
        gets_s(senBuf);//warming:gets()is unsafe  
        if (senBuf[0] == '#')
        {
            ::send(aSocket, senBuf, 1024, 0);
            printf("正在请求断开...\n");
            Sleep(3000);
            return 0;
        }
        //printf("服务器：%s\n",senBuf);  
        ::send(aSocket, senBuf, 1024, 0);
        memset(recBuf, ' ', 1024);
        memset(senBuf, ' ', 1024);
    }
    ::closesocket(aSocket);
    ::closesocket(lSocket);
    ::WSACleanup();
    return 0;
}