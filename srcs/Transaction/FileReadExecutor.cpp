#include "FileReadExecutor.hpp"

#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ResponseBuilder.hpp"
FileReadExecutor::FileReadExecutor() : logging_(Logging(__FUNCTION__)) {}

FileReadExecutor::FileReadExecutor(FileReadExecutor const &other) {
    *this = other;
}

FileReadExecutor &FileReadExecutor::operator=(FileReadExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileReadExecutor::~FileReadExecutor() {}

HTTPResponse *FileReadExecutor::GetFileExec(std::string file_path) {
    std::ifstream ifs(file_path);
    std::string str;
    std::ostringstream oss;

    while (getline(ifs, str)) {
        oss << str << std::endl;
    }
    logging_.Debug("file read finished");
    logging_.Debug(oss.str());
    return ResponseBuilder::Build(oss.str());
}

HTTPResponse *FileReadExecutor::Exec(HTTPRequest const &request,
                                     ServerLocation const &sl) {
    logging_.Debug("Function starts");
    struct stat stat_buf;
    std::string file_path = sl.alias() + request.uri();
    logging_.Debug("file_path = [" + file_path + "]");

    if (stat(file_path.c_str(), &stat_buf) == -1) {
        logging_.Fatal("stat failed");
        logging_.Fatal(strerror(errno));
    }

    if (S_ISREG(stat_buf.st_mode)) {
        logging_.Debug("URI indicate regular file.");
        return GetFileExec(file_path);
        //        return GetFileExecutor(request, sl);
    }
    logging_.Info("Function ends abnormally");
    return NULL;
}
