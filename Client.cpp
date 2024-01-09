#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	/////////////////////////////////// [ 1. 소켓 생성 및 설정 단계 ]
	// 윈도우 전용으로 사용하는 소켓을(.dll) 로딩하는 코드, 리눅스 사용시 코드를 변경해야합니다.
	// 초기화 - 2.2 버전 사용 및 가져오기
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// MS 공식문서에 들어가보면 로드 성공시 0 이 아니면 무엇이든 간에 로드 실패입니다.
	// exit(-1); : 에러 발생시 종료시키기 위한 함수
	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	// 서버 주소 정보를 담는 구조체 선언
	// 구조체 초기값을 0 으로 초기화 합니다.
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	// IPv4 방식의 주소 체계 사용한다는 설정을 합니다.
	// 설정할 로컬 호스트의 IP 주소를 입력합니다.
	// 서버로 연결할 포트 번호를 설정합니다.
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(30211);


	// 소켓 생성
	//	- PF_INET	  : TCP, UDP 프로토콜을 사용하겠다는 의미 (추가로 IPv6 는 AF_INET6)
	//	- SOCK_STREAM : 연결 지향형 데이터 전송(통로 개설)
	//	- IPPROTO_TCP : TCP 수준에서 적용 가능한 소켓 옵션 선택(통로 옵션 설정) 
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	/////////////////////////////////// [ 2. 소켓 연결 ]
	// connect : 클라이언트가 서버에 소켓 연결을 시도할 때 사용합니다.
	// 클라이언트의 소켓, 연결하려는 서버의 주소 정보, 서버 주소의 크기(길이)
	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}


	/////////////////////////////////// [ 3. 서버로부터 받음 ]
	// 서버로부터 데이터를 받기 위해서 빈 배열을 생성해줍니다.
	// recv	: 소켓으로부터 데이터를 수신하는 함수입니다.
	//	- ServerSocket		 : 데이터를 수신할 서버가 클라이언트와 연결된 소켓입니다.
	//	- RecvBuffer		 : 서버로부터 수신한 데이터를 저장하는 버퍼입니다.
	//	- sizeof(RecvBuffer) : 수신할 데이터의 최대 크기입니다.
	//	- 0					 : flag, 여러 옵션을 나타내는데 0 인 경우 일반적으로 아무런 옵션 없이 데이터를 전송합니다.
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent : " << RecvBuffer << endl;

	// 수신한 데이터를 str 타입으로 변환해줍니다.
	// size_t 는 부호 없는 정수의 형식만을 담을 수 있는 특수한 타입입니다.
	// 이 타입을 이용해서 Data.find() 같은 함수의 사용이 가능합니다.
	// Data.find('+'); 즉, 받아온 문자열 안에 '+' 문자가 있는지 찾아내는 방식입니다.
	// 만약 찾지 못한다면 string::npos 으로 반환 되기 때문에 이를 반복해서 원하는 연산자를 찾게 합니다.
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

	// 구해온 문자열로부터 연산자를 기준으로 이전 숫자와 이후의 숫자를 구합니다.
	string FirstNumberString = Data.substr(0, OperatorPosition);
	string SecondNumberString = Data.substr(OperatorPosition + 1,
		Data.length() - OperatorPosition - 1);

	// 구한 문자열을 정수로 변환하기 위해서 stoi 함수를 사용합니다.
	// 이후 마찬가지로 Data.find() 와 if 문을 이용해서 연산을 진행합니다.
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


	/////////////////////////////////// [ 4. 서버로 보냄 ]
	/* 숫자 하나 한정 및 넘겨온 문자열이 1+4 같은 형태의 경우
	* char FirstNumber = RecvBuffer[0] - 48;	// 0, 1, 2
	* char SecondNumber = RecvBuffer[2] - 48;
	* char ResultNum = FirstNumber + SecondNumber;
	* 
	* ResultNum += 48;							// 결과 : 3
	* char Buffer[1024] = { 0, };
	* Buffer[0] = ResultNum;
	* Buffer[1] = '\0';
	*/
	//int Sum = atoi(RecvBuffer) * 5;
	//strcat(LastBuffer, itoa(Sum, LastBuffer, 10));

	// 서버로 데이터를 보낼 때 필요한 정보들입니다.
	// ServerSocket : 데이터를 전송할 클라이언트가 서버에 연결된 소켓입니다.
	// Buffer		: 전송하고자 하는 데이터가 저장된 버퍼 입니다.
	// (int)(strlen(Buffer) + 1) : 전송할 데이터의 크기입니다.
	//	- char 배열의 형태이기 때문에 에러 방지를 위해 NULL 종료 문자 '\0' 을 포함한 길이 입니다.
	// 0			: flag, 여러 옵션을 나타내는데 0 인 경우 일반적으로 아무런 옵션 없이 데이터를 전송합니다.
	int SentByte = send(ServerSocket, Buffer, (int)(strlen(Buffer) + 1), 0);
	cout << "Send Data" << endl;


	/////////////////////////////////// [ 5. 연결 종료 ]
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}