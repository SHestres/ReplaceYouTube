//Folowing tutorial at https://www.youtube.com/watch?v=0Zr_0Jy8mWE&list=PLZo2FfoMkJeEogzRXEJeTb3xpA2RAzwCZ&index=9


#include <iostream>
#include <string>
#include <ws2tcpip.h>					//Header file for Winsock functions

#pragma comment (lib, "ws2_32.lib")		//Winsock Library file

void main()
{
	std::string serverIP = "10.140.100.111";		//Server ip address
	int port = 54000;								//Listening port number on the server

	//Initialize Winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsError = WSAStartup(ver, &wsData);
	if (wsError != 0)
	{
		std::cerr << "Can't initialize Winsock. Error #" << wsError << std::endl;
		return;
	}

	//Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket. Error #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	//Fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

	//Connect to the server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server. Error #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	//While loop to send/recieve data
	char buf[4096];
	std::string userInput;
	do
	{
		//Prompt the user for some text
		std::cout << "> ";
		getline(std::cin, userInput);

		if (userInput.size() > 0)	//Make sure the user has typed something
		{
			//Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult == SOCKET_ERROR)
			{
				std::cout << "Couldn't send. Err #" << WSAGetLastError() << std::endl;
			} 


			//Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived < 0)
			{
				std::cout << "Received nothing from the server" << std::endl;
			}
			else if (bytesReceived == 0)
			{
				std::cout << "Disconnected." << std::endl;
			}

			//Echo response to console
			std::cout << "Server> " << std::string(buf, bytesReceived) << std::endl;
		}
	} while (userInput.size() > 0);

	//Gracefully close everything
	closesocket(sock);
	WSACleanup();

}