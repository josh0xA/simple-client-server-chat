#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netinet/in.h>
#include <netdb.h>

#define DEF_BUFFER_SIZE 0x400
#define DEF_SUCCESS_CODE 0
#define DEF_ERROR_CODE -1 

typedef int __socket__; 

namespace cl {
  const std::string localhost = "127.0.0.1";
  std::string Callback(void* data);

  template <typename ClientType>
  class Client {
  public:
    bool ValidateSocket(ClientType sock);
    bool ConnectSocket(ClientType sock, ClientType portnumber);
    bool SendData(ClientType sock);    
  public:
    ClientType socketfd;
    ClientType portnumber;
    bool connected = false; 

  private:
    std::array<char, DEF_BUFFER_SIZE> buffer; 
    
    struct sockaddr_in serv_addr;
    struct hostent* server;     
  };

}

#endif 

