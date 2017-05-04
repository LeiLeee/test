#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI chat(LPVOID p)
{
	SOCKET con_sock=*(SOCKET*)p;
	char s[200];
	memset(s,0,sizeof(s));
	int a;
	while(1)
	{
		//recvfrom也可以
		a=recv(con_sock,s,sizeof(s),0);
		printf("%d\n",a);
		if(0==a || SOCKET_ERROR==a)
		{
			printf("connect interrupt\n");
			closesocket(con_sock);
			exit(0);
		}
		else
		{
			printf("other: %s\n",s);
			memset(s,0,sizeof(s));
		}
	}
}

DWORD WINAPI chat1(LPVOID p)
{
	SOCKET con_sock=*(SOCKET*)p;
	printf("%d\n",con_sock);
	int len;
	char s[200];
	memset(s,0,sizeof(s));
	while(1)
	{
		printf("input:\n");
		fgets(s,200,stdin);
		len = strlen(s);
		printf("len1:%d\n",strlen(s));
		if(s[len-1]=='\n')
		{
			s[len-1]='\0';
		}
		printf("len2:%d\n",strlen(s));
		//sendto 也可以
		if(SOCKET_ERROR == send(con_sock,s,strlen(s),0))
		{
			printf("send fail\n");
		}
	}

}
int main()
{
	WORD word = MAKEWORD(2,2);
	WSADATA wsadata;
	if(WSAStartup(word,&wsadata)!= 0 )
	{
		return 0 ;
	}

	SOCKET client_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(client_sock == INVALID_SOCKET)
	{
		printf("client_sock fail\n");
	}
	printf("%d\n",client_sock);
	sockaddr_in client_addr;
	client_addr.sin_addr.S_un.S_addr = inet_addr("192.168.14.152");
	client_addr.sin_port = 8888;
	client_addr.sin_family = AF_INET;
/*
	if(SOCKET_ERROR == bind(client_sock,(sockaddr*)&client_addr,sizeof(client_addr)))
	{
		printf("bind fail\n");
	}
*/
	if(SOCKET_ERROR == connect(client_sock,(sockaddr*)&client_addr,sizeof(client_addr)))
	{
		printf("connect fail \n");
	}
	
	
/*	sockaddr_in addr_in;
	addr_in.sin_family=AF_INET;
	addr_in.sin_port = 8888;
	addr_in.sin_addr.S_un.S_addr = inet_addr("192.168.14.152");
*/
	HANDLE handle = CreateThread(NULL,0,chat,(LPVOID)&client_sock,0,NULL);
	if(handle == NULL)
	{
		printf("createthread fail\n");
	}
	HANDLE handle1 = CreateThread(NULL,0,chat1,(LPVOID)&client_sock,0,NULL);
	if(handle1 == NULL)
	{
		printf("createthread fail\n");
	}

	
	CloseHandle(handle);
	CloseHandle(handle1);
	while(1);
	
	WSACleanup();
	system("pause");
	return 0;
}