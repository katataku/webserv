#include "FileDeleteExecutor.hpp"

#include <sys/stat.h>

#include <cstdio>
#include <string>

#include "HTTPException.hpp"
#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"
#include "utils.hpp"

FileDeleteExecutor::FileDeleteExecutor() {}

FileDeleteExecutor::FileDeleteExecutor(FileDeleteExecutor const &other) {
    *this = other;
}

FileDeleteExecutor &FileDeleteExecutor::operator=(
    FileDeleteExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileDeleteExecutor::~FileDeleteExecutor() {}

static bool HasAllPermission(const std::string &path) {
    return access(path.c_str(), R_OK | W_OK | X_OK);
}

static bool IsFile(const std::string &path) {
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        throw HTTPException(500);
    }
    return S_ISREG(st.st_mode);
}

static bool IsDir(const std::string &path) {
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        throw HTTPException(500);
    }
    return S_ISDIR(st.st_mode);
}

HTTPResponse *FileDeleteExecutor::Exec(HTTPRequest const &request,
                                       ServerLocation const &sl) {
    std::string path = sl.ResolveAlias(request.canonical_path());

    if (!IsExist(path)) {
        throw HTTPException(404);
    }

    if ((IsFile(path) && HasAllPermission(Dir(path))) ||
        (IsDir(path) && HasAllPermission(path))) {
        throw HTTPException(403);
    }

    if (std::remove(path.c_str()) != 0) {
        throw HTTPException(500);
    }

    return ResponseBuilder::BuildNoBody(200);
}
