#include <iostream>
#include <string>

#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

#define CYAN "\033[36m"
#define RED "\033[31m"
#define RESET "\033[0m"

int main(int ac, char **av) {
  if (ac != 2) {
    std::cerr << "Usage: " << av[0] << " <config file path>" << std::endl;
    return 0;
  }

  Config conf = Config::Parse(av[1]);

  #if DEBUG
  std::cerr << "[debug] Success to parse config" << std::endl;
  #endif

  // TODO Serverクラスを作るか
  // Server serv(conf);
  // Socket serv = Socket::OpenListenSocket(conf);
  int listenfd = Socket::OpenListenRawSocket(conf); // ソケットのfdを取り合えずそのまま返す

  // #if DEBUG
  // std::cerr << "[debug] server is listening to " << serv.GetFd() << std::endl;
  // assert(serv.GetFd() >= 0);
  // #endif
  #if DEBUG
  std::cerr << "[debug] server is listening to " << listenfd << std::endl;
  std::cerr << "[debug] here?" << std::endl;
  // assert(listenfd >= 0);
  #endif

  // Socket client;
  std::string req_str;
  Request req;
  Response res;
  while (true) {
    // Socket client = serv.Accept();
    int clientfd = Socket::RawAccept(listenfd);

    #if DEBUG
    std::cerr << "[debug] server accept client connection" << std::endl;
    #endif

    // req_str = client.Receive();
    req_str = Socket::RawReceive(clientfd);
    #if DEBUG
    std::cerr << "[debug] server receive HTTP request from client" << std::endl;
    std::cerr << CYAN << "<<<<< Request From Client" << RESET << std::endl;
    std::cerr << req_str << std::endl;
    std::cerr << CYAN << ">>>>> Request From Client" << RESET << std::endl;
    #endif

    req = Request::Parse(req_str);
    #if DEBUG
    std::cerr << "[debug] Success to parse request" << std::endl;
    std::cerr << req << std::endl;
    #endif

    res = req.ExecMethod();
    #if DEBUG
    std::cerr << "[debug] Success to exec some operation" << std::endl;
    std::cerr << RED << "<<<<< Response From Server" << RESET << std::endl;
    std::cerr << res << std::endl;
    std::cerr << RED << ">>>>> Response From Server" << RESET << std::endl;
    #endif

    // client.Send(res);
    Socket::RawSend(clientfd, res);
    #if DEBUG
    std::cerr << "[debug] Success to send HTTP response to client" << std::endl;
    #endif

    close(clientfd);
  }
  close(listenfd);
}
