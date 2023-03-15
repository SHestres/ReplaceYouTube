#pragma once

#include <string>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#define RecBufferSize 4096

//Forward declartion
class TcpConnection;

typedef void(*MessageReceivedHandler)(TcpConnection ServerConnection, std::string msg);

class TcpConnection
{
public:
	TcpConnection(std::string serverIp, int port, MessageReceivedHandler handler);

	~TcpConnection();

	//Initialize Winsock
	bool Init();

	//Cleanup
	void Cleanup();

	//Send Message to server
	void Send(std::string msg);

	//Wait for server response
	void waitForResponse(std::string* response);

private:
	std::string s_IP;
	int s_port;
	MessageReceivedHandler msg_handler;
	SOCKET sock;
	char recBuf[RecBufferSize];
	//Create a Socket
	bool CreateSocket();

	//Connect to server
	bool Connect();

};