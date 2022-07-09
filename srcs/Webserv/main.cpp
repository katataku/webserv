#include <cassert>
#include <iostream>
#include <string>

#include "./Config.hpp"
#include "./Request.hpp"
#include "./Response.hpp"
#include "./Socket.hpp"

int main(int argc, char **argv) {
    std::cout << "argc:" << argc << std::endl;
    std::cout << "argv:" << argv << std::endl;

    Webserv webserv;
    webserv.Run(argc, argv);
}
