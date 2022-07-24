#include "CGIExecutor.hpp"

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <map>
#include <string>
#include <vector>

#include "CGIRequest.hpp"
#include "CGIResponse.hpp"

CGIExecutor::CGIExecutor() : logging_(Logging(__FUNCTION__)) {}

CGIExecutor::CGIExecutor(CGIExecutor const &other) { *this = other; }

CGIExecutor &CGIExecutor::operator=(CGIExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

CGIExecutor::~CGIExecutor() {}

HTTPResponse *CGIExecutor::Exec(HTTPRequest const &request,
                                ServerLocation const &sl) {
    CGIRequest cgi_req(request, sl);
    CGIResponse cgi_res = this->CGIExec(cgi_req, sl);
    // TODO(iyamada) ResponseBuilderに任せても良いかも
    return cgi_res.ToHTTPResponse();
}

extern char **environ;

static int execve(const std::string &path, std::vector<std::string> arg,
                  std::map<std::string, std::string> env) {
    // char *av[] = {"./../../sample_data/cgi-bin/cgi-req-dumper.py", NULL};

    char **av = new char *[arg.size() + 1];

    for (std::vector<std::string>::size_type i = 0; i != arg.size(); ++i) {
        av[i] = const_cast<char *>(arg[i].c_str());
    }
    av[arg.size()] = NULL;

    setenv("CONTENT_LENGTH", env["CONTENT_LENGTH"].c_str(), 1);
    setenv("CONTENT_TYPE", env["CONTENT_TYPE"].c_str(), 1);
    setenv("PATH_INFO", env["PATH_INFO"].c_str(), 1);
    setenv("REQUEST_METHOD", env["REQUEST_METHOD"].c_str(), 1);
    setenv("SERVER_PROTOCOL", env["SERVER_PROTOCOL"].c_str(), 1);

    return execve(path.c_str(), av, environ);
}

CGIResponse CGIExecutor::CGIExec(CGIRequest const &req,
                                 ServerLocation const &sl) {
    int pipe_to_cgi[2], pipe_to_serv[2];

    if (pipe(pipe_to_cgi) == -1) {
        std::runtime_error("Error: pipe failed " +
                           std::string(strerror(errno)));
    }
    if (pipe(pipe_to_serv) == -1) {
        std::runtime_error("Error: pipe failed " +
                           std::string(strerror(errno)));
    }

    if (fork() == 0) {
        dup2(pipe_to_cgi[0], STDIN_FILENO);
        dup2(pipe_to_serv[1], STDOUT_FILENO);
        if (execve(req.path(), req.arg(), req.env()) == -1) {
            std::runtime_error("Error: execve failed " +
                               std::string(strerror(errno)));
        }
        close(pipe_to_cgi[0]);
        close(pipe_to_serv[1]);
    }

    if (req.ShouldSendRequestBody()) {
        if (write(pipe_to_cgi[1], req.body().c_str(), req.body().size()) ==
            -1) {
            std::runtime_error("Error: write failed " +
                               std::string(strerror(errno)));
        }
    }
    close(pipe_to_cgi[1]);
    int exit_status = 0;
    if (wait(&exit_status) == -1) {
        std::runtime_error("Error: wait failed " +
                           std::string(strerror(errno)));
    }

    char buf[4096];
    bzero(buf, 4096);
    ssize_t byte = read(pipe_to_serv[0], buf, 4096);
    close(pipe_to_serv[0]);
    if (byte == -1) {
        std::runtime_error("Error: read failed " +
                           std::string(strerror(errno)));
    }
    buf[byte] = '\0';
    return CGIResponse(std::string(buf));
}
