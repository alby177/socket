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

	Server server1(2000);                               // Server instance

    // Create socket
	if (server1.SockCreate() == 1)
		return 1;

    std::cout << "Socket created!" << std::endl;
    
    // Bind socket
	if (server1.SockBind() == 1)
		return 1;

    std::cout << "Socket binded!" << std::endl;
    
    // Start listening on socket
	if(server1.SockListen() == 1)
        return 1;
    
    std::cout << "Socket is listening, waiting for connections!" << std::endl;
    
    // Waiting for client connection
    do
    {
        std::cout << ".";
        std::cout.flush();
        usleep(1000000);
        
    }while(server1.SockAccept() != 0);

	std::cout << "Connected!" << std::endl;

    // Communicate with client
    while(1)
    {
        // Check for received message
        if (server1.SockReceive() > 0)
        {
            // Check for shutdown message
            if (server1.GetMessageReceived() == "Muori" || server1.GetMessageReceived() == "muori")
            {
                // Wait for client disconnection
                while (server1.SockReceive() != 0)
                    std::cout << "Wating for client to disconnect" << std::endl;
                std::cout << "Client disconnected" << std::endl;
                break;
            }
            else
            {
                std::cout << "Received message: " << server1.GetMessageReceived() << std::endl;
                std::cout << "Sending: " << server1.GetMessageReceived() + " from server" << std::endl;
                
                // Echo
                if (server1.SockSend(server1.GetMessageReceived() + " from server") == 1)
                    break;
            }
        }
    }
    
    server1.SockClose(server1.GetSockAddrServ());
    std::cout << "Server is down" << std::endl;
	
	return 0;
}
