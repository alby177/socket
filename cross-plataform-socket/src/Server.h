/*
 * Server.h
 *
 *  Created on: 26 ott 2018
 *      Author: Gotta
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <string>
#define DEFAULT_PORT "27015"

#ifdef _WIN32
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT_WIN7 0x0601  /* Windows 7. */
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <stdio.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

class Server {
public:
	Server(unsigned short port);									// Constructor
	~Server();														// Destructor
	int SockCreate();												// Socket creation
	int SockBind();													// Socket binding
	int SockListen();												// Socket listen
	int SockAccept();												// Socket accept
	std::string SockReceive();										// Socket receive
	int SockSend(std::string bufSend);								// Socket send
	void SockClose(int sockAddr);				                    // Socket close
    int GetSockAddrServ() {return mSockAddrServ;};                  // Socket address getter
    
private:
	unsigned short mPort				{0};						// Server port number
	int mSockAddrServ					{0};						// Socket server address
	int mSockAddrClient					{0};						// Socket client address

#ifdef _WIN32
	struct addrinfo mSAddr;											// Server configuration structure Windows
	struct addrinfo *mResult			{nullptr};					// Server configuration resolved address structure Windows
	struct addrinfo mCAddr;											// Client configuration structure Windows
#else
	struct sockaddr_in mSAddr;										// Server configuration structure Unix
	struct sockaddr_in mCAddr;										// Client configuration structure Unix
#endif

};

#endif /* SERVER_H_ */
