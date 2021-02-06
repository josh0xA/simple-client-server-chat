#include "client.h"

std::string cl::Callback(void* data) {
  std::string& str = *(static_cast<std::string*>(data));
  return str; 
}

template <typename ClientType>
bool cl::Client<ClientType>::ValidateSocket(ClientType sock) { 
  if (sock <= DEF_SUCCESS_CODE) { 
    throw std::runtime_error("socket() - fatal");
  }
  return true; 
}

template <typename ClientType>
bool cl::Client<ClientType>::ConnectSocket(ClientType sock, ClientType portnumber) {
  server = gethostbyname(localhost.c_str());
  if (server == nullptr) { 
    throw std::runtime_error("gethostbyname() - fatal"); 
  }
  bzero(reinterpret_cast<char*> (&serv_addr), sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  bcopy(reinterpret_cast<char*> (server->h_addr), reinterpret_cast<char*> (&serv_addr.sin_addr.s_addr), server->h_length);
  serv_addr.sin_port = htons(portnumber); 

  if (connect(sock, reinterpret_cast<sockaddr*> (&serv_addr), sizeof(serv_addr)) <= DEF_ERROR_CODE) {
    throw std::runtime_error("connect() - fatal\n");
  }
  connected = true; 
  return connected; 
}

template <typename ClientType>
bool cl::Client<ClientType>::SendData(ClientType sock) {
  char recievestream[DEF_BUFFER_SIZE];
  char sendstream[DEF_BUFFER_SIZE]; 
  
  while (std::cin.getline(sendstream, DEF_BUFFER_SIZE)) {
    send(sock, sendstream, sizeof(sendstream) + 1, DEF_SUCCESS_CODE); 
    if (strcmp(sendstream, "exit\n") == DEF_SUCCESS_CODE) {
      break; 
    }
    recv(sock, recievestream, sizeof(recievestream), DEF_SUCCESS_CODE);
    std::cout << "From Server: " << recievestream << '\n';

    memset(sendstream, 0, sizeof(sendstream));
    memset(recievestream, 0, sizeof(recievestream)); 
  }
  close(sock);
  return false; 
}

int main(void) {
  cl::Client<__socket__> clnt; 
  clnt.socketfd = socket(AF_INET, SOCK_STREAM, DEF_SUCCESS_CODE);
  clnt.portnumber = 7001;
  
  if (clnt.ValidateSocket(clnt.socketfd)) {
    clnt.ConnectSocket(clnt.socketfd, clnt.portnumber);
    clnt.SendData(clnt.socketfd);
  } 
  
}



