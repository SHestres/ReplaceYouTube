// Following tutorial at https://www.youtube.com/watch?v=WDn-htpBlnU


#include <iostream>

#include"CTcpListener.h"
#include "Encoder.h"
#include "DashPackager.h"
#include "Window.h" //Has Video importer already

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

void main()
{
	//Files are relative to calling dir
	//Backslash needed to exec from different dir
	

	//system("python3 dashProgs\\mp4-dash.py ../success.mp4");

	/*

	DashPackager packager;
	bool resolutions[4] = { false, false, false, false };
	//packager.Init("C:\\Users\\sidne\\Downloads\\Gravity - 2K Trailer.mp4", "Gravity Trailer", resolutions);
	packager.Init("C:\\Users\\sidne\\Downloads\\Big_Buck_Bunny_1080_10s_30MB.mp4", "Bunny", resolutions);
	//packager.Init("../sample_960x540.m4v", "The Great Success Pls", resolutions);
	packager.Run();
	*/

	DashPackager packager;
	packager.Init("../sample_960x540.m4v", "C:\\Users\\sidne\\source\\repos\\SHestres\\ReplaceYouTube\\VideoFiles\\Packaged", "6969");
	vidStat videoStats = IDLE;
	auto f = packager.Run(&videoStats);

	bool isDone = false;
	while (!isDone) {
		switch (videoStats)
		{
		case (SUCCEEDED):
			std::cout << "Success!" << std::endl;
			isDone = true;
			break;
		case (BAD_ID):
			std::cout << "Bad ID" << std::endl;
			isDone = true;
			break;
		case (FAILED):
			std::cout << "Failed" << std::endl;
			isDone = true;
			break;
		}
	}

	std::cout << "Done!" << std::endl;

	//system()
	/*
	VideoLibrary library;

	library.createDemoData();
	*/

	/*
	library.StoreDatabase();
	library.LoadDatabase();
	*/
	
	/*
	Window win("dne.json");
	if(win.OpenWindow("Video Importer") != 0) return;
	win.Run();
	*/

	/*/CTcpListener server("", 54000, Listener_MessageReceived);

	if (server.Init())
	{
		server.Run();
	}
	*/

	/*
	Encoder encoder;

	int encode = encoder.Init("..\\success.mp4", "..\\output.av1");
	if(encode == 0)
	{
		encoder.SendEncodedData();
	}
	else if (encode == 2)
	{
		std::cout << "File not compressed" << std::endl;
	}
	*/
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg)
{
	listener->Send(client, msg);
}


//The remainder of this file is leftover from the bare-bones server


/*

#include <iostream>
#include <ws2tcpip.h>					//Header file for Winsock functions

#pragma comment (lib, "ws2_32.lib")		//Winsock Library file




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
	*//*
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

	//Close listening socket
	closesocket(listening); //Only closed in this case becuase we're currently only accepting one client

	//While loop: Accept and echo message back to client
	char buf[4096]; //Limited to 4096 for tutorial purposes, can and should be much larger

	while (true) //Example functionality of a server
	{
		ZeroMemory(buf, 4096);

		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0); //Copies bytes recieved into buf, and returned the number of bytes
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Exiting" << std::endl;
			break;
		}
		if(bytesReceived == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		

		//Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0); //Adding one to length sends the null ternimator because buf was zeroed.

	}

	//Close the socket
	closesocket(clientSocket);

	//Shut down / clean up winsock
	WSACleanup();

}*/


