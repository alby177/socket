# SOCKET
This repository has been created to learn how to handle sockets.

## CONTENT
* #### general:
  A generic socket implementation with POSIX client and server.
 
  ##### Client:
   POSIX client which is run specifying command line parameters:

   * No parameters: The client tries to connect to localhost

   * 1 parameter: The first parameter is for the IP address where the client must connect

   * 2 parameter: The second parameter is taken as massage to send to server. If this parameter is not specified, the client waits for the user to insert a message to send

  ##### Server:
  POSIX server which takes client message and echoes it sending it back to the client.

* #### cross-platform-socket
  A non-socket implementation thought to be used with every kind of system. It provides API to use as resembling classical POSIX functions.
  ##### Server:
  POSIX server class used for providing the server API.
  ##### main:
  Here the server is practically implemented using the API provided by the server object.
