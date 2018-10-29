//
//  Client.h
//  XcPrj
//
//  Created by Alberto Gotta on 27/10/2018.
//  Copyright © 2018 Alberto Gotta. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include <iostream>
#include <fcntl.h>
#define DEFAULT_PORT "27015"
#define LOCALHOST "127.0.0.1"

#ifdef _WIN32
  #ifndef _WIN32_WINNT
      //#define _WIN32_WINNT_WIN10 0x0A00 // Windows 10
	  #define _WIN32_WINNT_WIN7 0x0601 // Windows 7
  #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
  #else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <stdio.h>
  #include <string.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

class Client
{
public:
    Client(const char *IPAddress = "127.0.0.1", unsigned short port = 2000);            // Constructor
    ~Client();                                                                          // Destructor
    int SockCreate();                                                                   // Socket creation
    int SockConnect();                                                                  // Socket connect
    int SockReceive();                                                                  // Socket receive
    int SockSend(std::string bufSend);                                                  // Socket send
    void SockClose(int sockAddr);                                                       // Socket close
    std::string GetReceivedMsg()    {return msgRcv;};                                   // Received message getter
    int GetSockAddr()               {return mSockAddr;};                                // Connected socket number getter

private:
    const char *mIPAddr             {};                                                 // IP address for client connection
    unsigned short mPort            {0};                                                // Port for client connection
    int mSockAddr                   {-1};                                               // Socket created
    std::string msgRcv              {""};                                               // Message received from server

#ifdef _WIN32
    struct addrinfo mSAddr;                                                             // Server configuration structure Windows
    struct addrinfo *mResult            {nullptr};                                      // Server configuration resolved address structure Windows
    struct addrinfo mCAddr;                                                             // Client configuration structure Windows
#else

    struct sockaddr_in mCAddr;                                                          // Client configuration structure Unix
#endif

};

#endif /* Client_hpp */
