#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(30211);


	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}

	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent " << RecvBuffer << endl;

	int SentByte = send(ServerSocket, RecvBuffer, (int)strlen(RecvBuffer), 0);

	cout << "Send Data" << endl;

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}