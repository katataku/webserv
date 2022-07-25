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

static void ResisterEnv(std::map<std::string, std::string> env) {
    for (std::map<std::string, std::string>::iterator itr = env.begin();
         itr != env.end(); ++itr) {
        if (setenv(itr->first.c_str(), itr->second.c_str(), 1) == -1) {
            throw std::runtime_error("Error: setenv failed " +
                                     std::string(strerror(errno)));
        }
    }
}

static char **MakeArg(std::vector<std::string> arg) {
    char **av = new char *[arg.size() + 1];
    for (std::vector<std::string>::size_type i = 0; i != arg.size(); ++i) {
        av[i] = const_cast<char *>(arg[i].c_str());
    }
    av[arg.size()] = NULL;
    return av;
}

extern char **environ;

static int execve(const std::string &path, std::vector<std::string> arg,
                  std::map<std::string, std::string> env) {
    char **av = MakeArg(arg);
    ResisterEnv(env);

    int ret = execve(path.c_str(), av, environ);

    delete[] av;

    return ret;
}

static int write(int fd, const std::string &msg) {
    return write(fd, msg.c_str(), msg.size());
}

static std::string read(int fd) {
    char buf[4096];
    ssize_t byte = 0;
    std::string ret;

    for (;;) {
        byte = read(fd, buf, 4096);
        // 全て読み込んだら
        if (byte == 0) {
            break;
        }
        if (byte == -1) {
            throw std::runtime_error("Error: read failed " +
                                     std::string(strerror(errno)));
        }
        buf[byte] = '\0';
        ret += std::string(buf);

        // 読み込んだバイト数が定数より少ない場合は全部読み込んだ
        if (byte < 4096) {
            break;
        }
    }

    return ret;
}

// TODO(iyamada) エラー処理
CGIResponse CGIExecutor::CGIExec(CGIRequest const &req,
                                 ServerLocation const &sl) {
    (void)sl;

    int pipe_to_cgi[2], pipe_to_serv[2];

    if (pipe(pipe_to_cgi) == -1 || pipe(pipe_to_serv) == -1) {
        throw std::runtime_error("Error: pipe failed " +
                                 std::string(strerror(errno)));
    }

    if (fork() == 0) {
        dup2(pipe_to_cgi[0], STDIN_FILENO);
        dup2(pipe_to_serv[1], STDOUT_FILENO);
        if (execve(req.path(), req.arg(), req.env()) == -1) {
            throw std::runtime_error("Error: execve failed " +
                                     std::string(strerror(errno)));
        }
    }

    // TODO(iyamada)
    // bodyは設定されていなかったら空文字列なのでifいらないかも
    if (req.ShouldSendRequestBody()) {
        if (write(pipe_to_cgi[1], req.body()) == -1) {
            throw std::runtime_error("Error: write failed " +
                                     std::string(strerror(errno)));
        }
    }
    close(pipe_to_cgi[1]);

    int exit_status = 0;
    if (wait(&exit_status) == -1) {
        throw std::runtime_error("Error: wait failed " +
                                 std::string(strerror(errno)));
    }
    // TODO(iyamada) exit_status!=0の時、500を返すようにする

    std::string buf = read(pipe_to_serv[0]);
    close(pipe_to_serv[0]);
    return CGIResponse(std::string(buf));
}
