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
  struct sockaddr_in saddr;        // Server address structure
  int sockfd, connfd;       // Server socket Address and connection number
  unsigned short port = 2000; // Server port

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
  if ((connfd = connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr))) < 0)
  {
    std::cout << "Cannot connect to server socket" << std::endl;
    close(sockfd);
    return 0;
  }

  std::cout << "Connected" << std::endl;

  int received = 0;         // Received bytes size
  char msgSend[100];        // Message to send
  char msg[100] = "";       // Recevied message

  // Check for too many inputs
  if (argc > 2)
  {
    // Stop program execution
    std::cout << "Too many command line input" << std::endl;
    return 0;
  }
  else

    // Save command line message
    strcpy(msgSend, argv[1]);

  // Send command line message and verify message sent
  if (send(sockfd, msgSend, sizeof(msgSend), 0) != sizeof(msgSend))
  {
    std::cout << "Message send failed" << std::endl;
    close(sockfd);
  }

  std::cout << "Message sent: " << msgSend << std::endl;

  // Check for server kill message
  if (strcmp(msgSend, "Muori") != 0 && strcmp(msgSend, "muori") != 0 )
  {
    // Receive message and verify it
    if ((received = recv(sockfd, msg, sizeof(msg), 0)) < 0)
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

  // Close the socket
  close(sockfd);
  return 0;
}
