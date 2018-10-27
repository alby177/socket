//============================================================================
// Name        : CrossPlatSocket.cpp
// Author      : Alberto Gotta
// Version     :
// Copyright   : Your copyright notice
// Description : Cross platform socket
//============================================================================

#include <iostream>
#include <string>
#include "Server.h"

int main() {

	Server server1(2000);
	std::string msgRcv			{""};

	if (server1.SockCreate() == 1)
		return 1;

    std::cout << "Socket created!" << std::endl;
    
	if (server1.SockBind() == 1)
		return 1;

    std::cout << "Socket binded!" << std::endl;
    
	if(server1.SockListen() == 1)
        return 1;
    
    std::cout << "Socket is listening, waiting for connections!" << std::endl;
    
    if (server1.SockAccept() != 0)
        return 1;

	std::cout << "Connected!" << std::endl;
    
    while(1)
    {
        msgRcv = server1.SockReceive();
        if (msgRcv == "Muori" || msgRcv == "muori")
            break;
        
        std::cout << "Received message: " << msgRcv << std::endl;
        
        if (server1.SockSend(msgRcv) == 1)
            return 1;
    }
    
    server1.SockClose(server1.GetSockAddrServ());
    std::cout << "Server is down" << std::endl;
	
	return 0;
}
