//Following tutorial at https://www.youtube.com/watch?v=y4_mFrTeD_A&list=PLZo2FfoMkJeEogzRXEJeTb3xpA2RAzwCZ&index=9
#pragma once

#include <string>

#include <ws2tcpip.h>					//Header file for Winsock functions

#pragma comment (lib, "ws2_32.lib")		//Winsock Library file

//Forward declaration (potentially uneeded)
class CTcpListener;

// Callback to data received
typedef void(*MessageRecievedHandler)(CTcpListener* listener, int socketId, std::string msg);


class CTcpListener
{

public:

	CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	
	~CTcpListener();

	void Send(int clientSocket, std::string msg);

	//Initialize Winsock
	bool Init();

	//The main process
	void Run();

	//Cleanup
	void Cleanup();

private:

	//Create a socket
	SOCKET CreateSocket();

	//Wait for a connection
	SOCKET WaitForConnection(SOCKET listening);

	std::string				m_ipAddress;
	int						m_port;
	MessageRecievedHandler	HandleMessage;

};