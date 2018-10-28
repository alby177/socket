//============================================================================
// Name        : CrossPlatSocket.cpp
// Author      : Alberto Gotta
// Version     :
// Copyright   : Your copyright notice
// Description : Cross platform socket
//============================================================================

#include <iostream>
#include <string>
#include <stdio.h>
#include "Server.h"
#include "Client.h"

int main(int argc, char *argv[]) {
    
    // Check for command line arguments
    if (argc > 4 || argc < 2)
    {
        std::cout << "Wrong number of parameters, check documentation" << std::endl;
        return 1;
    }
    
    if (strcmp(argv[1], "Server") == 0 || strcmp(argv[1], "server") == 0)
    {
        Server server1(2000);                               // Server instance

        std::cout << "Server is coming alive" << std::endl;
        
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
        
        // Close socket
        server1.SockClose(server1.GetSockAddrServ());
        std::cout << "Server is down" << std::endl;
    }
    
    else if (strcmp(argv[1], "Client") == 0 || strcmp(argv[1], "client") == 0)
    {
        char *mIPAddr                       {(char*)"127.0.0.1"};               // Server IP address
        unsigned short mPort                {2000};                             // Server port
        std::string msgToSend               {""};                               // Message to send
        
        // Check for user defined IP address and port
        if (argc == 3)
        {
            mPort = atoi(argv[2]);
        }
        else if (argc == 4)
        {
            mIPAddr = argv[2];
            mPort = atoi(argv[3]);
        }
        
        std::cout << "Client is trying to connect at " << mIPAddr << " on port " << mPort << std::endl;
        
        Client client1(mIPAddr, mPort);                                                       // Client instance
        
        // Create socket
        if (client1.SockCreate() == 1)
            return 1;
        
        std::cout << "Socket created, trying to connect" << std::endl;
        
        // Connect socket
        while(client1.SockConnect() != 0)
        {
            std::cout << ".";
            std::cout.flush();
            usleep(1000000);
        }
        
        std::cout << std::endl << "Client connected" << std::endl;
        
        // Check for server termination message
        while(1)
        {
            std::cout << "Send message to Server: " << std::endl;
            std::cin >> msgToSend;
            
            // Send message to server
            if (client1.SockSend(msgToSend) != 0)
                break;
            
            std::cout << "Message sent: " << msgToSend << std::endl;
            
            // Check for server kill message
            if (msgToSend != "Muori" && msgToSend != "muori")
            {
                // Receive message
                if (client1.SockReceive() < 0)
                    return 1;
                
                // Check for correctly received message
                if (client1.GetReceivedMsg() != "")
                    std::cout << "Message received: " << client1.GetReceivedMsg() << std::endl;
                else
                    std::cout << "No message receveived" << std::endl;
            }
            else
            {
                std::cout << "Asked server to shutdown itself" << std::endl;
                break;
            }
        }
        
        // Close socket
        client1.SockClose(client1.GetSockConnAddr());
        std::cout << "Client is down" << std::endl;
        return 0;
    }
    
}
