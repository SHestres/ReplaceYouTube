//Folowing tutorial at https://www.youtube.com/watch?v=0Zr_0Jy8mWE&list=PLZo2FfoMkJeEogzRXEJeTb3xpA2RAzwCZ&index=9


#include <iostream>
#include <string>
#include <ws2tcpip.h>	//Header file for Winsock functions

#include "TcpClientNetworking.h"
#include "VideoPlayer.h"

#pragma comment (lib, "ws2_32.lib")		//Winsock Library file

//Forward Declaration
void DoNothing(TcpConnection ServerConnection, std::string msg);

void main()
{
	//VideoPlayer player("C:\\Users\\sidne\\source\\repos\\SHestres\\ReplaceYouTube\\success.mp4");
	VideoPlayer player("..\\sample_960x540.m4v");

	if (player.Init() == 0)
	{
		player.Run();
	}





	/* Server connection

	std::string serverIP = "10.140.100.111";		//Server ip address
	int port = 54000;								//Listening port number on the server

	TcpConnection server(serverIP, port, DoNothing);

	int attempts = 0;
	bool connected = false;
	do
	{
		connected = server.Init();
		attempts++;
	} while (!connected && attempts < 10);

	while (true)
	{
		std::cout << ">";

		std::string userInput;
		getline(std::cin, userInput);
		server.Send(userInput);

		std::string serverResponse;
		server.waitForResponse(&serverResponse);

		std::cout << "Server>" << serverResponse << std::endl;
	}

	*/
}

void DoNothing(TcpConnection ServerConnection, std::string msg)
{}
