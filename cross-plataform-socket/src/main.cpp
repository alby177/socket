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

	if (server1.SockBind() == 1)
		return 1;

	if(server1.SockListen() == 1)
		return 1;

	std::cout << "Waiting for connection" << std::endl;

	do
	{
		std::cout << ".";
		std::cout.flush();

	}while(server1.SockAccept() != 0);

	std::cout << "Connected!" << std::endl;

	msgRcv = server1.SockReceive();

	msgRcv = server1.SockSend(msgRcv);

	return 0;
}
