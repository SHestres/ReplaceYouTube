#include "CTcpListener.h"
#include <iostream>

#define MAX_BUFFER_SIZE (49152)


CTcpListener::CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), HandleMessage(handler)
{
	m_port = port;
}

CTcpListener::~CTcpListener()
{
	Cleanup();
}

void CTcpListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

//Initialize Winsock
bool CTcpListener::Init()
{
	WSADATA wsData; //Struct that holds config data
	WORD ver = MAKEWORD(2, 2); //Use version 2.2ish

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		std::cerr << "Can't initialize Winsock. Exiting" << std::endl;
		return false;
	}
	return true;
}

//The main process
void CTcpListener::Run()
{
	char* buf;
	buf = (char*) malloc(MAX_BUFFER_SIZE);

	if (buf == NULL)
	{
		return;
	}

	while (true)
	{
		// Create a listening socket
		SOCKET listening = CreateSocket();
		if (listening == INVALID_SOCKET)
		{
			break;
		}
		//Wait for a connection
		SOCKET client = WaitForConnection(listening);
		if (client != INVALID_SOCKET)
		{
			//Close socket to only connect to one client at a time
			closesocket(listening);
			

			
			//Loop receive / send
			int bytesReceived = 0;
			do
			{
				ZeroMemory(buf, MAX_BUFFER_SIZE);

				bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);

				if (HandleMessage != NULL)
				{
					HandleMessage(this, client, std::string(buf, 0, bytesReceived)); //Something about c++ strings best practice that idk yet
				}
			} while (bytesReceived > 0); //Disconnected client => bytesRecieved == 0

			closesocket(client);
		}

		
	}
}

//Cleanup
void CTcpListener::Cleanup()
{
	WSACleanup();
}

//Create a socket
SOCKET CTcpListener::CreateSocket()
{
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); //(Address family, Socket type (stream for tcp), protocol flag)
	if (listening != INVALID_SOCKET)
	{
		//Bind an ip address and port to the socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		if (m_ipAddress != "")
		{
			inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);
		}
		else
		{
			hint.sin_addr.S_un.S_addr = INADDR_ANY;
		}

		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != SOCKET_ERROR)
		{
			int listenOk = listen(listening, SOMAXCONN);
			if (listenOk == SOCKET_ERROR) {
				std::cerr << "Listening gave winsock error " << WSAGetLastError() << std::endl;
				return -1;
			}
		}
		else
		{
			std::cerr << "Binding the socket gave winsock error " << WSAGetLastError() << std::endl;
			return -1;
		}
	}

	return listening; //listening will equal -1 if creating the socket failed
}

//Wait for a connection
SOCKET CTcpListener::WaitForConnection(SOCKET listening) 
{
	//Get Connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSock = accept(listening, (sockaddr*) &client, &clientSize); //Will stall while waiting for a client
	
	//Print Info
	char host[NI_MAXHOST];		//Client's remote name
	char service[NI_MAXSERV];	//Service (i.e. port) the client is connected to

	ZeroMemory(host, NI_MAXHOST);	//same as memset(host, 0, NI_MAXHOST); Clears the memory at &host
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((SOCKADDR*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) //If you can't get the client name
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); //Converts the ip address into a string, stores in string buffer "host"
		std::cout << host << " connected on port " <<
			ntohs(client.sin_port) << std::endl;
	}
	
	return clientSock;
}