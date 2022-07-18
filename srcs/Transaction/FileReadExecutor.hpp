#ifndef SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_
#define SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_

#include <string>

#include "HTTPResponse.hpp"
#include "IExecutor.hpp"
#include "Logging.hpp"

class FileReadExecutor : public IExecutor {
 public:
    FileReadExecutor();
    FileReadExecutor(FileReadExecutor const &other);
    FileReadExecutor &operator=(FileReadExecutor const &other);
    virtual ~FileReadExecutor();

    virtual HTTPResponse *Exec(HTTPRequest const &request,
                               ServerLocation const &sl);

 private:
    Logging logging_;
    HTTPResponse *GetFileExec(std::string file_path);
};

#endif  // SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_
