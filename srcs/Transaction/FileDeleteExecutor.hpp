#ifndef SRCS_TRANSACTION_FILEDELETEEXECUTOR_HPP_
#define SRCS_TRANSACTION_FILEDELETEEXECUTOR_HPP_

#include "HTTPResponse.hpp"
#include "IExecutor.hpp"
#include "Logging.hpp"

class FileDeleteExecutor : public IExecutor {
 public:
    FileDeleteExecutor();
    FileDeleteExecutor(FileDeleteExecutor const &other);
    FileDeleteExecutor &operator=(FileDeleteExecutor const &other);

    virtual ~FileDeleteExecutor();

    virtual HTTPResponse *Exec(HTTPRequest const &request,
                               ServerLocation const &sl);

 private:
};

#endif  // SRCS_TRANSACTION_FILEDELETEEXECUTOR_HPP_
