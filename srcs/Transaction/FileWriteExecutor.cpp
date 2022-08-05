#include "FileWriteExecutor.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>

#include "HTTPException.hpp"
#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"

FileWriteExecutor::FileWriteExecutor() : logging_(Logging(__FUNCTION__)) {}

FileWriteExecutor::FileWriteExecutor(FileWriteExecutor const &other) {
    *this = other;
}

FileWriteExecutor &FileWriteExecutor::operator=(
    FileWriteExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileWriteExecutor::~FileWriteExecutor() {}

static bool IsDirectory(const std::string &path) {
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        throw HTTPException(500);
    }
    return S_ISDIR(st.st_mode);
}

static void CreateFileAndWrite(const std::string &path,
                               const std::string &content) {
    std::ofstream ofs(path.c_str());

    if (!ofs) {
        throw HTTPException(500);
    }

    ofs << content;
}

HTTPResponse *FileWriteExecutor::Exec(HTTPRequest const &request,
                                      ServerLocation const &sl) {
    std::string path = sl.ResolveAlias(request.canonical_path());

    if (!IsExist(Dir(path))) {
        throw HTTPException(404);
    }
    if (IsExist(path) && IsDirectory(path)) {
        throw HTTPException(403);
    }

    CreateFileAndWrite(path, request.request_body());

    return ResponseBuilder::BuildNoBody(201);
}
