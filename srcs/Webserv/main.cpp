#include "Webserv.hpp"

static void InstallIgnoreSignalHandler() {
    struct sigaction ignore_act;
    bzero(&ignore_act, sizeof(struct sigaction));
    ignore_act.sa_handler = SIG_IGN;
    // ゾンビプロセスを回収するためにSIGCHLDをignore
    if (sigaction(SIGPIPE, &ignore_act, NULL) == -1 ||
        sigaction(SIGCHLD, &ignore_act, NULL) == -1) {
        throw std::runtime_error("Error: sigaction " +
                                 std::string(strerror(errno)));
    }
}

int main(int argc, char **argv) {
    InstallIgnoreSignalHandler();

    Webserv webserv;
    webserv.Run(argc, argv);
}
