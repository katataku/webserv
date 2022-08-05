#ifndef SRCS_TRANSACTION_FILEWRITEEXECUTOR_HPP_
#define SRCS_TRANSACTION_FILEWRITEEXECUTOR_HPP_

#include <string>
#include <vector>

#include "HTTPResponse.hpp"
#include "IExecutor.hpp"
#include "Logging.hpp"

class FileWriteExecutor : public IExecutor {
 public:
    FileWriteExecutor();
    FileWriteExecutor(FileWriteExecutor const &other);
    FileWriteExecutor &operator=(FileWriteExecutor const &other);
    virtual ~FileWriteExecutor();

    virtual HTTPResponse *Exec(HTTPRequest const &request,
                               ServerLocation const &sl);

 private:
    Logging logging_;
};

#endif  // SRCS_TRANSACTION_FILEWRITEEXECUTOR_HPP_
