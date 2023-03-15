#include "TcpClientNetworking.h"
#include <iostream>

TcpConnection::TcpConnection(std::string serverIp, int port, MessageReceivedHandler handler)
	: s_IP(serverIp), s_port(port), msg_handler(handler)
{

}

TcpConnection::~TcpConnection()
{
	Cleanup();
}

//Initialize Winsock
bool TcpConnection::Init()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsError = WSAStartup(ver, &wsData);
	if (wsError != 0)
	{
		std::cerr << "Can't initialize Winsock. Error #" << wsError << std::endl;
		return false;
	}

	if (!CreateSocket())
	{
		return false;
	}
	if (!Connect())
	{
		return false;
	}
	
	return true;
}

//Cleanup
void TcpConnection::Cleanup()
{
	WSACleanup();
}

//Send Message to server
void TcpConnection::Send(std::string msg)
{
	int sendResult = send(sock, msg.c_str(), msg.size() + 1, 0);
	if (sendResult == SOCKET_ERROR)
	{
		std::cout << "Couldn't send. Err #" << WSAGetLastError() << std::endl;
	}
}

//Wait for server response
void TcpConnection::waitForResponse(std::string* response)
{
	ZeroMemory(recBuf, RecBufferSize);
	int bytesReceived = recv(sock, recBuf, RecBufferSize, 0);
	if (bytesReceived < 0)
	{
		std::cout << "Error recieving from server" << std::endl;
	}
	else if (bytesReceived == 0)
	{
		std::cout << "Disconnected." << std::endl;
	}

	*response = std::string(recBuf);
}

//Create a Socket
bool TcpConnection::CreateSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket. Error #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	return true;
}

//Connect to server
bool TcpConnection::Connect()
{
	//Fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(s_port);
	int sockOK = inet_pton(AF_INET, s_IP.c_str(), &hint.sin_addr);
	if (sockOK == 0)
	{
		std::cerr << "Invalid IP" << std::endl;
		return false;
	}
	if (sockOK < 0)
	{
		std::cerr << "Can't create sockaddr. Error #" << WSAGetLastError() << std::endl;
	}

	//Connect to the server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server. Error #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return false;
	}
	return true;
}
