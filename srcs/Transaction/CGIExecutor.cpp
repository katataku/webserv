#include "CGIExecutor.hpp"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <map>
#include <string>
#include <vector>

#include "CGIRequest.hpp"
#include "CGIResponse.hpp"
#include "HTTPException.hpp"

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
    logging_.Debug("Exec start");
    CGIRequest cgi_req(request, sl);
    CGIResponse cgi_res = this->CGIExec(cgi_req);

    // POSTだったらステータスコードを201にする
    if (request.method() == "POST") {
        cgi_res.set_status_code("201");
    }

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

static int read(int fd, std::string *ret) {
    char buf[4096];
    ssize_t byte = 0;

    for (;;) {
        byte = read(fd, buf, 4096);
        // 全て読み込んだら
        if (byte == 0) {
            break;
        }
        if (byte == -1) {
            return -1;
        }
        buf[byte] = '\0';
        *ret += std::string(buf);

        // 読み込んだバイト数が定数より少ない場合は全部読み込んだ
        if (byte < 4096) {
            break;
        }
    }

    return 0;
}

// タイムアウトすると1を返す
static int wait_until(pid_t pid, int *status, int second) {
    int cnt = 0;
    int step_us = 10000;  // 10ms
    int target = second * 1000000 / step_us;
    while (waitpid(pid, status, WNOHANG) == 0) {
        if (cnt == target) {
            kill(pid, SIGKILL);
            return 1;
        }
        usleep(step_us);
        cnt++;
    }
    return 0;
}

static void close_pipe(int pipe[2]) {
    close(pipe[0]);
    close(pipe[1]);
}

// TODO(iyamada) エラー処理
CGIResponse CGIExecutor::CGIExec(CGIRequest const &req) {
    logging_.Debug("CGIExec start");
    int pipe_to_cgi[2], pipe_to_serv[2];

    if (pipe(pipe_to_cgi) == -1) {
        throw HTTPException(500);
    }

    if (pipe(pipe_to_serv) == -1) {
        close_pipe(pipe_to_cgi);
        throw HTTPException(500);
    }

    pid_t pid = fork();
    if (pid == -1) {
        close_pipe(pipe_to_cgi);
        close_pipe(pipe_to_serv);
        throw HTTPException(500);
    }

    if (pid == 0) {
        // 必要ないパイプはクローズ
        close(pipe_to_cgi[1]);
        close(pipe_to_serv[0]);

        dup2(pipe_to_cgi[0], STDIN_FILENO);
        dup2(pipe_to_serv[1], STDOUT_FILENO);
        execve(req.path(), req.arg(), req.env());
        // execvが-1を返すとき、子プロセスを終了するためexit
        exit(1);
    }
    // 必要ないパイプはクローズ
    close(pipe_to_cgi[0]);
    close(pipe_to_serv[1]);

    if (req.ShouldSendRequestBody()) {
        if (write(pipe_to_cgi[1], req.body()) == -1) {
            kill(pid, SIGKILL);
            close(pipe_to_cgi[1]);
            close(pipe_to_serv[0]);
            throw HTTPException(500);
        }
    }
    close(pipe_to_cgi[1]);

    int exit_status = 0;
    if (wait_until(pid, &exit_status, 5) != 0 || exit_status != 0) {
        close(pipe_to_serv[0]);
        throw HTTPException(500);
    }
    logging_.Debug("CGIExec finish wait: exit_status=" + numtostr(exit_status));

    std::string buf;
    if (read(pipe_to_serv[0], &buf) != 0) {
        close(pipe_to_serv[0]);
        throw HTTPException(500);
    }

    logging_.Debug("CGIExec finish");
    return CGIResponse(std::string(buf));
}
