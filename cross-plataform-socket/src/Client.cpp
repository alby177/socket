//
//  Client.cpp
//  XcPrj
//
//  Created by Alberto Gotta on 27/10/2018.
//  Copyright © 2018 Alberto Gotta. All rights reserved.
//

#include "Client.h"

Client::Client(const char* IPAddress, unsigned short port)
{
    
    // Save parameters
    mIPAddr = (char*)IPAddress;
    mPort = port;
    
    // Set configuration structure
#ifdef _WIN32
    
    ZeroMemory(&mCAddr, sizeof(mCAddr));
    mCAddr.ai_family = AF_INET;
    mCAddr.ai_socktype = SOCK_STREAM;
    mCAddr.ai_protocol = IPPROTO_TCP;
    mCAddr.ai_flags = AI_PASSIVE;
#else
    
    memset(&mCAddr, '\0', sizeof(mCAddr));
    mCAddr.sin_family = AF_INET;
    mCAddr.sin_addr.s_addr = htonl(*IPAddress);
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
    
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }
    
    // Resolve the server address and port
    result = getaddrinfo(mIPAddr, mPort, &mSAddr, &mResult);
    if ( result != 0 )
    {
        std::cout << "getaddrinfo failed with error: " << result << std::endl;
        WSACleanup();
        return 1;
    }
    
    // Create a SOCKET for connecting to server
    mSockConnect = socket(mResult->ai_family, mResult->ai_socktype, mResult->ai_protocol);
    if (mSockAddrServ == int(INVALID_SOCKET))
    {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(mResult);
        WSACleanup();
        return 1;
    }
#else
    
    std::cout << "Connecting to: " << mIPAddr << std::endl;
    
    // Create socket
    if ((mSockAddr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Cannot create socket" << std::endl;
        return 1;
    }
    
    // Set return value
    result = 0;
#endif
    
    return result;
}

int Client::SockConnect()
{
#ifdef _WIN32
    
    // Connect socket to server
    mSockConnect = socket(mCAddr->ai_family, mCAddr->ai_socktype, mCAddr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    return 0;
#else
    
    // Connect socket to server
    mSockConnect = connect(mSockAddr, (struct sockaddr *) &mCAddr, sizeof(mCAddr));
    if (mSockConnect >= 0)
        return 0;
    else
        return 1;    
#endif
}

int Client::SockReceive()
{
    
    long result                             {0};                            // Result value
    char bufRcv[500]                        {""};                           // Message received buffer
    
    // Receive data from client
    result = recv(mSockConnect, &bufRcv, sizeof(bufRcv), 0);
    
    // Save received message
    msgRcv.assign(bufRcv);
    
    return (int)result;
}

int Client::SockSend(std::string bufSend)
{
    unsigned long byteCount                 {0};                            // Number of byte sent
    const char *msgToSend                   {bufSend.c_str()};              // Message to send
    
    // Send message passed as argument
    byteCount = send(mSockConnect, msgToSend, strlen(msgToSend) + 1, 0);
    if (byteCount != strlen(msgToSend) + 1)
    {
        std::cout << "Message send failed" << std::endl;
        SockClose(mSockConnect);
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
