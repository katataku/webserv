#include "Webserv.hpp"

static void InstallIgnoreSIGPIPEHandler() {
    struct sigaction ignore_act;
    ignore_act.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &ignore_act, NULL) == -1) {
        throw std::runtime_error(strerror(errno));
    }
}

int main(int argc, char **argv) {
    InstallIgnoreSIGPIPEHandler();

    Webserv webserv;
    webserv.Run(argc, argv);
}
