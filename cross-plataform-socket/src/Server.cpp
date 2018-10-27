/*
 * Server.cpp
 *
 *  Created on: 26 ott 2018
 *      Author: Gotta
 */

#include "Server.h"

Server::Server(unsigned short port)
{
	// Save port number
	mPort = port;

	// Set configuration structure
#ifdef _WIN32
    
	ZeroMemory(&mSAddr, sizeof(mSAddr));
	mSAddr.ai_family = AF_INET;
	mSAddr.ai_socktype = SOCK_STREAM;
	mSAddr.ai_protocol = IPPROTO_TCP;
	mSAddr.ai_flags = AI_PASSIVE;
#else
    
	memset(&mSAddr, '\0', sizeof(mSAddr));
	mSAddr.sin_family = AF_INET;
	mSAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	mSAddr.sin_port = htons(port);
#endif
}

Server::~Server()
{
}

int Server::SockCreate()
{
#ifdef _WIN32

	int result                        {0};                                // Result value
	WSADATA wsaData;					                                  // Windows socket implementation information info

	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0)
	{
		std::cout << "WSAStartup failed with error: " << result << std::endl;
		return 1;
	}

	// Resolve the server address and port
	result = getaddrinfo(NULL, DEFAULT_PORT, &mSAddr, &mResult);
	if ( result != 0 )
	{
		std::cout << "getaddrinfo failed with error: " << result << std::endl;
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	mSockAddrServ = socket(mResult->ai_family, mResult->ai_socktype, mResult->ai_protocol);
	if (mSockAddrServ == int(INVALID_SOCKET))
	{
		std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(mResult);
		WSACleanup();
		return 1;
	}
#else

    // Create socket
    mSockAddrServ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check socket creation
    if (mSockAddrServ < 0)
    {
		std::cout << "Cannot create socket" << std::endl;
		return 1;
    }
#endif
    
	return mSockAddrServ;
}

int Server::SockBind()
{
	int result                        {0};                                // Result value

#ifdef _WIN32

	 // Setup the TCP listening socket
	result = bind(mSockAddrServ, mResult->ai_addr, (int)mResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(mResult);
        SockClose(mSockAddrServ);
		return 1;
	}

#else

	 // Bind socket to the address structure and verify it
	 result = bind(mSockAddrServ, (struct sockaddr*) &mSAddr, sizeof(mSAddr));
	 if (result < 0)
	 {
	   std::cout << "Cannot bind socket" << std::endl;
	   close(mSockAddrServ);
	   return 1;
	 }
#endif

	return result;
}

int Server::SockListen()
{
	int result			            {0};			                    // Result value

	// Wait for a client connection
    result = listen(mSockAddrServ, 5);
    if (result == -1 ) {
    	std::cout << "listen failed with error" << std::endl;
    	SockClose(mSockAddrServ);
       return 1;
    }

    // Set socket as non-blocking
#ifdef _WIN32
    
    u_long iMode    {0};
    
    if(ioctlsocket(mSockAddrServ, FIONBIO, &iMode) != NO_ERROR)
        std::cout << "Cannot set socket as non-blocking" << std::endl;
#else

    fcntl(mSockAddrServ, F_SETFL, O_NONBLOCK);
#endif
    
    return result;
}

int Server::SockAccept()
{
	int result					    {-1};							    // Result value
	socklen_t sockClientLenght		{sizeof(sockClientLenght)};		    // Socket client address lenght
    
    // Accept client connection
    mSockAddrClient = accept(mSockAddrServ, (sockaddr *) &mSockAddrClient, &sockClientLenght);

    // Check for client connected
	if(mSockAddrClient >= 0)
		result = 0;
    
	return result;
}


int Server::SockReceive()
{
	long result					        {0};							// Result value
	char bufRcv[500]				    {""};					        // Message received buffer

    // Receive data from client
    result = recv(mSockAddrClient, &bufRcv, sizeof(bufRcv), 0);
    
    // Save received message
    msgRcv.assign(bufRcv);
    
    return (int)result;
}


int Server::SockSend(std::string bufSend)
{
	unsigned long byteCount				{0};							// Number of byte sent
    const char *msgToSend               {bufSend.c_str()};              // Message to send

	// Send message passed as argument
	byteCount = send(mSockAddrClient, msgToSend, strlen(msgToSend) + 1, 0);
	if (byteCount != strlen(msgToSend) + 1)
	{
		std::cout << "Message send failed" << std::endl;
		SockClose(mSockAddrClient);
		SockClose(mSockAddrServ);
		return 1;
	}
	else
		return 0;
}

void Server::SockClose(int sockAddr)
{
    // Close socket connection
#ifdef _WIN32

	closesocket(sockAddr);
	WSACleanup();
#else

    close(sockAddr);
#endif
}
