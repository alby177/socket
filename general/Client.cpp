// Basic Client implementation
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, const char *argv[])
{
  struct sockaddr_in saddr;           // Server address structure
  int sockfd, connfd;                 // Server socket Address and connection number
  unsigned short port = 2000;         // Server port
  bool connected = false;             // Client connection flag
  unsigned int connectionTries = 0;   // Connection tries accumulator

  // Create socket and verify the socket working
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cout << "Cannot create socket" << std::endl;
    return 0;
  }

  // Reset server address struct
  memset(&saddr, '\0', sizeof(saddr));

  // Set the socket structure as the open socket
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  saddr.sin_port = htons(port);

  // Connect to server socket and verify connection
  while (connected == false)
  {
    connfd = connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (connfd >= 0)
    {
      connected = true;
    }
    else
    {
      ++connectionTries;
      std::cout << ".";
      std::cout.flush();
      usleep(1000000);
    }

    // Check for too much connection tries
    if (connectionTries > 10000)
    {
      std::cout << "Connection failed, server not available" << std::endl;
      close(sockfd);
      return 0;
    }
  }

  std::cout << "\nConnected" << std::endl;

  int received = 0;           // Received bytes size
  char msgSend[100];          // Message to send
  char msg[100] = "";         // Recevied message
  bool manualMessage = false; // Flag for manual message
  int sizeSend = 0;           // Size of the message sent

  // Check for inputs
  if (argc > 2)
  {
    // Stop program execution
    std::cout << "Too many command line input" << std::endl;
    return 0;
  }
  else if (argc == 2)
  {
    // Save command line message
    strcpy(msgSend, argv[1]);
  }
  else if (argc == 1)
    manualMessage = true;

  while(strcmp(msgSend, "Muori") != 0 && strcmp(msgSend, "muori") != 0)
  {
    if (manualMessage == true)
    {
      std::cout << "Send message to Server: " << std::endl;
      std::cin >> msgSend;
    }

    // Send command line message and verify message sent
    sizeSend = send(sockfd, msgSend, sizeof(msgSend), 0);
    if (sizeSend != sizeof(msgSend))
    {
      std::cout << "Message send failed" << std::endl;
      close(sockfd);
    }

    std::cout << "Message sent: " << msgSend << std::endl;

    // Check for server kill message
    if (strcmp(msgSend, "Muori") != 0 && strcmp(msgSend, "muori") != 0 )
    {
      // Receive message and verify it
      received = recv(sockfd, msg, sizeof(msg), 0);
      if (received < 0)
      {
        std::cout << "Error receiving message" << std::endl;
        close(sockfd);
      return 0;
      }

      // Print message received
      if (strcmp("", msg))
        std::cout << "Message received: " << msg << std::endl;
      else
        std::cout << "No message receveived" << std::endl;
    }
    else
      std::cout << "Asked server to shutdown itself" << std::endl;
  }

  // Close the socket
  close(sockfd);
  return 0;
}
