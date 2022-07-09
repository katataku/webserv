#include <cassert>
#include <iostream>
#include <string>

#include "./Config.hpp"
#include "./Request.hpp"
#include "./Response.hpp"
#include "./Socket.hpp"
#include "Webserv.hpp"

int main(int argc, char **argv) {
    Webserv webserv;
    webserv.Run(argc, argv);
}
