#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <string>

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


	/////////////////////////////////// [ �����κ��� ���� ]
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent : " << RecvBuffer << endl;

	string Data = RecvBuffer;
	size_t OperatorPosition = Data.find('+');
	if (OperatorPosition == string::npos)
	{
		OperatorPosition = Data.find('-');
		if (OperatorPosition == string::npos)
		{
			OperatorPosition = Data.find('*');
			if (OperatorPosition == string::npos)
			{
				OperatorPosition = Data.find('/');
				if (OperatorPosition == string::npos)
				{
					OperatorPosition = Data.find('%');
				}
			}
		}
	}

	string FirstNumberString = Data.substr(0, OperatorPosition);
	string SecondNumberString = Data.substr(OperatorPosition + 1,
		Data.length() - OperatorPosition - 1);

	int FirstNumber = stoi(FirstNumberString);
	int SecondNumber = stoi(SecondNumberString);
	int ResultNumber = 0;
	OperatorPosition = Data.find('+');
	if (OperatorPosition == string::npos)
	{
		OperatorPosition = Data.find('-');
		if (OperatorPosition == string::npos)
		{
			OperatorPosition = Data.find('*');
			if (OperatorPosition == string::npos)
			{
				OperatorPosition = Data.find('/');
				if (OperatorPosition == string::npos)
				{
					OperatorPosition = Data.find('%');
					ResultNumber = FirstNumber % SecondNumber;
				}
				else
				{
					ResultNumber = FirstNumber / SecondNumber;
				}
			}
			else
			{
				ResultNumber = FirstNumber * SecondNumber;
			}
		}
		else
		{
			ResultNumber = FirstNumber - SecondNumber;
		}
	}
	else
	{
		ResultNumber = FirstNumber + SecondNumber;
	}

	char Buffer[1024] = { 0, };
	//sprintf_s(Buffer, "%s, %s", FirstNumberString.c_str(), SecondNumberString.c_str());
	sprintf_s(Buffer, 1024, "%d", ResultNumber);


	/////////////////////////////////// [ ������ ���� ]
	/* ���� �ϳ� ���� �� �Ѱܿ� ���ڿ��� 1+4 ���� ������ ���
	* char FirstNumber = RecvBuffer[0] - 48;	// 0, 1, 2
	* char SecondNumber = RecvBuffer[2] - 48;
	* char ResultNum = FirstNumber + SecondNumber;
	* 
	* ResultNum += 48;							// ��� : 3
	* char Buffer[1024] = { 0, };
	* Buffer[0] = ResultNum;
	* Buffer[1] = '\0';
	*/
	//int Sum = atoi(RecvBuffer) * 5;
	//strcat(LastBuffer, itoa(Sum, LastBuffer, 10));

	int SentByte = send(ServerSocket, Buffer, (int)(strlen(Buffer) + 1), 0);
	cout << "Send Data" << endl;


	/////////////////////////////////// [ ���� ���� ]
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}