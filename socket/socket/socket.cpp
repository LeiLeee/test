#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI chat(LPVOID p)
{
	SOCKET con_sock = *(SOCKET*)p;
	char data[200];
	memset(data,0,200);
	int a;
	while(1)
	{
		a=recv(con_sock,data,sizeof(data),0);
		if(a==0 || a==SOCKET_ERROR)
		{
			printf("thread exit\n");
			closesocket(con_sock);
			exit(0);
		}
		else
		{
			printf("%d\n",strlen(data));
			printf("accept information:%s\n",data);
			memset(data,0,200);
		}
	}
}

DWORD WINAPI send_sock(LPVOID p)
{
	SOCKET con_sock = *(SOCKET*)p;
	char s[200];
	memset(s,0,200);
	
	while(1)
	{
		fgets(s,sizeof(s),stdin);
		if(s[strlen(s)-1]=='\n')
		{
			s[strlen(s)-1]='\0';
		}
		send(con_sock,s,strlen(s),0);
	}
}


int main(int argc,char* argv[])
{
	//初始化window 套接字即（加载套接字库，下面是使用winsock2.2的版本库）
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsadata;

	if(0 != WSAStartup(sockVersion,&wsadata))
	{
		return 0 ;
	}
	//创建套接字  协议族，地址家族，决定者地址类型
	SOCKET listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(listen_sock == INVALID_SOCKET)
	{
		printf("listen_sock fail");
	}

	//bind套接字
	sockaddr_in addr_in;
	addr_in.sin_family=AF_INET;
	addr_in.sin_port = 8888;
	addr_in.sin_addr.S_un.S_addr = inet_addr("192.168.14.152");
	if(SOCKET_ERROR == bind(listen_sock,(sockaddr*)&addr_in,sizeof(addr_in)))
	{
		printf("bind fail\n");
	}
	//监听
	listen(listen_sock,4);

	//等待客户端的请求
	SOCKET com_sock;
	while(1)
	{
		DWORD newthreadid;
		sockaddr_in client_addr;
		int len = sizeof(client_addr);
		com_sock = accept(listen_sock,(sockaddr*)&client_addr,&len);
		if(com_sock == INVALID_SOCKET)
		{
			printf("com_sock fail");
		}
		else
		{
			printf("connect success\n");
		}
		printf("ip:%s\n  port:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		HANDLE handle = CreateThread(NULL,0,chat,(LPVOID)&com_sock,0,&newthreadid);
		if(handle == NULL)
		{
			printf("create thread fail");
			break;
		}
		HANDLE handle1 = CreateThread(NULL,0,send_sock,(LPVOID)&com_sock,0,NULL);
		if(handle1 == NULL)
		{
			printf("create thread fail");
			break;
		}

		CloseHandle(handle);
		CloseHandle(handle1);
	}
	
	closesocket(com_sock);
	closesocket(listen_sock);
	
	WSACleanup();

	system("pause");
	return 0;
}