#ifndef SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_
#define SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_

#include <string>
#include <vector>

#include "HTTPResponse.hpp"
#include "IExecutor.hpp"
#include "Logging.hpp"

enum FileStatKind { STAT_FILE_NOT_EXIST, STAT_REGULAR_FILE, STAT_DIRECTORY };

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
    HTTPResponse *ListDirectoryExec(std::string request_uri,
                                    std::string alias_resolved_uri);
    static std::string BuildListPage(std::string request_uri,
                                     std::vector<std::string> files);
    int CheckFileStat(std::string file_path);
};

#endif  // SRCS_TRANSACTION_FILEREADEXECUTOR_HPP_
