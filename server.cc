#include "server.h"

template <typename ServerType>
bool srv::Server<ServerType>::ValidateSocket(ServerType sock) {
  if (sock == DEF_ERROR_CODE) {
    throw std::runtime_error("socket() - fatal\n");
  }
  return true;
}

template <typename ServerType>
bool srv::Server<ServerType>::ConnectClients(ServerType sock, sockaddr_in server_address, sockaddr_in clientaddress) {
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portnumber);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, reinterpret_cast<sockaddr*> (&server_address), sizeof(server_address)) == DEF_ERROR_CODE) {
    throw std::runtime_error("bind() - fatal\n");
  }
  if (listen(sock, QUEUE) <= DEF_ERROR_CODE) {
    throw std::runtime_error("listen() - fatal\n");
  }
  socketlength_client = sizeof(clientaddress);
  connection = accept(sock, reinterpret_cast<sockaddr*> (&clientaddress), &socketlength_client);
  if (connection <= DEF_ERROR_CODE) {
    throw std::runtime_error("Cannot Accept Connection\n");
  }
  return true; 
}

template <typename ServerType>
void srv::Server<ServerType>::GetClientHost(ServerType portnumber, sockaddr_in clientaddress)
{
  char hostbuffer[srv::__max_host__];
  char servicebuffer[srv::__max_service__];

  memset(hostbuffer, 0, sizeof(hostbuffer));
  memset(servicebuffer, 0, sizeof(servicebuffer));

  if (getnameinfo(reinterpret_cast<sockaddr*> (&clientaddress), sizeof(clientaddress), hostbuffer, srv::__max_host__,
      servicebuffer, srv::__max_service__, DEF_SUCCESS_CODE) == DEF_SUCCESS_CODE) {
    std::cout << hostbuffer << " : connected on port: " << servicebuffer << " (SRV: " << portnumber 
      << ") " << '\n';
  } else {
      inet_ntop(AF_INET, &clientaddress.sin_addr, hostbuffer, srv::__max_host__);
      std::cout << hostbuffer << " : connected on port: " << ntohs(clientaddress.sin_port) << '\n';
  }

}

template <typename ServerType>
bool srv::Server<ServerType>::BroadcastClients(ServerType sock, bool connected) {
  while (connected) {
    memset(sendbuffer, 0, sizeof(sendbuffer));
    data_length = recv(connection, recievebuffer, sizeof(recievebuffer) + 1, 0);
    std::cout << "Recieved: " << recievebuffer << '\n';
    if (strcmp(sendbuffer, "exit\n") == DEF_SUCCESS_CODE) {
      break;
    }
    std::cin.getline(sendbuffer, DEF_BUFFER_SIZE);
    send(connection, sendbuffer, data_length + 1, DEF_SUCCESS_CODE);
  }
  close(connection);
  close(sock);
  return true;
}

int main(void)
{
  srv::Server<__socket__> server; 

  server.socketfd = socket(AF_INET, SOCK_STREAM, DEF_SUCCESS_CODE);
  server.portnumber = 7001; 
  struct sockaddr_in server_address, clientaddress;
  
  if (server.ValidateSocket(server.socketfd)) { 
    if (server.ConnectClients(server.socketfd, server_address, clientaddress)) {
      server.conn_active = true;
      server.GetClientHost(server.portnumber, clientaddress);
      server.BroadcastClients(server.socketfd, server.conn_active);
    }
    return DEF_ERROR_CODE;
  }
  
}



