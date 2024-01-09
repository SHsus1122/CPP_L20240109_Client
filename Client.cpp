#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	/////////////////////////////////// [ 1. ���� ���� �� ���� �ܰ� ]
	// ������ �������� ����ϴ� ������(.dll) �ε��ϴ� �ڵ�, ������ ���� �ڵ带 �����ؾ��մϴ�.
	// �ʱ�ȭ - 2.2 ���� ��� �� ��������
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// MS ���Ĺ����� ������ �ε� ������ 0 �� �ƴϸ� �����̵� ���� �ε� �����Դϴ�.
	// exit(-1); : ���� �߻��� �����Ű�� ���� �Լ�
	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	// ���� �ּ� ������ ��� ����ü ����
	// ����ü �ʱⰪ�� 0 ���� �ʱ�ȭ �մϴ�.
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	// IPv4 ����� �ּ� ü�� ����Ѵٴ� ������ �մϴ�.
	// ������ ���� ȣ��Ʈ�� IP �ּҸ� �Է��մϴ�.
	// ������ ������ ��Ʈ ��ȣ�� �����մϴ�.
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(30211);


	// ���� ����
	//	- PF_INET	  : TCP, UDP ���������� ����ϰڴٴ� �ǹ� (�߰��� IPv6 �� AF_INET6)
	//	- SOCK_STREAM : ���� ������ ������ ����(��� ����)
	//	- IPPROTO_TCP : TCP ���ؿ��� ���� ������ ���� �ɼ� ����(��� �ɼ� ����) 
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	/////////////////////////////////// [ 2. ���� ���� ]
	// connect : Ŭ���̾�Ʈ�� ������ ���� ������ �õ��� �� ����մϴ�.
	// Ŭ���̾�Ʈ�� ����, �����Ϸ��� ������ �ּ� ����, ���� �ּ��� ũ��(����)
	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}


	/////////////////////////////////// [ 3. �����κ��� ���� ]
	// �����κ��� �����͸� �ޱ� ���ؼ� �� �迭�� �������ݴϴ�.
	// recv	: �������κ��� �����͸� �����ϴ� �Լ��Դϴ�.
	//	- ServerSocket		 : �����͸� ������ ������ Ŭ���̾�Ʈ�� ����� �����Դϴ�.
	//	- RecvBuffer		 : �����κ��� ������ �����͸� �����ϴ� �����Դϴ�.
	//	- sizeof(RecvBuffer) : ������ �������� �ִ� ũ���Դϴ�.
	//	- 0					 : flag, ���� �ɼ��� ��Ÿ���µ� 0 �� ��� �Ϲ������� �ƹ��� �ɼ� ���� �����͸� �����մϴ�.
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent : " << RecvBuffer << endl;

	// ������ �����͸� str Ÿ������ ��ȯ���ݴϴ�.
	// size_t �� ��ȣ ���� ������ ���ĸ��� ���� �� �ִ� Ư���� Ÿ���Դϴ�.
	// �� Ÿ���� �̿��ؼ� Data.find() ���� �Լ��� ����� �����մϴ�.
	// Data.find('+'); ��, �޾ƿ� ���ڿ� �ȿ� '+' ���ڰ� �ִ��� ã�Ƴ��� ����Դϴ�.
	// ���� ã�� ���Ѵٸ� string::npos ���� ��ȯ �Ǳ� ������ �̸� �ݺ��ؼ� ���ϴ� �����ڸ� ã�� �մϴ�.
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

	// ���ؿ� ���ڿ��κ��� �����ڸ� �������� ���� ���ڿ� ������ ���ڸ� ���մϴ�.
	string FirstNumberString = Data.substr(0, OperatorPosition);
	string SecondNumberString = Data.substr(OperatorPosition + 1,
		Data.length() - OperatorPosition - 1);

	// ���� ���ڿ��� ������ ��ȯ�ϱ� ���ؼ� stoi �Լ��� ����մϴ�.
	// ���� ���������� Data.find() �� if ���� �̿��ؼ� ������ �����մϴ�.
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


	/////////////////////////////////// [ 4. ������ ���� ]
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

	// ������ �����͸� ���� �� �ʿ��� �������Դϴ�.
	// ServerSocket : �����͸� ������ Ŭ���̾�Ʈ�� ������ ����� �����Դϴ�.
	// Buffer		: �����ϰ��� �ϴ� �����Ͱ� ����� ���� �Դϴ�.
	// (int)(strlen(Buffer) + 1) : ������ �������� ũ���Դϴ�.
	//	- char �迭�� �����̱� ������ ���� ������ ���� NULL ���� ���� '\0' �� ������ ���� �Դϴ�.
	// 0			: flag, ���� �ɼ��� ��Ÿ���µ� 0 �� ��� �Ϲ������� �ƹ��� �ɼ� ���� �����͸� �����մϴ�.
	int SentByte = send(ServerSocket, Buffer, (int)(strlen(Buffer) + 1), 0);
	cout << "Send Data" << endl;


	/////////////////////////////////// [ 5. ���� ���� ]
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}