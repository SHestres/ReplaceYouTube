// RYTServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Following tutorial at https://www.youtube.com/watch?v=WDn-htpBlnU


#include <iostream>
#include <ws2tcpip.h> //Includes what's needed to interface with windows sockets

#pragma comment (lib, "ws2_32.lib")




void main()
{
	//Initialize winsock
	WSADATA wsData; //Struct that holds config data
	WORD ver = MAKEWORD(2, 2); //Use version 2.2

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		std::cerr << "Can't initialize Winsock. Exiting" << std::endl;
		return;
	}

	//Create a socket
	/*
		A socket is often just a number that get's mapped in the OS, but on windows systems
		it's given an actual type
	*/
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); //(Address family, Socket type (stream for tcp), protocol flag)
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket. Exiting" << std::endl;
		return;
	}

	//Bind an ip address and port to the socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton ....

	bind(listening, (sockaddr*)&hint, sizeof(hint)); 

	//Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN); //Marks socket for listening, doesn't actually accept a connection

	//Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Client socket invalid." << std::endl;
	}



	//Close listening socket

	//While loop: Accept and echo message back to client



	//Close the socket

	//Shut down winsock


}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
