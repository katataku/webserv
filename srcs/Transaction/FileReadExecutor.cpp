#include "FileReadExecutor.hpp"

#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"

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
    std::ifstream ifs(file_path.c_str());
    std::string str;
    std::ostringstream oss;

    oss << ifs.rdbuf();

    logging_.Debug("file read finished");
    logging_.Debug(oss.str());
    return ResponseBuilder::Build(oss.str());
}

HTTPResponse *FileReadExecutor::Exec(HTTPRequest const &request,
                                     ServerLocation const &sl) {
    logging_.Debug("Function starts");
    struct stat stat_buf;
    std::string alias_resolved_uri = sl.ResolveAlias(request.uri());

    logging_.Debug("alias_resolved_uri = [" + alias_resolved_uri + "]");

    if (stat(alias_resolved_uri.c_str(), &stat_buf) == -1) {
        logging_.Fatal("stat failed");
        logging_.Fatal(strerror(errno));
        return NULL;
    }

    if (S_ISREG(stat_buf.st_mode)) {
        logging_.Debug("URI indicate regular file.");
        return GetFileExec(alias_resolved_uri);
        //        return GetFileExecutor(request, sl);
    }
    logging_.Info("Function ends abnormally");
    return NULL;
}
