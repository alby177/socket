// Basic Server implementation
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  struct sockaddr_in saddr, caddr;           // Address structs for server and client
  int sockfd, isock = 0;
  socklen_t clen;
  unsigned short port = 2000;               // Server port
  char msg[]= "Ciao";

  // Create socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Verify that the socket working
  if (sockfd < 0)
  {
    std::cout << "Cannot create socket" << std::endl;
    return 0;
  }

  // Reset server address struct
  memset(&saddr, '\0', sizeof(saddr));

  // Set the socket structure as the open socket
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(port);

  // Bind socket to the address structure and verify it
  if (bind(sockfd, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
  {
    std::cout << "Cannot bind socket" << std::endl;
    close(sockfd);
    return 0;
  }

  // Start to wait for connection on the socket
  if (listen(sockfd, 5) < 0)
  {
    std::cout << "Cannot listen" << std::endl;
    close(sockfd);
    return 0;
  }

  // Save size client address data structure
  clen = sizeof(caddr);
  std::cout << "Waiting for a client to connect" << std::endl;

  int byteSent;
  int received;
  char msgRcv[100] = "";

  // Set the socket as non-blocking
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  do
  {
    // Accept client connection and verify it
    isock = accept(sockfd, (sockaddr*) &caddr, &clen);

    // Wait for client
    if (isock < 0)
    {
      std::cout << ".";
      std::cout.flush();
      usleep(1000000);
    }
  }while(isock < 0);

  std::cout << "\nClient connected" << std::endl;

  while(1)
  {
    // Receive message
    received = recv(isock, msgRcv, sizeof(msgRcv), 0);

    // Create string to send back message
    char msgReSent[received];
    int i = 0;

    // Verify message received
    if (received > 0)
      {
        std::cout << "Message received: " << msgRcv << std::endl;

        // Check for client request to kill server
        if (strcmp(msgRcv, "Muori") == 0 || strcmp(msgRcv, "muori") == 0)
        {
          // Void message
          char msgVoid[100] = "";

          std::cout << "Client disconnection..." << std::endl;

          // Wait for the client to be disconnected
          while(recv(isock, msgVoid, sizeof(msgVoid), 0) != 0)
            std::cout << "|";

          std::cout << std::endl << "Client disconnected!" << std::endl;

          // Close client socket
          close(isock);

          // Exit the cycle
          break;
        }
        else
        {
          // Send dummy message
          byteSent = send(isock, msgRcv, strlen(msgRcv) + 1, 0);

          // Check for message sent
          if (byteSent != strlen(msgRcv) + 1)
          {
            std::cout << "Message send failed, size " << byteSent << std::endl;
            close(isock);
          }
          else
          {
            std::cout << "Message: \"" << msgRcv << "\" of " << byteSent << " character sent" << std::endl;
          }
        }
    }
  }

  // Close the connection
  close(sockfd);
  return 0;
}
