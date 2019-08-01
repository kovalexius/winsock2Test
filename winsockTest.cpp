

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>

#include <WinSock2.h>




int main()
{
    //char host_adr[64] = "www.wikipedia.org";
	char host_adr[64] = "www.youtube.com";

    // 1. Инициализация
    WSADATA wsaData;
    WORD version = MAKEWORD(2,2);
    int res = WSAStartup(version, &wsaData);
	if (res == 0)
		std::cout << "Initialization true ... " << std::endl;
	else
		std::cout << "Initialization failed: " << WSAGetLastError() << std::endl;

    // 2. Создание сокета
    SOCKET client;
    client = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET != client)
		std::cout << "Creature socket true ... " << std::endl;

	u_long iMode = 0; // 0 - blocking, not 0 - nonblocking
	auto iResult = ioctlsocket(client, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);
	

    hostent *ent = gethostbyname(host_adr);
    if (!ent)
    {
		std::cout << "Cannot get the host by name!";
        return -1;
    }
    
    struct in_addr addr;
    int addrIdx = 0;
    while (ent->h_addr_list[addrIdx] != 0) {
        addr.s_addr = *(u_long *) ent->h_addr_list[addrIdx++];
		std::cout << inet_ntoa(addr) << std::endl;
    }

    // 3. Устанавливаем соединение
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.S_un.S_addr = *(u_long *) gethostbyname(host_adr)->h_addr_list[0];
    
	//service.sin_addr.S_un.S_addr = inet_addr("172.18.4.79");
    std::cout << "ip: " << inet_ntoa(service.sin_addr) << std::endl;
    
    service.sin_port = htons(80);
	//service.sin_port = htons(8080);

	int conn = connect(client, (struct sockaddr*)&service, sizeof(service));
	if (conn == SOCKET_ERROR)
	{
		std::cout << "Connection false ... " << conn << " error: " << WSAGetLastError() << std::endl;
		return -1;
	}
	else
		std::cout << "Connection true ... " << std::endl;


    // 4. Посылаем данные
    //char msg[1024] = "GET /registry HTTP/1.0\r\nUser-Agent: CPPTest\r\nConnection: close\r\n\r\n";
	char msg[1024] = "GET / HTTP/1.0\r\nUser-Agent: CPPTest\r\nConnection: close\r\n\r\n";
    int put_send = send(client, msg, strlen(msg), 0);
    if(SOCKET_ERROR == put_send)
		std::cout << "Error send message ... " << std::endl;
    else 
		std::cout << "Bytes Sent: " << put_send << std::endl;


    // 5. Принимаем данные 
    char RecvBuffer[4096];
   

	//Sleep(500);
	int put_recv;
	bool isReceived = false;
    do {
        put_recv = recv(client, RecvBuffer, sizeof(RecvBuffer), 0);
		if (put_recv > 0)
		{
			printf("Bytes received: %d\n", put_recv);
			isReceived = true;
		}
		else if (put_recv == 0)
		{
			if (isReceived)
			{
				printf("Connection closed\n");
				break;
			}
		}
        else 
			printf("recv failed: %d\n", WSAGetLastError());
    }   while (put_recv >= 0);

	closesocket(client);

	std::cout << RecvBuffer << std::endl;

   
    WSACleanup();

	int k;
	std::cin >> k;
    return 1;
}





