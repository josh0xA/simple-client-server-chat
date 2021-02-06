#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <netdb.h>
#include <iostream>

#define DEF_SUCCESS_CODE 0 
#define DEF_ERROR_CODE -1
#define DEF_BUFFER_SIZE 4096

#define QUEUE 20
#define DEF_SET_VALUE(s_val, k_val) ((s_val) = (k_val))

typedef int __socket__; 

namespace srv {
  constexpr short __max_host__ = 1025;
  constexpr short __max_service__ = 32;

  template <typename ServerType>
  class Server {
  public:
    bool ValidateSocket(ServerType sock);
    void GetClientHost(ServerType portnumber, sockaddr_in clientaddress);

    bool ConnectClients(ServerType sock, sockaddr_in server_address, sockaddr_in clientaddress);
    bool BroadcastClients(ServerType sock, bool connected);
  public:
    ServerType socketfd, portnumber; 
    bool conn_active = false;
  private:
    char sendbuffer[DEF_BUFFER_SIZE];
    char recievebuffer[DEF_BUFFER_SIZE]; 
    socklen_t socketlength_client; 
    
    __socket__ data_length, connection;

  };

}

#endif 
