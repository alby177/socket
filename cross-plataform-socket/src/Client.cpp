//
//  Client.cpp
//  XcPrj
//
//  Created by Alberto Gotta on 27/10/2018.
//  Copyright © 2018 Alberto Gotta. All rights reserved.
//

#include "Client.h"

Client::Client(const char *IPAddress, unsigned short port)
{

    // Save constructor arguments
    mIPAddr = IPAddress;
    mPort = port;

    // Set configuration structure
#ifdef _WIN32

    ZeroMemory(&mCAddr, sizeof(mCAddr));
    /*
    mCAddr.ai_family = AF_INET;
    mCAddr.ai_socktype = SOCK_STREAM;
    mCAddr.ai_protocol = IPPROTO_TCP;
    */
    mCAddr.sin_family = AF_INET;
    mCAddr.sin_addr.s_addr = inet_addr(mIPAddr);
    mCAddr.sin_port = htons(mPort);
#else

    memset(&mCAddr, '\0', sizeof(mCAddr));
    mCAddr.sin_family = AF_INET;
    mCAddr.sin_addr.s_addr = inet_addr(mIPAddr);
    mCAddr.sin_port = htons(port);
#endif
}

Client::~Client()
{
}

int Client::SockCreate()
{

    int result                        {0};                                // Result value

#ifdef _WIN32

    WSADATA wsaData;                                                      // Windows socket implementation information info
    char port[10]					  {};								  // Port number used for connection

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }
    /*
    // Save port number into a C string
    sprintf(port, "%d", mPort);

    // Resolve the server address and port
    result = getaddrinfo(mIPAddr, port, &mCAddr, &mResult);
    if ( result != 0 )
    {
        std::cout << "getaddrinfo failed with error: " << result << std::endl;
        WSACleanup();
        return 1;
    }
    */

    // Create a SOCKET for connecting to server
    //mSockAddr = socket(mResult->ai_family, mResult->ai_socktype, mResult->ai_protocol);
    mSockAddr = socket(AF_INET, SOCK_STREAM, 0);
    if (mSockAddr == int(INVALID_SOCKET))
    {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(mResult);
        WSACleanup();
        return 1;
    }

    u_long iMode    {1};                                                // Non-blocking value for windows socket

    // Set socket as non-blocking
    if(ioctlsocket(mSockAddr, FIONBIO, &iMode) != NO_ERROR)
        std::cout << "Cannot set socket as non-blocking" << std::endl;
#else

    // Create socket
    if ((mSockAddr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Cannot create socket" << std::endl;
        return 1;
    }

    fcntl(mSockAddr, F_SETFL, O_NONBLOCK);

#endif

    // Set return value
    result = 0;

    return result;
}

int Client::SockConnect()
{
#ifdef _WIN32

    int result {0};

	// Connect to server.
    //result = connect(mSockAddr, mResult->ai_addr, static_cast<int>(mResult->ai_addrlen));
    result = connect(mSockAddr, (struct sockaddr *) &mCAddr, sizeof(mCAddr));
    std::cout << result << std::endl;
    return result;
#else

    // Connect to server
    return(connect(mSockAddr, (struct sockaddr *) &mCAddr, sizeof(mCAddr)));
#endif
}

int Client::SockReceive()
{
    long result                             {0};                            // Result value
    char bufRcv[500]                        {""};                           // Message received buffer

    // Receive data from client
    result = recv(mSockAddr, bufRcv, sizeof(bufRcv), 0);

    // Save received message
    msgRcv.assign(bufRcv);

    return (int)result;
}

int Client::SockSend(std::string bufSend)
{
    unsigned long byteCount                 {0};                            // Number of byte sent
    const char *msgToSend                   {bufSend.c_str()};              // Message to send

    // Send message passed as argument
    byteCount = send(mSockAddr, msgToSend, strlen(msgToSend), 0);
    if (byteCount != strlen(msgToSend))
    {
        std::cout << "Message send failed" << std::endl;
        //SockClose(mSockConnect);
        //SockClose(mSockAddr);
        return 1;
    }
    else
        return 0;
}

void Client::SockClose(int sockAddr)
{
    // Close socket connection
#ifdef _WIN32

    closesocket(sockAddr);
    WSACleanup();
#else

    close(sockAddr);
#endif
}
